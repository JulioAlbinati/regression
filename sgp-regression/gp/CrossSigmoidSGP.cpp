#include "CrossSigmoidSGP.hpp"

Individual* CrossSigmoidSGP::crossover(const Individual& ind1, const Individual& ind2, const Dataset& train, const Dataset& test)
{
	Individual coeff = Individual::build_random_individual(max_height, false, num_terminals, operators, generators, train, test);
	coeff.apply_sigmoid();
	Individual temp(1.0, train, test);
	Individual complement("-", temp, coeff);
	Individual temp2("*", coeff, ind1);
	Individual temp3("*", complement, ind2);
	return new Individual("+", temp2, temp3);
}
