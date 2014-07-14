file = open('keijzer-6-train.data', 'w')
for i in range(1, 51):
	value = 0.0
	for j in range(1, i + 1):
		value += 1.0 / float(j)
	file.write('%s,X1=%s\n' % (value, i))
file.close()
file = open('keijzer-6-test.data', 'w')
for i in range(1, 121):
	value = 0.0
	for j in range(1, i + 1):
		value += 1.0 / float(j)
	file.write('%s,X1=%s\n' % (value, i))
file.close()
