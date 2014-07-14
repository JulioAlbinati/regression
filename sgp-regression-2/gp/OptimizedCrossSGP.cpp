#include <cmath>

#include "OptimizedCrossSGP.hpp"

double OptimizedCrossSGP::best_coefficient(const Individual& ind1, const Individual& ind2, const Dataset& train)
{
	double numerator = 0.0;
	double denominator = 0.0;

	std::vector<double> fitted_ind1 = ind1.fitted_train();
	std::vector<double> fitted_ind2 = ind2.fitted_train();
	unsigned int size = fitted_ind1.size() < fitted_ind2.size() ? fitted_ind1.size() : fitted_ind2.size();

	for (unsigned int i = 0; i < size; ++i)
	{
		numerator += (train[i]->target - fitted_ind2[i]) * (fitted_ind1[i] - fitted_ind2[i]);
		denominator += pow(fitted_ind1[i] - fitted_ind2[i], 2);
	}
	double coeff = numerator / denominator;

	if (coeff > 1.0)
		coeff = 1.0;
	else if (coeff < 0.0)
		coeff = 0.0;

	return coeff;
}

Individual* OptimizedCrossSGP::crossover(const Individual& ind1, const Individual& ind2, const Dataset& train, const Dataset& test)
{
	double coeff = best_coefficient(ind1, ind2, train);
	Individual coeff_ind(coeff, train, test);
	Individual compl_ind(1.0 - coeff, train, test);
	Individual temp1("*", coeff_ind, ind1);
	Individual temp2("*", compl_ind, ind2);
	return new Individual("+", temp1, temp2);
}
