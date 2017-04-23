import os, sys, argparse, subprocess, json, hashlib

parser = argparse.ArgumentParser(description='Gather GCOV data for Coveralls')
parser.add_argument('--git', help='path to the git binary')
parser.add_argument('--gcov', help='path to the gcov program')
parser.add_argument('--src_dir', help='directory for source files')
parser.add_argument('--bin_dir', help='directory for generated files')
parser.add_argument('--int_dir', help='directory for temporary gcov files')
parser.add_argument('--dirs', help='directory filters for relevant sources')
parser.add_argument('--out', help='output JSON file for Coveralls')
args = parser.parse_args();
args.dirs = args.dirs.split(':')
for idx in xrange(len(args.dirs)):
	dname = args.dirs[idx]
	if dname[len(dname) - 1] != os.path.sep:
		args.dirs[idx] += os.path.sep

class cd:
	def __init__(self, dirname):
		self.dirname = os.path.expanduser(dirname)
	def __enter__(self):
		self.saved = os.getcwd()
		os.chdir(self.dirname)
	def __exit__(self, etype, value, traceback):
		os.chdir(self.saved)

def run(*args):
	p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, err = p.communicate();
	return (out, err, p.returncode)

def output(*args):
	return run(*args)[0].strip()

def git_log_format(fmt):
	return output(args.git, 'log', '-1', '--pretty=format:%' + fmt)

def gcov(gcda):
	run(args.gcov, '-p', '-o', os.path.dirname(gcda), gcda)

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

job_id = ENV('TRAVIS_JOB_ID')
if job_id:
	service = 'travis-ci'
	repo_token = ''
	sys.stdout.write("Preparing Coveralls for Travis-CI job {}.\n".format(job_id))
else:
	service = ''
	repo_token = ENV('COVERALLS_REPO_TOKEN')

JSON = {
  'service_name': service,
  'service_job_id': job_id,
  'repo_token': repo_token,
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

with cd(args.int_dir):
	for gcda in recurse(args.bin_dir, '.gcda'):
		gcov(gcda)

for stats in recurse(args.int_dir, '.gcov'):
	src = os.path.splitext(os.path.basename(stats))[0].replace('#', '/')
	if src[0:len(args.src_dir)] != args.src_dir: continue
	name = src[len(args.src_dir):]
	if name[0] != '/': continue
	name = name[1:]
	relevant = False
	for dname in args.dirs:
		if len(dname) < len(name) and name[:len(dname)] == dname:
			relevant = True
			break
	if not relevant: continue

	file = {
		'name': name,
		'source_digest': file_md5(src),
		'coverage': []
	}

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
			file['coverage'].append(st)

	JSON['source_files'].append(file)

with open(args.out, 'w') as j:
	json.dump(JSON, j)
