from random import randint
from sys import stdin, argv

name = argv[1]

lines = []
for l in stdin:
	lines.append(l.strip())

test = []
num = int(0.3 * len(lines))
for i in range(num):
	index = randint(0, len(lines) - 1)
	test.append(lines[index])
	lines.remove(lines[index])

file = open('%s-test.data' % name, 'w')
for l in test:
	file.write('\n%s' % l)
file.close()

file = open('%s-train.data' % name, 'w')
for l in lines:
	file.write('\n%s' % l)
file.close()
