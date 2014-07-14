from random import uniform
from math import exp

file = open('vladislavleva-1-train.data', 'w')
for i in range(100):
	x = uniform(0.3, 4)
	y = uniform(0.3, 4)
	value = exp(-(x-1) ** 2) / (1.2 + (y - 2.5) ** 2)
	file.write('%s,X1=%s,X2=%s\n' % (value, x, y))
file.close()
file = open('vladislavleva-1-test.data', 'w')
for i in range(45):
	for j in range(45):
		x = -0.2 + 0.1 * i
		y = -0.2 + 0.1 * j
		value = exp(-(x-1) ** 2) / (1.2 + (y - 2.5) ** 2)
		file.write('%s,X1=%s,X2=%s\n' % (value, x, y))
file.close()
