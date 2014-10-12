CFLAGS=-Wall -Wextra -O3 -g --std=c++0x -fopenmp

all: std-gp sgp

framework:
	g++ ${CFLAGS} -c data/Dataset.cpp -o data/Dataset.o
	g++ ${CFLAGS} -c graph/Digraph.cpp -o graph/Digraph.o
	g++ ${CFLAGS} -c gp/Parameters.cpp -o gp/Parameters.o
	g++ ${CFLAGS} -c gp/Individual.cpp -o gp/Individual.o
	g++ ${CFLAGS} -c gp/Population.cpp -o gp/Population.o
	g++ ${CFLAGS} -c gp/GP.cpp -o gp/GP.o

std-gp: framework
	g++ ${CFLAGS} -c main-std-gp.cpp -o main-std-gp.o
	g++ ${CFLAGS} data/Dataset.o graph/Digraph.o gp/Parameters.o gp/Individual.o gp/Population.o gp/GP.o main-std-gp.o -o std-gp

clean:
	rm *.o data/*.o graph/*.o gp/*.o std-gp
