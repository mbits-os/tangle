from __future__ import print_function
import os, sys, argparse, errno, subprocess, json

parser = argparse.ArgumentParser(description='Gather GCOV data for Coveralls')
parser.add_argument('--in',    required=True,  help='Coveralls JSON file', dest='json')
parser.add_argument('--prev',  required=False, help='Coveralls JSON file for previous build')
parser.add_argument('--out',   required=False, help='Output directory')
args = parser.parse_args()

def escape(s):
	return s.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;').replace('"', '&quot;')

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

def print_header(git):
	repo_head = git['head']
	author = '{name} <{email}>'.format(name=repo_head['author_name'], email=repo_head['author_email'])
	commiter = '{name} <{email}>'.format(name=repo_head['committer_name'], email=repo_head['committer_email'])

	print('branch: {}'.format(git['branch']))
	print('commit: {}'.format(repo_head['id']))
	print('author: {}'.format(author))
	if author != commiter:
		print('commiter: {}'.format(commiter))
	print('\n   ', '\n    '.join(repo_head['message'].split('\n')))
	print()

def percent(value):
	return '{:.2f}%'.format(value)

def average(value):
	return '{}.{:02}'.format(value / 100, value % 100)

def diff_(curr, prev, fmt, conv=str):
	if curr == prev:
		return fmt.format(conv(curr), '', '')
	if curr > prev:
		return fmt.format(conv(curr), '+', conv(curr - prev))
	return fmt.format(conv(curr), '-', conv(prev - curr))

class stats:
	def __init__(self):
		self.coverage = 0.0
		self.lines = 0
		self.relevant = 0
		self.covered = 0
		self.visited = 0
		self.average = 0

	def append(self, stat):
		self.lines += 1
		if stat is None: return
		self.relevant += 1
		if stat:
			self.covered += 1
		self.visited += stat
		self.__update()

	def __str__(self):
		return str((self.lines, self.relevant, self.covered, self.visited))

	def __iadd__(self, rhs):
		self.lines += rhs.lines
		self.relevant += rhs.relevant
		self.covered += rhs.covered
		self.visited += rhs.visited
		self.__update()
		return self

	def __add__(self, rhs):
		tmp = stats()
		tmp += self
		tmp += rhs
		return tmp

	def __update(self):
		if self.relevant:
			self.coverage = 100.0 * self.covered / self.relevant
			self.average = 100 * self.visited / self.relevant
		else:
			self.coverage = 0.0
			self.average = 0

	def txt(self, prev):
		if prev is None:
			prev = stats()
			prev += self

		return '{} {} {} {} {}'.format(
			diff_(self.coverage, prev.coverage, '{:>7}{:>2}{:<7}', percent),
			diff_(self.lines, prev.lines, '{:>8}{:>2}{:<8}'),
			diff_(self.relevant, prev.relevant, '{:>8}{:>2}{:<8}'),
			diff_(self.covered, prev.covered, '{:>8}{:>2}{:<8}'),
			diff_(self.average, prev.average, '{:>8}{:>2}{:<8}', average)
			)

class file_info:
	def __init__(self):
		self.coverage = stats()
		self.prev = None
		self.digest = ''
		self.lines = []

	def txt(self, name):
		return ' {}     {}'.format(
			self.coverage.txt(self.prev),
			os.path.basename(name)
			)

class dir_info:
	def __init__(self):
		self.coverage = stats()
		self.prev = None
		self.files = set()

	def append(self, fcovg, fname):
		self.coverage += fcovg
		self.files.add(fname)

	def append_prev(self, fcovg, fname):
		if self.prev is None:
			self.prev = stats()
		self.prev += fcovg
		self.files.add(fname)

	def txt(self, name):
		return ' {} {}'.format(
			self.coverage.txt(self.prev), name
			)

class sources:
	def __init__(self, files, with_lines = False):
		self.total = stats()
		self.prev = None
		self.dirs = {}
		self.files = {}

		self.stats = {}
		dirs = {}

		for filedef in files:
			info = file_info()
			info.digest = filedef['source_digest']

			for stat in filedef['coverage']:
				info.coverage.append(stat)

			if with_lines:
				info.lines = filedef['coverage']

			name = filedef['name']
			self.files[name] = info

			dname = os.path.dirname(name)
			if dname not in self.dirs:
				self.dirs[dname] = dir_info()
			self.dirs[dname].append(info.coverage, name)
			self.total += info.coverage

	def txt(self):
		return ' {} TOTAL'.format(
			self.total.txt(self.prev)
			)

	def diff_to(self, prev):
		self.prev = stats()
		for fname in prev.files:
			prev_file = prev.files[fname]
			try:
				info = self.files[fname]
			except:
				info = file_info()
				info.digest = None
				self.files[fname] = info
			self.prev += prev_file.coverage 
			info.prev = prev_file.coverage
			dname = os.path.dirname(fname)
			if dname not in self.dirs:
				self.dirs[dname] = dir_info()
			self.dirs[dname].append_prev(prev_file.coverage, fname)

with open(args.json) as io:
	data = json.load(io)

print_header(data['git'])
curr = sources(data['source_files'], True)

if args.prev is not None:
	with open(args.prev) as io:
		prev = json.load(io)
		prev = sources(prev['source_files'])
		curr.diff_to(prev)

print('COVERAGE             TOTAL           RELEVANT            COVERED          HITS/LINE           NAME')
print(curr.txt())
print()

for dname in sorted(curr.dirs.keys()):
	dinfo = curr.dirs[dname]
	print(dinfo.txt(dname))
	for fname in sorted(dinfo.files):
		finfo = curr.files[fname]
		print(finfo.txt(fname))
	print()

css = '''*, td, th {
	box-sizing: border-box;
}

div.content {
	max-width: 1000px;
	margin: 0 auto;
}

table.listing {
	width: 100%;
	padding: 1em;
	border-collapse: separate;
	border-spacing: 0;
}

table.listing tr td {
	color: #555;
	font-family: monospace;
	padding: 0 1em;
	background: rgba(192,192,192,0.1);
}

table.listing tr.success td {
	background: rgba(208,233,153,0.2);
}

table.listing tr.error td {
	background: rgba(216,134,123,0.2);
}

table.listing a {
	color: inherit;
	text-decoration: none;
}

table.listing td:nth-child(odd) {
	text-align: right;
}

table.listing td:nth-child(1) {
	border-right: solid 1px #ddd;
}

table.listing td:nth-child(2) {
	border-left: solid 1px #fff;
}

table.listing td:nth-child(3) {
	font-size: smaller;
}

table.listing pre {
	word-wrap: break-word;
}'''

if args.out is not None:
	mkdir_p(args.out)

	with cd(args.out):
		commit = data['git']['head']['id']
		for fname in curr.files:
			finfo = curr.files[fname]
			if finfo.digest is None:
				continue
			print(finfo.digest, fname)
			content = output('git', 'show', '%s:%s' % (commit, fname)).split('\n')
			with open(finfo.digest + '.html', 'w') as out:
				print('''<html>
<head>
<title>{fname}</title>
<style type="text/css">
{css}
</style>
</head>
<body>
<h1>{fname}</h1>
<div class="content">
<table class="listing">'''.format(fname=escape(fname), css=css, lines=len(finfo.lines)), file=out)
				for lineno in range(len(content)):
					stat = None
					if lineno < len(finfo.lines):
						stat = finfo.lines[lineno]
					klass = ''
					if stat is None:
						stat = ''
					else:
						klass = 'success' if stat else 'error'
						if not stat: stat = ''
						else: stat = '<a href="#L{lineno}">{stat}&times;</a>'.format(stat=stat, lineno=(lineno + 1))
					if klass: klass = ' class="{}"'.format(klass)
					print('<tr{klass}><td><a name="L{lineno}"></a><a href="#L{lineno}">{lineno}</a></td><td><a href="#L{lineno}"><pre>{code}</pre></a></td><td>{stat}</td></tr>'.format(
						klass=klass, lineno=(lineno + 1), code=escape(content[lineno]), stat=stat
						), file=out
					)

				print('''</table>
</div>
</body>
</html>''', file=out)
