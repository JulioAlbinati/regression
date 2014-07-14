from random import uniform
from math import cos, sin

file = open('korns-12-train.data', 'w')
for i in range(1, 10001):
	inst = []
	for j in range(1, 6):
		inst.append(uniform(-50, 50))

	value = 2.0 - 2 * cos(9.8 * inst[1]) * sin(1.3 * inst[3])
	file.write('%s,X1=%s,X2=%s,X3=%s,X4=%s,X5=%s\n' % (value, inst[0], inst[1], inst[2], inst[3], inst[4]))
file.close()

file = open('korns-12-test.data', 'w')
for i in range(1, 10001):
	inst = []
	for j in range(1, 6):
		inst.append(uniform(-50, 50))

	value = 2.0 - 2 * cos(9.8 * inst[1]) * sin(1.3 * inst[3])
	file.write('%s,X1=%s,X2=%s,X3=%s,X4=%s,X5=%s\n' % (value, inst[0], inst[1], inst[2], inst[3], inst[4]))
file.close()
