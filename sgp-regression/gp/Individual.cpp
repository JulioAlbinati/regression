#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>

#include "Individual.hpp"

Individual::Individual(const std::string& operation, const Individual& left, const Individual& right) : num_elements(left.size() + right.size() + 1)
{
	std::vector<double> train_left = left.fitted_train();
	std::vector<double> test_left = left.fitted_test();
	std::vector<double> train_right = right.fitted_train();
	std::vector<double> test_right = right.fitted_test();

	unsigned int num_train_left = train_left.size();
	unsigned int num_train_right = train_right.size();
	unsigned int num_test_left = test_left.size();
	unsigned int num_test_right = test_right.size();

	if (num_train_left > num_train_right)
		train_values.resize(num_train_right);
	else
		train_values.resize(num_train_left);
	if (num_test_left > num_test_right)
		test_values.resize(num_test_right);
	else
		test_values.resize(num_test_left);

	if (operation == "+")
	{
		for (unsigned int i = 0; i < num_train_left && i < num_train_right; ++i)
			train_values[i] = train_left[i] + train_right[i];
		for (unsigned int i = 0; i < num_test_left && i < num_test_right; ++i)
			test_values[i] = test_left[i] + test_right[i];
	}
	else if (operation == "-")
	{
		for (unsigned int i = 0; i < num_train_left && i < num_train_right; ++i)
			train_values[i] = train_left[i] - train_right[i];
		for (unsigned int i = 0; i < num_test_left && i < num_test_right; ++i)
			test_values[i] = test_left[i] - test_right[i];
	}
	else if (operation == "*")
	{
		for (unsigned int i = 0; i < num_train_left && i < num_train_right; ++i)
			train_values[i] = train_left[i] * train_right[i];
		for (unsigned int i = 0; i < num_test_left && i < num_test_right; ++i)
			test_values[i] = test_left[i] * test_right[i];
	}
	else
	{
		for (unsigned int i = 0; i < num_train_left && i < num_train_right; ++i)
		{
			if (train_right[i] > 10e-6 || train_right[i] < -10e-6)
				train_values[i] = train_left[i] / train_right[i];
			else
				train_values[i] = 0.0;
		}
		for (unsigned int i = 0; i < num_test_left && i < num_test_right; ++i)
		{
			if (test_right[i] > 10e-6 || test_right[i] < -10e-6)
				test_values[i] = test_left[i] / test_right[i];
			else
				test_values[i] = 0.0;
		}
	}
}

Individual Individual::build_random_individual(unsigned int max_height, bool full, unsigned int num_terminals, 
	const std::vector<std::string>& operators, const Dataset& train, const Dataset& test)
{
	std::vector< std::vector< std::pair<unsigned int, short> > > tree_matrix;
	if (full)
	{
		unsigned int nodes_level = 1;
		// Generating only operators...
		for (unsigned int i = 0; i < max_height - 1; ++i)
		{
			tree_matrix.push_back(std::vector<std::pair<unsigned int, short> >());
			for (unsigned int j = 0; j < nodes_level; ++j)
			{
				unsigned int index = rand() % operators.size();
				tree_matrix[i].push_back(std::pair<unsigned int, short>(index, 0));
			}
			nodes_level *= 2;
		}
		// Generating terminal nodes...
		tree_matrix.push_back(std::vector<std::pair<unsigned int, short> >());
		for (unsigned int i = 0; i < nodes_level; ++i)
		{
			double coin = ((double) rand()) / RAND_MAX;
			if (coin < 0.5)
			{
				unsigned int index = rand() % num_terminals;
				tree_matrix[max_height - 1].push_back(std::pair<unsigned int, short>(index, 1));
			}
			else
				tree_matrix[max_height - 1].push_back(std::pair<unsigned int, short>(0, 2));
		}
	}
	else
	{
		unsigned int nodes_level = 1;
		for (unsigned int i = 0; i < max_height; ++i)
		{
			unsigned int next_level = 0;
			tree_matrix.push_back(std::vector<std::pair<unsigned int, short> >());
			for (unsigned int j = 0; j < nodes_level; ++j)
			{
				double coin = ((double) rand()) / RAND_MAX;
				if (coin < 0.9 && i < max_height - 1)
				{
					unsigned int index = rand() % operators.size();
					tree_matrix[i].push_back(std::pair<unsigned int, short>(index, 0));
					next_level += 2;
				}
				else
				{
					coin = ((double) rand()) / RAND_MAX;
					if (coin < 0.5)
					{
						unsigned int index = rand() % num_terminals;
						tree_matrix[i].push_back(std::pair<unsigned int, short>(index, 1));
					}
					else
						tree_matrix[i].push_back(std::pair<unsigned int, short>(0, 2));
				}
			}
			nodes_level = next_level;
		}
	}

	std::vector<Individual> roots;
	for (int i = tree_matrix.size() - 1; i >= 0; --i)
	{
		std::vector<Individual> new_roots;
		unsigned int cur_index = 0;
		for (unsigned int j = 0; j < tree_matrix[i].size(); ++j)
		{
			if (tree_matrix[i][j].second == 0)	
			{
				new_roots.push_back(Individual(operators[tree_matrix[i][j].first], roots[cur_index], roots[cur_index + 1]));
				cur_index += 2;
			}
			else if (tree_matrix[i][j].second == 1)
				new_roots.push_back(Individual(tree_matrix[i][j].first, train, test));
			else
			{
				double value = ((double) rand()) / (RAND_MAX / 2) - 1.0;
				new_roots.push_back(Individual(value, train, test));
			}
		}
		roots = new_roots;
	}

	return roots[0];
}
