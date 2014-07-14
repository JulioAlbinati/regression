from random import uniform
from math import cos, sin

file = open('vladislavleva-4-train.data', 'w')
for i in range(1, 1025):
	inst = []
	den = 0.0
	for j in range(1, 6):
		inst.append(uniform(0.05, 6.05))
		den += (inst[j - 1] - 3) ** 2

	value = 10.0 / 5 + den
	file.write('%s,X1=%s,X2=%s,X3=%s,X4=%s,X5=%s\n' % (value, inst[0], inst[1], inst[2], inst[3], inst[4]))
file.close()

file = open('vladislavleva-4-test.data', 'w')
for i in range(1, 5001):
	inst = []
	den = 0.0
	for j in range(1, 6):
		inst.append(uniform(-0.25, 6.35))
		den += (inst[j - 1] - 3) ** 2

	value = 10.0 / 5 + den
	file.write('%s,X1=%s,X2=%s,X3=%s,X4=%s,X5=%s\n' % (value, inst[0], inst[1], inst[2], inst[3], inst[4]))
file.close()
