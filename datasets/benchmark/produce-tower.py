from random import randint

file = open('tower.data')
lines = file.readlines()
new_lines = []
for l in lines[1:]:
	tokens = l.strip().split('\t')
	new_line = '%s' % tokens[-1]
	for i in range(0, len(tokens) - 1):
		new_line += ',X%s=%s' % (i + 1, tokens[i])
	new_lines.append(new_line)
file.close()

num_test = len(new_lines) / 2
file = open('tower-test.data', 'w')
for i in range(0, num_test):
	index = randint(0, len(new_lines) - 1)
	file.write('%s\n' % new_lines[index])
	new_lines.remove(new_lines[index])
file.close()

file = open('tower-train.data', 'w')
for l in new_lines:
	file.write('%s\n' % l)
file.close()
