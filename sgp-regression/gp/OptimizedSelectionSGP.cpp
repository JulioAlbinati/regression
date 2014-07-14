#include <limits>
#include <cmath>

#include "OptimizedSelectionSGP.hpp"

void OptimizedSelectionSGP::iteration(const Dataset& train, const Dataset& test)
{
        Population* new_pop = new Population();

        // Elitism: adding best solution to new population
        unsigned int best_index = cur_pop->best(train).first;
        new_pop->add_solution(new Individual((*cur_pop)[best_index]));

        for (unsigned int i = 1; i < pop_size && budget > 0; ++i)
        {
                double coin = ((double) rand() / RAND_MAX);
                if (coin < mut_rate || budget < pop_size)
                {
                        std::vector<unsigned int> selected = selection(1, train);
                        new_pop->add_solution(mutation((*cur_pop)[selected[0]], train, test));
                }
                else
                {
                        std::vector<unsigned int> selected = selection(1, train);
			Individual ind2 = best_match((*cur_pop)[selected[0]], train);
                        new_pop->add_solution(crossover((*cur_pop)[selected[0]], ind2, train, test));
			budget -= pop_size;
                }
        }

        delete cur_pop;
        cur_pop = new_pop;
}

const Individual& OptimizedSelectionSGP::best_match(const Individual& ind, const Dataset& train)
{
	unsigned int best = 0;
	double best_error = std::numeric_limits<double>::max();
	for (unsigned int i = 0; i < pop_size; ++i)
	{
		double coeff = best_coefficient(ind, (*cur_pop)[i], train);
		double sse = 0.0;
		std::vector<double> fitted1 = ind.fitted_train();
		std::vector<double> fitted2 = (*cur_pop)[i].fitted_train();
		for (unsigned int j = 0; j < train.size(); ++j)
			sse += pow(train[j]->target - coeff * fitted1[j] - (1 - coeff) * fitted2[j], 2);
		if (sse < best_error)
		{
			best_error = sse;
			best = i;
		}
	}

	return (*cur_pop)[best];
}
