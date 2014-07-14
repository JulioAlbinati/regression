#ifndef OPTIMIZED_CROSS_LINEAR_SGP
#define OPTIMIZED_CROSS_LINEAR_SGP

#include "OptimizedCrossSGP.hpp"

class OptimizedCrossLinearSGP : public OptimizedCrossSGP
{
public:
	inline OptimizedCrossLinearSGP(unsigned int pop_size, unsigned int num_iter, unsigned int max_height, unsigned int mut_height, unsigned int tour_size, 
		  double mut_rate, double mut_step, short mode_init, unsigned int num_terminals, const std::vector<std::string>& operators) : 
		  OptimizedCrossSGP(pop_size, num_iter, max_height, mut_height, tour_size, mut_rate, mut_step, mode_init, num_terminals, operators) {}
protected:
	virtual std::pair<double, double> best_coefficients(const Individual& ind1, const Individual& ind2, const Dataset& train);
	virtual Individual* crossover(const Individual& ind1, const Individual& ind2, const Dataset& train, const Dataset& test);
};

#endif
