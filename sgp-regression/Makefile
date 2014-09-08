CFLAGS=-Wall -Wextra -g --std=c++0x -O3 -fopenmp

all: sgp sigmoid-cross opt-cross opt-linear

framework:
	g++ ${CFLAGS} -c data/Dataset.cpp -o data/Dataset.o
	g++ ${CFLAGS} -c gp/Parameters.cpp -o gp/Parameters.o
	g++ ${CFLAGS} -c gp/Individual.cpp -o gp/Individual.o
	g++ ${CFLAGS} -c gp/Population.cpp -o gp/Population.o
	g++ ${CFLAGS} -c gp/SGP.cpp -o gp/SGP.o

sgp: framework
	g++ ${CFLAGS} -c main-sgp.cpp -o main-sgp.o
	g++ ${CFLAGS} data/Dataset.o gp/Parameters.o gp/Individual.o gp/Population.o gp/SGP.o main-sgp.o -o sgp

sigmoid-cross: framework
	g++ ${CFLAGS} -c gp/CrossSigmoidSGP.cpp -o gp/CrossSigmoidSGP.o
	g++ ${CFLAGS} -c main-sigmoid-cross-sgp.cpp -o main-cross-sigmoid-sgp.o
	g++ ${CFLAGS} data/Dataset.o gp/Parameters.o gp/Individual.o gp/Population.o gp/SGP.o gp/CrossSigmoidSGP.o main-cross-sigmoid-sgp.o -o cross-sigmoid-sgp

framework-opt-cross: framework
	g++ ${CFLAGS} -c gp/OptimizedCrossSGP.cpp -o gp/OptimizedSGP.o

opt-cross: framework-opt-cross
	g++ ${CFLAGS} -c main-opt-cross-sgp.cpp -o main-opt-cross-sgp.o
	g++ ${CFLAGS} data/Dataset.o gp/Parameters.o gp/Individual.o gp/Population.o gp/SGP.o gp/OptimizedSGP.o main-opt-cross-sgp.o -o opt-cross-sgp

opt-linear: framework-opt-cross
	g++ ${CFLAGS} -c gp/OptimizedCrossLinearSGP.cpp -o gp/OptimizedCrossLinearSGP.o
	g++ ${CFLAGS} -c main-opt-cross-linear-sgp.cpp -o main-opt-cross-linear-sgp.o
	g++ ${CFLAGS} data/Dataset.o gp/Parameters.o gp/Individual.o gp/Population.o gp/SGP.o gp/OptimizedSGP.o gp/OptimizedCrossLinearSGP.o main-opt-cross-linear-sgp.o -o opt-cross-linear-sgp

clean:
	rm gp/*.o data/*.o *.o sgp cross-sigmoid-sgp opt-cross-sgp opt-cross-linear-sgp
