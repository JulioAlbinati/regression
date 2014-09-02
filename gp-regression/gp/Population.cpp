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
		double mean_real = 0.0;
		for (unsigned int i = 0; i < dataset.size(); ++i)
			mean_real += (dataset[i].target / ((double) dataset.size()));

		previous_fitness[dataset_id] = std::vector<double>();
		for (unsigned int i = 0; i < solutions.size(); ++i)
		{
			std::vector<double> fitted = solutions[i]->evaluate(dataset, dataset_id);
			double mean_fitted = 0.0;
			for (unsigned int j = 0; j < dataset.size() && j < fitted.size(); ++j)
				mean_fitted += (fitted[j] / ((double) fitted.size()));


			double numerator = 0.0, dev_real = 0.0, dev_fitted = 0.0;
			for (unsigned int j = 0; j < dataset.size() && j < fitted.size(); ++j)
			{
				numerator += (dataset[j].target - mean_real) * (fitted[j] - mean_fitted);
				dev_real += pow(dataset[j].target - mean_real, 2);
				dev_fitted += pow(fitted[j] - mean_fitted, 2);
			}
			double r = numerator / (sqrt(dev_real * dev_fitted));
			if (std::isfinite(r))
				previous_fitness[dataset_id].push_back(r);
			else
				previous_fitness[dataset_id].push_back(0);
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
