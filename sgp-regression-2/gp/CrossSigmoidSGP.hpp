#ifndef SIGMOID_SGP

#define SIGMOID_SGP

#include "SGP.hpp"

class CrossSigmoidSGP : public SGP
{
public:
	inline CrossSigmoidSGP(unsigned int pop_size, unsigned int num_iter, unsigned int max_height, unsigned int mut_height, unsigned int tour_size, 
		  double mut_rate, double mut_step, short mode_init, unsigned int num_terminals, const std::vector<std::string>& operators) : SGP(pop_size, 
		  num_iter, max_height, mut_height, tour_size, mut_rate, mut_step, mode_init, num_terminals, operators) {}
private:
	virtual Individual* crossover(const Individual& ind1, const Individual& ind2, const Dataset& train, const Dataset& test);
};

#endif
