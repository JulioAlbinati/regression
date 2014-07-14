#include <cstdlib>

#include "SGP.hpp"

void SGP::iteration(const Dataset& train, const Dataset& test)
{
	Population* new_pop = new Population();

	// Elitism: adding best solution to new population
	unsigned int best_index = cur_pop->best(train).first;
	new_pop->add_solution(new Individual((*cur_pop)[best_index]));

	for (unsigned int i = 1; i < pop_size; ++i)
	{
		double coin = ((double) rand() / RAND_MAX);
		if (coin < mut_rate)
		{
			std::vector<unsigned int> selected = selection(1, train);
			new_pop->add_solution(mutation((*cur_pop)[selected[0]], train, test));
		}
		else
		{
			std::vector<unsigned int> selected = selection(2, train);
			new_pop->add_solution(crossover((*cur_pop)[selected[0]], (*cur_pop)[selected[1]], train, test));
		}
	}

	delete cur_pop;
	cur_pop = new_pop;
}

void SGP::initialize_pop(const Dataset& train, const Dataset& test)
{
	unsigned int num_full = 0;
	unsigned int num_grow = 0;
	if (mode_init == 1)
		num_full = pop_size;
	else if (mode_init == 2)
		num_grow = pop_size;
	else
	{
		num_full = pop_size / 2;
		num_grow = pop_size - num_full;
	}

	for (unsigned int i = 0; i < num_full; ++i)
		cur_pop->add_solution(new Individual(Individual::build_random_individual(max_height, true, num_terminals, operators, train, test)));
	for (unsigned int i = 0; i < num_grow; ++i)
		cur_pop->add_solution(new Individual(Individual::build_random_individual(max_height, false, num_terminals, operators, train, test)));
}

std::vector<unsigned int> SGP::selection(unsigned int num, const Dataset& train)
{
	std::vector<Statistics> fitness = cur_pop->evaluate(train);
	std::vector<unsigned int> selected;

	for (unsigned int i = 0; i < num; ++i)
	{
		unsigned int best = rand() % fitness.size();
		for (unsigned int j = 1; j < tour_size; ++j)
		{
			unsigned int index = rand() % fitness.size();
			if (fitness[index].mse < fitness[best].mse)
				best = index;
		}
		selected.push_back(best);
	}
	return selected;
}

Individual* SGP::mutation(const Individual& original, const Dataset& train, const Dataset& test)
{
	Individual random_ind1 = Individual::build_random_individual(mut_height, false, num_terminals, operators, train, test);
        Individual random_ind2 = Individual::build_random_individual(mut_height, false, num_terminals, operators, train, test);
	random_ind1.apply_sigmoid();
	random_ind2.apply_sigmoid();
        Individual temp1("-", random_ind1, random_ind2);
	Individual ms(mut_step, train, test);
	Individual temp2("*", ms, temp1);
	return new Individual("+", temp2, original);
}

Individual* SGP::crossover(const Individual& ind1, const Individual& ind2, const Dataset& train, const Dataset& test)
{
	double coeff = ((double) rand()) / RAND_MAX;
	Individual ind_coeff(coeff, train, test);
	Individual ind_compl(1.0 - coeff, train, test);
	Individual temp1("*", ind1, ind_coeff);
	Individual temp2("*", ind2, ind_compl);
	return new Individual("+", temp1, temp2);
}
