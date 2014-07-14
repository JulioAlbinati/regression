#ifndef SGP_CLASS

#define SGP_CLASS

#include "GP.hpp"

class SemanticGP : public GP
{
public:
	inline SemanticGP(unsigned int pop_size, unsigned int num_iter, unsigned int max_height, unsigned int tour_size, double mut_rate, double mut_step, 
			  short mode_init, const std::vector<std::string>& terminals, const std::vector<std::string>& operators) : GP(pop_size, num_iter, 
			  max_height, tour_size, mut_rate, mode_init, terminals, operators), mut_step(mut_step) {}
private:
	double mut_step;

	Individual* mutation(Individual* individual, Digraph& digraph);
	Individual* crossover(Individual* ind1, Individual* ind2, Digraph& digraph);
};

#endif
