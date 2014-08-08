from sys import argv
from random import gauss
from math import sqrt, pow

if __name__ == '__main__':
	filename = argv[1]
	file = open(filename)
	lines = file.readlines()
	file.close()

	degree = float(argv[2])
	targets = []
	mean = 0.0
	for line in lines:
		tokens = line.strip().split(',')
		target = float(tokens[0])
		mean += target
		targets.append(target)
	mean /= len(targets)

	sd = 0.0
	for t in targets:
		sd += pow(t - mean, 2)
	sd = sqrt(sd / len(targets))

	for line in lines:
		tokens = line.strip().split(',')
		target = float(tokens[0])
		print '%s,%s' % (target + gauss(0, sd * degree), ','.join(tokens[1:]))
