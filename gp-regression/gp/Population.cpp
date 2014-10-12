#include <limits>
#include <cmath>

#include "Population.hpp"

const std::vector<double>& Population::evaluate(const Dataset& dataset, unsigned int dataset_id)
{
	std::map< unsigned int, std::vector<double> >::iterator it = previous_fitness.find(dataset_id);
	if (it != previous_fitness.end())
		return it->second;
	else
	{
		previous_fitness[dataset_id] = std::vector<double>();
		for (unsigned int i = 0; i < solutions.size(); ++i)
		{
			std::vector<double> fitted = solutions[i]->evaluate(dataset, dataset_id);
			double sse = 0.0;
			for (unsigned int j = 0; j < dataset.size() && j < fitted.size(); ++j)
				sse += pow(fitted[j] - dataset[j].target, 2);
			double rmse = sqrt(sse / dataset.size());

			if (std::isfinite(rmse))
				previous_fitness[dataset_id].push_back(rmse);
			else
				previous_fitness[dataset_id].push_back(std::numeric_limits<double>::max());
		}

		return previous_fitness[dataset_id];
	}
}

void Population::fix_indexes(const std::vector<unsigned int>& deleted)
{
	for (unsigned int i = 0; i < deleted.size(); ++i)
	{
		for (unsigned int j = 0; j < solutions.size(); ++j)
		{
			if (solutions[j]->index() > deleted[i])
				solutions[j]->decrease_index();
		}
	}
}
