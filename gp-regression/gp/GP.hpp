#ifndef GP_CLASS

#define GP_CLASS

#include <iostream>

#include "Population.hpp"

class GP
{
public:
	inline GP(unsigned int pop_size, unsigned int num_iter, unsigned int max_height, unsigned int tour_size, double mut_rate, short mode_init, 
		  const std::vector<std::string>& terminals, const std::vector<std::string>& operators) : pop_size(pop_size), num_iter(num_iter), 
		  max_height(max_height), tour_size(tour_size), mut_rate(mut_rate), mode_init(mode_init), terminals(terminals), operators(operators)
	{
		cur_pop = new Population();
	}

	inline virtual ~GP() {}

	inline virtual std::string run(const Dataset& train, const Dataset& test)
	{
		srand(time(NULL));
		Digraph graph;
		initialize_pop(graph);

		cur_pop->evaluate(train, 0);
		cur_pop->evaluate(test, 1);
		
		std::pair<unsigned int, double> best_at_train;
		for (unsigned int i = 0; i < num_iter; ++i)
		{
			std::cout << "--- ITERATION " << i + 1 << " ---" << std::endl;
			iteration(train, graph);

			best_at_train = cur_pop->best(train, 0);
			std::cout << "Best (train): " << best_at_train.second << std::endl;
			std::vector<double> fitness_test = cur_pop->evaluate(test, 1);
			std::cout << "Best (test): " << fitness_test[best_at_train.first] << std::endl;

			cur_pop->fix_indexes(graph.remove_unused());
			std::cout << "#vertices = " << graph.size() << std::endl;
		}

		unsigned int count = 0;
		std::string expr = graph.subgraph_as_string((*cur_pop)[best_at_train.first]->index(), (*cur_pop)[best_at_train.first]->get_values(), count);
		if (cur_pop != NULL)
			delete cur_pop;
		graph.remove_unused();
		return expr;
	}

protected:
	unsigned int pop_size, num_iter, max_height, tour_size;
	double mut_rate;
	short mode_init;
	std::vector<std::string> terminals, operators;
	Population* cur_pop;

	virtual void iteration(const Dataset& train, Digraph& graph);
	virtual void initialize_pop(Digraph& graph);
	virtual std::vector<unsigned int> selection(unsigned int num, const Dataset& train);	
	virtual Individual* mutation(Individual* individual, Digraph& digraph);
	virtual Individual* crossover(Individual* ind1, Individual* ind2, Digraph& digraph);
};

#endif
