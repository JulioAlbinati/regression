#ifndef POPULATION

#define POPULATION

#include "Individual.hpp"

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

	inline void add_solution(Individual* i) { solutions.push_back(i); }

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
	}

	const std::vector<double>& evaluate(const Dataset& dataset, unsigned int dataset_id);

	inline std::pair<unsigned int, double> best(const Dataset& dataset, unsigned int dataset_id)
	{
		std::vector<double> fitness = evaluate(dataset, dataset_id);
		unsigned int best_index = 0;
		for (unsigned int i = 1; i < fitness.size(); ++i)
		{
			if (fitness[best_index] > fitness[i])
				best_index = i;
		}
		return std::pair<unsigned int, double>(best_index, fitness[best_index]);
	}

	void fix_indexes(const std::vector<unsigned int>& deleted);

	inline Individual* operator[](unsigned int index)
	{
		if (index < solutions.size())
			return solutions[index];

		return NULL;
	}

	inline unsigned int size() const { return solutions.size(); }

private:
	std::vector<Individual*> solutions;
	std::map< unsigned int, std::vector<double> > previous_fitness;
};

#endif
