from random import uniform

def f1(x):
	return 0.2519800792-0.9792427276*x-0.08559907454*x*x-0.7792666275*x*x*x

def f2(x):
	return -0.2999243600-0.2017535612*x-0.8706542684*x*x-0.8289883533*x*x*x+0.4337792992*x*x*x*x

def f3(x):
	return 0.2213736912-0.6921094145*x-0.6830066931*x*x+0.7290474026*x*x*x+0.3850480204*x*x*x*x+0.7333559613*x*x*x*x*x

def f4(x):
	return 0.6930892711+0.3515364260*x-0.7473842819*x*x-0.1173738488*x*x*x+0.02003409362*x*x*x*x+0.9743861711*x*x*x*x*x-0.4599036656*x*x*x*x*x*x

def f5(x):
	return 0.4215743020-0.7074997282*x+0.9160306846*x*x+0.8298825445*x*x*x+0.1599837874*x*x*x*x+0.6295237744*x*x*x*x*x+0.1317452890*x*x*x*x*x*x+0.5242756165*x*x*x*x*x*x*x

def f6(x):
	return -0.2210679311+0.06864118219*x-0.8088444464*x*x+0.3240047233*x*x*x+0.5983865068*x*x*x*x-0.2103330551*x*x*x*x*x-0.7376628578*x*x*x*x*x*x+0.7763968764*x*x*x*x*x*x*x-0.03938125549*x*x*x*x*x*x*x*x

def f7(x):
	return 0.3954424801+0.3981203889*x+0.1891832775*x*x+0.1074583774*x*x*x+0.2190329232*x*x*x*x-0.4576566444*x*x*x*x*x-0.5380782443*x*x*x*x*x*x+0.7836101544*x*x*x*x*x*x*x+0.4840406694*x*x*x*x*x*x*x*x*x+0.6928746592*x*x*x*x*x*x*x*x*x

def f8(x):
	return -0.2840700615-0.5869436745*x+0.3871034866*x*x-0.5939383375*x*x*x+0.5833673754*x*x*x*x+0.5576582896*x*x*x*x*x-0.7038581729*x*x*x*x*x*x-0.2879518820*x*x*x*x*x*x*x-0.7331485233*x*x*x*x*x*x*x*x+0.9815689412*x*x*x*x*x*x*x*x*x-0.3457738083*x*x*x*x*x*x*x*x*x*x

def generate_file(filename, function, size):
	file = open(filename, 'w')
	for i in range(0, size):
		value = uniform(-1.0, 1.0)
		file.write('%s,X1=%s\n' % (function(value), value))

if __name__ == '__main__':
	generate_file('f1-train.data', f1, 20)
	generate_file('f2-train.data', f2, 20)
	generate_file('f3-train.data', f3, 20)
	generate_file('f4-train.data', f4, 20)
	generate_file('f5-train.data', f5, 20)
	generate_file('f6-train.data', f6, 20)
	generate_file('f7-train.data', f7, 20)
	generate_file('f8-train.data', f8, 20)
	generate_file('f1-test.data', f1, 20)
	generate_file('f2-test.data', f2, 20)
	generate_file('f3-test.data', f3, 20)
	generate_file('f4-test.data', f4, 20)
	generate_file('f5-test.data', f5, 20)
	generate_file('f6-test.data', f6, 20)
	generate_file('f7-test.data', f7, 20)
	generate_file('f8-test.data', f8, 20)
