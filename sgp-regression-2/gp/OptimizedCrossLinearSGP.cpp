#include <cmath>

#include "OptimizedCrossLinearSGP.hpp"

std::pair<double, double> OptimizedCrossLinearSGP::best_coefficients(const Individual& ind1, const Individual& ind2, const Dataset& train)
{
	double f1_sq = 0.0, f2_sq = 0.0, f1f2 = 0.0, f1ft = 0.0, f2ft = 0.0;
	std::vector<double> fitted_ind1 = ind1.fitted_train();
	std::vector<double> fitted_ind2 = ind2.fitted_train();

	for (unsigned int i = 0; i < train.size(); ++i)
	{
		f1_sq += fitted_ind1[i] * fitted_ind1[i];
		f2_sq += fitted_ind2[i] * fitted_ind2[i];
		f1f2 += fitted_ind1[i] * fitted_ind2[i];
		f1ft += fitted_ind1[i] * train[i]->target;
		f2ft += fitted_ind1[i] * train[i]->target;
	}

	double beta1 = (f2_sq * f1ft - f1f2 * f2ft) / (f1_sq * f2_sq - f1f2 * f1f2);
	double beta2 = (f1_sq * f2ft - f1f2 * f1ft) / (f1_sq * f2_sq - f1f2 * f1f2);
	return std::pair<double, double>(beta1, beta2);
}

Individual* OptimizedCrossLinearSGP::crossover(const Individual& ind1, const Individual& ind2, const Dataset& train, const Dataset& test)
{
	std::pair<double, double> coeffs = best_coefficients(ind1, ind2, train);
	Individual coeff1_ind(coeffs.first, train, test);
	Individual coeff2_ind(coeffs.second, train, test);
	Individual temp1("*", coeff1_ind, ind1);
	Individual temp2("*", coeff2_ind, ind2);
	return new Individual("+", temp1, temp2);
}
