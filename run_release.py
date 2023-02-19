import json
import os
import sys
import subprocess
from typing import Callable
from contextlib import contextmanager
import shutil

__dir__ = os.path.dirname(__file__)
__root__ = os.path.join(__dir__, "CMakePresets.json")

__build__ = "release"
__config__ = "{}-{}".format(__build__, "msbuild" if os.name == "nt" else "ninja")


def which(name: str) -> str:
    pathext_var = os.environ.get("PATHEXT")
    if pathext_var is None:
        return name

    pathext = pathext_var.lower().split(os.path.pathsep)
    if os.path.sep in name:
        split = os.path.splitext(name)
        if len(split) < 2 or split[1].lower() not in pathext:
            for ext in pathext:
                result = f"{name}{ext}"
                if os.path.exists(result):
                    return result
        if os.path.exists(name):
            return name
        return None
    for dirname in os.environ["PATH"].split(os.path.pathsep):
        for ext in pathext:
            result = os.path.join(dirname, f"{name}{ext}")
            if os.path.exists(result):
                return result
    return None


def inherit(configs: list[dict], select: Callable):
    lookup = {config["name"]: config for config in configs}
    while True:
        missing = [key for key in lookup if "inherits" in lookup[key]]
        filled = [key for key in lookup if "inherits" not in lookup[key]]
        if not len(missing):
            break
        for key in missing:
            config = lookup[key]
            inherits = [*config.get("inherits", [])]
            for sup in inherits:
                if sup in filled:
                    config["inherits"].remove(sup)
                    if not len(config["inherits"]):
                        del config["inherits"]

                    inherited = lookup[sup]
                    for key in inherited:
                        if key == "hidden":
                            continue
                        if key in config:
                            continue
                        config[key] = inherited[key]
    return {key: select(lookup[key]) for key in lookup}


def load_presets(path):
    path = os.path.abspath(path)
    stack = [path]
    seens: set[str] = set()
    results = {}
    while len(stack):
        path = stack[0]
        stack = stack[1:]

        if path in seens:
            continue

        with open(path, encoding="UTF-8") as input:
            data = json.load(input)
            seens.add(path)

        dirname = os.path.dirname(path)
        include = data.get("include", [])

        for inc in include:
            stack.append(os.path.abspath(os.path.join(dirname, inc)))

        for key in data:
            if key == "include":
                continue
            field = data[key]
            if isinstance(field, list):
                try:
                    results[key].extend(field)
                except KeyError:
                    results[key] = field

    binaryDirs = inherit(
        results.get("configurePresets", []), lambda cfg: cfg.get("binaryDir")
    )
    buildPresets = inherit(
        results.get("buildPresets", []),
        lambda cfg: (
            binaryDirs.get(cfg.get("configurePreset")),
            cfg.get("configuration"),
        ),
    )

    return buildPresets


def run(*cmd):
    print("{}{}{}".format("\033[32m", " ".join(cmd), "\033[m"))
    cmd = [which(cmd[0]), *cmd[1:]]
    proc = subprocess.run(cmd, shell=False)
    if proc.returncode != 0:
        sys.exit(proc.returncode)


@contextmanager
def cd(path):
    os.makedirs(path, exist_ok=True)
    prev = os.getcwd()
    os.chdir(path)
    yield None
    os.chdir(prev)


def conan(profile: str):
    run(
        "conan",
        "install",
        __dir__,
        "--build",
        "missing",
        "-pr:b",
        profile,
        "-pr:h",
        profile,
    )


presets = load_presets(__root__).get(__build__)
if presets is None:
    print(f"Cannot find {__build__} in buildPresets")
    sys.exit(1)
bindir, config = presets
if bindir is None:
    print(f"Cannot find binaryDir attached to {__build__}")
    sys.exit(1)
if config is None:
    print(f"Cannot find configuration attached to {__build__}")
    sys.exit(1)
bindir = os.path.abspath(bindir.replace("${sourceDir}", __dir__))
instdir = os.path.join(os.path.dirname(bindir), "install")

script = [
    ["cmake", "--preset", __config__],
    ["cmake", "--build", "--preset", __build__, "--parallel"],
    ["cmake", "--install", bindir, "--prefix", instdir, "--config", config],
]

with cd(os.path.join(os.path.dirname(bindir), "conan")):
    conan("debug")
    conan("release")


def postinst(src: str, dst: str):
    for parent, dirs, files in os.walk(src):
        dst_parent = os.path.abspath(os.path.join(dst, os.path.relpath(parent, src)))

        for subdir in dirs:
            os.makedirs(os.path.join(dst_parent, subdir), exist_ok=True)
        for filename in files:
            shutil.copyfile(
                os.path.join(parent, filename),
                os.path.join(dst_parent, filename),
            )


for cmd in script:
    run(*cmd)

__bin__ = os.path.abspath(os.path.join(instdir, "bin"))
__tools__ = os.path.abspath(os.path.join("..", "..", "tools", "outfilm", "tools"))
postinst(os.path.join(instdir, "modules"), __dir__)
postinst(os.path.join(instdir, "modules"), __tools__)
postinst(__bin__, __tools__)
