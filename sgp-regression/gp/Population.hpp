#ifndef POPULATION

#define POPULATION

#include "Individual.hpp"

class Statistics
{
public:
	inline Statistics() : rmse(0.0), mse(0.0), mae(0.0), total_error(0.0), hits(0.0) {}

	double rmse, mse, mae, total_error, hits;
};

class Population
{
public:
	virtual inline ~Population()
	{
		for (unsigned int i = 0; i < solutions.size(); ++i)
		{
			if (solutions[i] != NULL)
				delete solutions[i];
			solutions[i] = NULL;
		}
	}

	inline void add_solution(Individual* i)
	{
		solutions.push_back(i);
		previous_train_fitness.clear();
		previous_test_fitness.clear();
	}

	inline void remove_solution(unsigned int index)
	{
		for (std::vector<Individual*>::iterator it = solutions.begin(); it != solutions.end(); ++it)
		{
			if (index == 0)
			{
				solutions.erase(it);
				break;
			}
			--index;
		}
		previous_train_fitness.clear();
		previous_test_fitness.clear();
	}

	const std::vector<Statistics>& evaluate(const Dataset& dataset, bool use_train = true);

	inline std::pair<unsigned int, double> best(const Dataset& dataset, bool use_train = true)
	{
		std::vector<Statistics> fitness = evaluate(dataset, use_train);
		unsigned int best_index = 0;
		for (unsigned int i = 1; i < fitness.size(); ++i)
		{
			if (fitness[best_index].mse > fitness[i].mse)
				best_index = i;
		}
		return std::pair<unsigned int, double>(best_index, fitness[best_index].mse);
	}

	inline const Individual& operator[](unsigned int index)
	{
		assert(index < solutions.size());
		return *solutions[index];
	}

	inline unsigned int size() const { return solutions.size(); }

private:
	std::vector<Individual*> solutions;
	std::vector<Statistics> previous_train_fitness, previous_test_fitness;
};

#endif
