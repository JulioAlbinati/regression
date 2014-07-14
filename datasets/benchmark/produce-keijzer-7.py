from random import uniform
from math import log

file = open('keijzer-7-train.data', 'w')
for x in range(1, 101):
	value = log(x)
	file.write('%s,X1=%s\n' % (value, x))
file.close()

file = open('keijzer-7-test.data', 'w')
for i in range(991):
	x = 1 + 0.1 * i
	value = log(x)
	file.write('%s,X1=%s\n' % (value, x))
file.close()
