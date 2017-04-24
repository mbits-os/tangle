import os, sys, argparse, json

parser = argparse.ArgumentParser(description='Gather GCOV data for Coveralls')
parser.add_argument('--src_dir', required=True, help='directory for source files')
parser.add_argument('--in',      required=True, help='Coveralls JSON file', dest='json')
args = parser.parse_args()

class cd:
	def __init__(self, dirname):
		self.dirname = os.path.expanduser(dirname)
	def __enter__(self):
		self.saved = os.getcwd()
		os.chdir(self.dirname)
	def __exit__(self, etype, value, traceback):
		os.chdir(self.saved)

def mkdir_p(name):
	pass

with open(args.json) as io:
	data = json.load(io)

repo_head = data['git']['head']
files = data['source_files']

print 'branch: {}'.format(data['git']['branch'])
print 'commit: {}'.format(repo_head['id'])
print 'author: {} <{}>'.format(repo_head['author_name'], repo_head['author_email'])
if repo_head['author_email'] != repo_head['committer_email']:
	print 'committer: {} <{}>'.format(repo_head['committer_name'], repo_head['committer_email'])
print

total_lines = 0
total_covered = 0
total_visited = 0

stats = {}
dirs = {}

for filedef in files:
	name = filedef['name']
	coverage = filedef['coverage']

	lines = 0
	covered = 0
	visited = 0

	for stat in coverage:
		if stat is None: continue
		lines += 1
		visited += stat
		if stat: covered += 1

	total_lines += lines
	total_covered += covered
	total_visited += visited


	per = 0
	avg = 0
	if lines:
		per = (100.0 * covered) / lines
		avg = (100 * visited) / lines

	dirn, filen = os.path.split(name)
	if dirn not in stats:
		stats[dirn] = []
	if dirn not in dirs:
		dirs[dirn] = [0, 0, 0, 0]
	dirs[dirn][0] += lines
	dirs[dirn][1] += covered
	dirs[dirn][2] += visited
	stats[dirn].append((per, lines, covered, avg/100, avg%100, filen))

dirs = [
	((100.0 * dirs[key][1]) / dirs[key][0],
	dirs[key][0],
	dirs[key][1],
	(100*dirs[key][2]/dirs[key][0])/100,
	(100*dirs[key][2]/dirs[key][0])%100,
	key) for key in dirs]

per = 0
avg = 0
if total_lines:
	per = 100 * total_covered / total_lines
	avg = 100 * total_visited / total_lines
print '{:6.2f}% {:6} {:6} {:6}.{:02} TOTAL'.format(per, total_lines, total_covered, avg / 100, avg % 100)
print

dirs = sorted(dirs, key=lambda stat: stat[0])
for stat in dirs:
	print '{:6.2f}% {:6} {:6} {:6}.{:02} {}'.format(*stat)
	files = sorted(stats[stat[5]], key=lambda stat: stat[0])
	for sub in files:
		print '{:6.2f}% {:6} {:6} {:6}.{:02}     {}'.format(*sub)
	print
