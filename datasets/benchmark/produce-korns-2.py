from random import uniform

file = open('korns-2-train.data', 'w')
for i in range(1, 10001):
	x = uniform(-50, 50)
	y = uniform(-50, 50)
	w = uniform(-50, 50)
	u = uniform(-50, 50)
	v = uniform(-50, 50)
	value = 0.23 + 14.2 * ((v + y) / (3 * w))
	file.write('%s,X1=%s,X2=%s,X3=%s,X4=%s,X5=%s\n' % (value, x, y, w, u, v))
file.close()

file = open('korns-2-test.data', 'w')
for i in range(1, 10001):
	x = uniform(-50, 50)
	y = uniform(-50, 50)
	w = uniform(-50, 50)
	u = uniform(-50, 50)
	v = uniform(-50, 50)
	value = 0.23 + 14.2 * ((v + y) / (3 * w))
	file.write('%s,X1=%s,X2=%s,X3=%s,X4=%s,X5=%s\n' % (value, x, y, w, u, v))
file.close()
