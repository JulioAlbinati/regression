#ifndef INDIVIDUAL

#define INDIVIDUAL

#include <cmath>
#include <random>
#include <omp.h>

#include "../data/Dataset.hpp"

class Individual
{
public:
	// Constructors
	inline Individual(unsigned int variable, const Dataset& train, const Dataset& test) : num_elements(1)
	{
		unsigned int num_train = train.size();
		unsigned int num_test = test.size();
		train_values.resize(num_train);
		test_values.resize(num_test);
		for (unsigned int i = 0; i < num_train; ++i)
			train_values[i] = train[i]->attrs[variable];
		for (unsigned int i = 0; i < num_test; ++i)
			test_values[i] = test[i]->attrs[variable];
	}

	inline Individual(double value, const Dataset& train, const Dataset& test) : num_elements(1)
	{
		train_values.resize(train.size(), value);
		test_values.resize(test.size(), value);
	}

	Individual(const std::string& operation, const Individual& left, const Individual& right);

	inline void apply_sigmoid()
	{
		unsigned int num_train = train_values.size();
		unsigned int num_test = test_values.size();
		for (unsigned int i = 0; i < num_train; ++i)
			train_values[i] = 1.0 / (1.0 + exp(-1.0 * train_values[i]));
		for (unsigned int i = 0; i < num_test; ++i)
			test_values[i] = 1.0 / (1.0 + exp(-1.0 * test_values[i]));
	}

	inline unsigned int size() const { return num_elements; }

	inline const std::vector<double>& fitted_train() const { return train_values; }

	inline const std::vector<double>& fitted_test() const { return test_values; }

	static Individual build_random_individual(unsigned int max_height, bool full, unsigned int num_terminals, 
		const std::vector<std::string>& operators, std::vector<std::mt19937>& generators, const Dataset& train, const Dataset& test);

private:
	std::vector<double> train_values;
	std::vector<double> test_values;
	unsigned int num_elements;
};

#endif
