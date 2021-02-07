import sys
import json
import string
import random

'''
to recreate html_entities.cpp, call
curl https://html.spec.whatwg.org/entities.json | python3 utils/src/html_entities.py > utils/src/html_entities.cpp
'''

# set variable below to True to get one possible content for the "nosuch" tests
WRITE_WRONG_ID = False

names = [None, {}]
letters = string.ascii_letters + string.digits


def printable(b):
    if b > 31 and b < 127:
        return chr(b).replace('\\', '\\\\').replace('"', '\\"')
    return '\\x{:02x}'.format(b)


def prefix(name, value):
    ctx = names
    for ch in name:
        if ch not in ctx[1]:
            ctx[1][ch] = [None, {}]
        ctx = ctx[1][ch]
    ctx[0] = value


def isLeafy(ctx):
    if ctx[0] is not None:
        return False
    if len(ctx[1]) != 1:
        return False
    key = list(ctx[1].keys())[0]
    return len(ctx[1][key][1]) == 0


def rewrite(ctx):
    for key in ctx[1]:
        rewrite(ctx[1][key])
    if len(ctx[1]) != 1:
        return
    key = list(ctx[1].keys())[0]
    subctx = ctx[1][key]
    if not isLeafy(subctx):
        return
    subkey = list(subctx[1].keys())[0]
    newkey = key + subkey
    ctx[1][newkey] = subctx[1][subkey]
    del ctx[1][key]


def denonify(ctx):
    if len(ctx[1]) == 0:
        return ctx[0]
    result = {}
    for key in ctx[1]:
        result[key] = denonify(ctx[1][key])
    if ctx[0] is None:
        return result
    return [ctx[0], result]


def make_wrong_id(keyprefix, keyset):
    if not WRITE_WRONG_ID:
        return

    while True:
        letter = random.choice(letters)
        if letter in keyset:
            continue
        wrong = keyprefix + letter + \
            ''.join(random.choice(letters) for i in range(2))
        print('\t    {{"{}"sv}},'.format(wrong), file=sys.stderr)
        return


def cxx_switch_char(ctx, level, prefix='  ', keyprefix=''):
    if isinstance(ctx, list):
        value, ctx = ctx
        if len(ctx) == 0:
            make_wrong_id(keyprefix, {})
        fun = 'ON_LEAF' if len(ctx) == 0 else 'ON_LEAF_NOBR'
        print('{}{}({}, "{}");  // &{};'.format(
            prefix, fun, level, value, keyprefix))

    if len(ctx) == 1:
        key = list(ctx.keys())[0]
        if len(key) > 1:
            value = ctx[key]
            make_wrong_id(keyprefix, {key[0]})
            print('{}ON_SUFFIX({}, "{}", "{}");  // &{}{};'.format(
                prefix, level, key, value, keyprefix, key))
            return
        elif len(key) == 1:
            subctx = ctx[key]
            if isinstance(subctx, str):
                make_wrong_id(keyprefix, {key})
                print('{}ON_SUFFIX({}, "{}", "{}");  // &{}{};'.format(
                    prefix, level, key, subctx, keyprefix, key))
                return

    print('{}CHAR_AT({})'.format(prefix, level))

    make_wrong_id(keyprefix, set(ctx.keys()))

    for key in sorted(ctx.keys()):
        subctx = ctx[key]
        if isinstance(subctx, str):
            make_wrong_id(keyprefix + key, {})
            print('{}  CHAR(\'{}\') ON_LEAF({}, "{}");  // &{}{};'.format(
                prefix, key, level + 1, subctx, keyprefix, key))
            continue
        if isinstance(subctx, dict) and len(subctx) == 1:
            subkey = list(subctx.keys())[0]
            if len(subkey) > 1:
                value = subctx[subkey]
                make_wrong_id(keyprefix + key, {subkey[0]})
                print('{}  CHAR(\'{}\') ON_SUFFIX({}, "{}", "{}");  // &{}{}{};'.format(
                    prefix, key, level + 1, subkey, value, keyprefix, key, subkey))
                continue
            elif len(subkey) == 1:
                subsubctx = subctx[subkey]
                if isinstance(subsubctx, str):
                    make_wrong_id(keyprefix + key, {subkey})
                    print('{}  CHAR(\'{}\') ON_SUFFIX({}, "{}", "{}");  // &{}{}{};'.format(
                        prefix, key, level + 1, subkey, subsubctx, keyprefix, key, subkey))
                    continue

        print('{}  CHAR(\'{}\')'.format(prefix, key))
        cxx_switch_char(ctx[key], level + 1, prefix + '    ', keyprefix + key)
    if level:
        print('{}CHAR_END()'.format(prefix))
    else:
        print('{}CHAR_END_NOBR()'.format(prefix))


entities = json.load(sys.stdin)

differ = []
ignored = set()

for name in entities:
    if name[-1] == ';':
        continue
    remap = name + ';'
    if remap not in entities:
        continue
    ignored.add(name)
    orig = entities[name]['codepoints']
    mapped = entities[remap]['codepoints']
    if orig != mapped:
        differ.append(name)

if len(differ):
    print('warning: some entities has divergent values; version without semicolon will be ignored', file=sys.stderr)

for name in entities:
    if name in ignored:
        continue

    cp = entities[name]['codepoints']
    value_bytes = list(''.join([chr(point) for point in cp]).encode('UTF-8'))
    value = ''.join([printable(b) for b in value_bytes])

    name = name[1:]
    if name[-1] == ';':
        name = name[:-1]

    prefix(name, value)

rewrite(names)
names = denonify(names)

print('''// clang-format off
// This file is generated, to recreate, call
// curl https://html.spec.whatwg.org/entities.json | python3 utils/src/html_entities.py > utils/src/html_entities.cpp

#include <tangle/html_entities.hpp>

using namespace std::literals;

#define CHAR_AT(NO) if (length > NO) { switch (name[NO]) {
#define CHAR(C) case C:
#define CHAR_END_NOBR() }}
#define CHAR_END() }} break;
#define ON_SUFFIX(NO, suffix, result) \\
  if (name.substr(NO) == suffix ## sv) { return result; } \\
  break
#define ON_LEAF_NOBR(NO, result) if (length == NO) return result
#define ON_LEAF(NO, result) ON_LEAF_NOBR(NO, result); break

namespace tangle {
char const* html_entity(std::string_view name) {
  auto const length = name.length();
''')

if WRITE_WRONG_ID:
    print('\t    {""sv},', file=sys.stderr)
cxx_switch_char(names, 0)

print('''

  return nullptr;
}
}  // namespace tangle

#undef CHAR_AT
#undef CHAR
#undef CHAR_END_NOBR
#undef CHAR_END
#undef ON_SUFFIX
#undef ON_LEAF_NOBR
#undef ON_LEAF

// clang-format on''')
