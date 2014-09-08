#include <limits>
#include <cmath>
#include<iostream>

#include "Population.hpp"

const std::vector<Statistics>& Population::evaluate(const Dataset& dataset, bool use_train)
{
	std::vector<Statistics> temp;
	temp.resize(solutions.size());

	if (use_train && previous_train_fitness.size() > 0)
		return previous_train_fitness;
	else if (!use_train && previous_test_fitness.size() > 0)
		return previous_test_fitness;
	else
	{
		//#pragma omp parallel for
		for (unsigned int i = 0; i < solutions.size(); ++i)
		{
			std::vector<double> fitted;
			if (use_train)
				fitted = solutions[i]->fitted_train();
			else
				fitted = solutions[i]->fitted_test();

			Statistics stats;
			for (unsigned int j = 0; j < dataset.size() && j < fitted.size(); ++j)
			{
				double residual = dataset[j]->target - fitted[j];
				stats.mse += pow(residual, 2);
				stats.mae += abs(residual);
				stats.total_error += abs(residual);
				if (residual > -0.01 && residual < 0.01)
					stats.hits += 1.0;
			}
			stats.mse /= ((double) dataset.size());
			stats.rmse = sqrt(stats.mse);
			stats.mae /= ((double) dataset.size());
			stats.hits /= ((double) dataset.size());

			if (!std::isfinite(stats.mse))
			{
				stats.rmse = std::numeric_limits<double>::max();
				stats.mse = std::numeric_limits<double>::max();
				stats.mae = std::numeric_limits<double>::max();
				stats.total_error = std::numeric_limits<double>::max();
				stats.hits = 0.0;
			}
			temp[i] = stats;
		}

		if (use_train)
		{
			previous_train_fitness = temp;
			return previous_train_fitness;
		}
		else
		{
			previous_test_fitness = temp;
			return previous_test_fitness;
		}
	}
}
