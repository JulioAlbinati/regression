#ifndef SGP_CLASS

#define SGP_CLASS

#include <iostream>
#include <cstdlib>

#include "Population.hpp"

class SGP
{
public:
	inline SGP(unsigned int pop_size, unsigned int num_iter, unsigned int max_height, unsigned int mut_height, unsigned int tour_size, 
		  double mut_rate, double mut_step, short mode_init, unsigned int num_terminals, const std::vector<std::string>& operators) : 
		  pop_size(pop_size), num_iter(num_iter), max_height(max_height), mut_height(mut_height), tour_size(tour_size), mut_rate(mut_rate), 
		  mut_step(mut_step), mode_init(mode_init), num_terminals(num_terminals), operators(operators)
	{
		cur_pop = new Population();
		graph = new DAG(operators);
	}

	inline virtual ~SGP()
	{
		if (cur_pop != NULL)
			delete cur_pop;
		if (graph != NULL)
			delete graph;
	}

	inline virtual std::string run(const Dataset& train, const Dataset& test)
	{
		srand(time(NULL));
		initialize_pop(train, test);
		
		std::pair<unsigned int, double> best_at_train;
		for (unsigned int i = 0; i < num_iter; ++i)
		{
			std::cout << "--- ITERATION " << i + 1 << " ---" << std::endl;
			iteration(train, test);

			best_at_train = cur_pop->best(train);
			std::vector<Statistics> fitness_train = cur_pop->evaluate(train);
			std::cout << "RMSE (train): " << fitness_train[best_at_train.first].rmse << std::endl;
			std::cout << "MSE (train): " << fitness_train[best_at_train.first].mse << std::endl;
			std::cout << "MAE (train): " << fitness_train[best_at_train.first].mae << std::endl;
			std::cout << "Total error (train): " << fitness_train[best_at_train.first].total_error << std::endl;

			std::vector<Statistics> fitness_test = cur_pop->evaluate(test, false);
			std::cout << "RMSE (test): " << fitness_test[best_at_train.first].rmse << std::endl;
			std::cout << "MSE (test): " << fitness_test[best_at_train.first].mse << std::endl;
			std::cout << "MAE (test): " << fitness_test[best_at_train.first].mae << std::endl;
			std::cout << "Total error (test): " << fitness_test[best_at_train.first].total_error << std::endl;

			std::cout << "Size: " << (*cur_pop)[best_at_train.first].size() << std::endl;
		}

		return graph->print_expression((*cur_pop)[best_at_train.first].get_index());
	}

protected:
	unsigned int pop_size, num_iter, max_height, mut_height, tour_size;
	double mut_rate, mut_step;
	short mode_init;
	unsigned int num_terminals;
	std::vector<std::string> operators;
	Population* cur_pop;
	DAG* graph;

	virtual void iteration(const Dataset& train, const Dataset& test);
	virtual void initialize_pop(const Dataset& train, const Dataset& test);
	virtual std::vector<unsigned int> selection(unsigned int num, const Dataset& train);

	// Operators
	virtual Individual* mutation(const Individual& individual, const Dataset& train, const Dataset& test);
	virtual Individual* crossover(const Individual& ind1, const Individual& ind2, const Dataset& train, const Dataset& test);
};

#endif
