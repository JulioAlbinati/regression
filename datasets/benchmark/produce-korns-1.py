from random import uniform

file = open('korns-1-train.data', 'w')
for i in range(10000):
	x = uniform(-50, 50)
	y = uniform(-50, 50)
	w = uniform(-50, 50)
	u = uniform(-50, 50)
	v = uniform(-50, 50)
	value = 1.57 + 24.3 * v
	file.write('%s,X1=%s,X2=%s,X3=%s,X4=%s,X5=%s\n' % (value, x, y, w, u, v))
file.close()

file = open('korns-1-test.data', 'w')
for i in range(10000):
	x = uniform(-50, 50)
	y = uniform(-50, 50)
	w = uniform(-50, 50)
	u = uniform(-50, 50)
	v = uniform(-50, 50)
	value = 1.57 + 24.3 * v
	file.write('%s,X1=%s,X2=%s,X3=%s,X4=%s,X5=%s\n' % (value, x, y, w, u, v))
file.close()
