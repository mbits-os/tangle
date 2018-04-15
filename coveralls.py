from __future__ import unicode_literals

import os
import errno
import sys
import argparse
import subprocess
import json
import hashlib

parser = argparse.ArgumentParser(description='Gather GCOV data for Coveralls')
parser.add_argument('--git',     required=True, help='path to the git binary')
parser.add_argument('--gcov',    required=True, help='path to the gcov program')
parser.add_argument('--src_dir', required=True, help='directory for source files')
parser.add_argument('--bin_dir', required=True, help='directory for generated files')
parser.add_argument('--int_dir', required=True, help='directory for temporary gcov files')
parser.add_argument('--dirs',    required=True, help='directory filters for relevant sources, separated with\':\'')
parser.add_argument('--out',     required=True, help='output JSON file for Coveralls')
args = parser.parse_args();
args.dirs = args.dirs.split(':')
for idx in range(len(args.dirs)):
	dname = args.dirs[idx].replace('\\', os.sep).replace('/', os.sep)
	if dname[len(dname) - 1] != os.path.sep:
		dname += os.path.sep
	args.dirs[idx] = dname

class cd:
	def __init__(self, dirname):
		self.dirname = os.path.expanduser(dirname)
	def __enter__(self):
		self.saved = os.getcwd()
		os.chdir(self.dirname)
	def __exit__(self, etype, value, traceback):
		os.chdir(self.saved)

def mkdir_p(path):
	try:
		os.makedirs(path)
	except OSError as exc:  # Python >2.5
		if exc.errno == errno.EEXIST and os.path.isdir(path):
			pass
		else:
			raise

def run(*args):
	p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, err = p.communicate();
	return (out, err, p.returncode)

def output(*args):
	return run(*args)[0].strip().decode('utf-8')

def git_log_format(fmt):
	return output(args.git, 'log', '-1', '--pretty=format:%' + fmt)

def gcov(dir_name, gcdas):
	out, err, code = run(args.gcov, '-p', '-o', dir_name, *gcdas)
	if code:
		print >>sys.stderr, err
		print >>sys.stderr, 'error:', code
		sys.exit()

def recurse(root, ext):
	for dirname, ign, files in os.walk(root):
		for f in files:
			split = os.path.splitext(f)
			if len(split) < 2 or split[1] != ext:
				continue
			yield os.path.join(dirname, f)

def ENV(name):
	if name in os.environ:
		return os.environ[name]
	return ''

def file_md5(path):
	m = hashlib.md5()
	with open(path, 'rb') as f:
		for line in f:
			m.update(line)
	return m.hexdigest()

def merge_coverage(dst, src):
	if len(dst) < len(src):
		dst += [None]*(len(src) - len(dst))
	ndx = 0
	for sample in src:
		if sample is None:
			ndx += 1
			continue
		if dst[ndx] is None:
			dst[ndx] = sample
			ndx += 1
			continue
		dst[ndx] += sample
		ndx += 1

def merge_file(sources, name, source_digest, coverage):
	name = name.replace('\\', '/')
	for i in range(len(sources)):
		if sources[i]['name'] != name:
			continue
		merge_coverage(sources[i]['coverage'], coverage)
		return
	sources.append({
		'name': name,
		'source_digest': source_digest,
		'coverage': coverage
	})

# for key in sorted(os.environ.keys()):
# 	if len(key) < 8 or key[:7] != 'TRAVIS_': continue
# 	sys.stdout.write("{} = {}\n".format(key, os.environ[key]))

job_id = ENV('TRAVIS_JOB_ID')
service = ''
if job_id:
	service = 'travis-ci'
	sys.stdout.write("Preparing Coveralls for Travis-CI job {}.\n".format(job_id))

JSON = {
  'service_name': service,
  'service_job_id': job_id,
  'repo_token': ENV('COVERALLS_REPO_TOKEN'),
  'git': {},
  'source_files': []
}

with cd(args.src_dir):
	JSON['git'] = {
		'branch': output(args.git, 'rev-parse', '--abbrev-ref', 'HEAD'),
		'head': {
			'id': git_log_format('H'),
			'author_name': git_log_format('an'),
			'author_email': git_log_format('ae'),
			'committer_name': git_log_format('cn'),
			'committer_email': git_log_format('ce'),
			'message': git_log_format('B')
		},
		'remotes': []
	}

gcda_dirs = {}
for gcda in recurse(os.path.abspath(args.bin_dir), '.gcda'):
	dirn, filen = os.path.split(gcda)
	if dirn not in gcda_dirs:
		gcda_dirs[dirn] = []
	gcda_dirs[dirn].append(filen)

for dirn in gcda_dirs:
	int_dir = os.path.relpath(dirn, args.bin_dir).replace(os.sep, '#')
	int_dir = os.path.join(args.int_dir, int_dir)
	mkdir_p(int_dir)
	with cd(int_dir):
		gcov(dirn, [os.path.join(dirn, filen) for filen in gcda_dirs[dirn]])

def gcov_source(filename):
	with open(filename) as src:
		for line in src:
			split = line.split(' 0:', 1)
			if len(split) < 2:
				break
			split = split[1].split('Source:', 1)
			if len(split) < 2:
				continue
			return os.path.normpath(os.path.join(args.bin_dir, split[1].rstrip()))
	return None

src_dir = os.path.abspath(args.src_dir)
for stats in recurse(args.int_dir, '.gcov'):
	src = gcov_source(stats)
	if src[0:len(src_dir)] != src_dir: continue
	name = src[len(src_dir):]
	if name[0] != os.sep: continue
	name = name[1:]
	relevant = False
	for dname in args.dirs:
		if len(dname) < len(name) and name[:len(dname)] == dname:
			relevant = True
			break
	if not relevant: continue

	coverage = []

	with open(stats) as stats_file:
		for line in stats_file:
			chunks = line.split(':', 2)
			if len(chunks) != 3: continue
			ln = chunks[1].strip()
			if ln == '0': continue
			st = chunks[0].strip()
			if st == '-': st = None
			elif st in ['#####', '=====']: st = 0
			else: st = int(st)
			coverage.append(st)

	merge_file(JSON['source_files'], name, file_md5(src), coverage)

with open(args.out, 'w') as j:
	json.dump(JSON, j)
