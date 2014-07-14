from random import uniform

file = open('keijzer-5-train.data', 'w')
for i in range(1000):
	x = uniform(-1, 1)
	y = uniform(1, 2)
	z = uniform(-1, 1)
	value = (30.0 * x * z) / ((x - 10.0) * y * y)
	file.write('%s,X1=%s,X2=%s,X3=%s\n' % (value, x, y, z))
file.close()

file = open('keijzer-5-test.data', 'w')
for i in range(10000):
	x = uniform(-1, 1)
	y = uniform(1, 2)
	z = uniform(-1, 1)
	value = (30.0 * x * z) / ((x - 10.0) * y * y)
	file.write('%s,X1=%s,X2=%s,X3=%s\n' % (value, x, y, z))
file.close()
