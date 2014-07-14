import numpy as np
from sys import stdin

data = []
for line in stdin:
	instance = []
	tokens = line.strip().split(',')
	instance.append(float(tokens[0]))

	for t in tokens[1:]:
		instance.append(float(t.strip().split('=')[1]))
	data.append(instance)

matrix = np.matrix(data)
(nrow, ncol) = matrix.shape
for i in range(1, ncol):
	matrix[:,i] = (matrix[:,i] - matrix[:,i].mean()) / matrix[:,i].std()

str_data = []
for i in range(nrow):
	instance = []
	for j in range(ncol):
		 instance.append(str(matrix[i,j]))
	str_data.append(instance)

for l in str_data:
	print '\t'.join(l)
