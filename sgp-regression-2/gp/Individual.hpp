#ifndef INDIVIDUAL

#define INDIVIDUAL

#include <cmath>

#include "../data/Dataset.hpp"
#include "../graph/DAG.hpp"

class Individual
{
public:
	// Constructors
	inline Individual(unsigned int variable, const Dataset& train, const Dataset& test, DAG& graph) : num_elements(1)
	{
		unsigned int num_train = train.size();
		unsigned int num_test = test.size();
		train_values.resize(num_train);
		test_values.resize(num_test);
		for (unsigned int i = 0; i < num_train; ++i)
			train_values[i] = train[i]->attrs[variable];
		for (unsigned int i = 0; i < num_test; ++i)
			test_values[i] = test[i]->attrs[variable];

		index = graph.add_terminal_node(variable);
	}

	inline Individual(double value, const Dataset& train, const Dataset& test, DAG& graph) : num_elements(1)
	{
		train_values.resize(train.size(), value);
		test_values.resize(test.size(), value);
		index = graph.add_terminal_node(value);
	}

	Individual(const std::string& operation, const Individual& left, const Individual& right, DAG& graph);

	inline void apply_sigmoid(DAG& graph)
	{
		unsigned int num_train = train_values.size();
		unsigned int num_test = test_values.size();
		for (unsigned int i = 0; i < num_train; ++i)
			train_values[i] = 1.0 / (1.0 + exp(-1.0 * train_values[i]));
		for (unsigned int i = 0; i < num_test; ++i)
			test_values[i] = 1.0 / (1.0 + exp(-1.0 * test_values[i]));

		unsigned int temp1 = graph.add_terminal_node(-1.0);
		unsigned int temp2 = graph.add_intermediate_node("*", temp1, index);
		unsigned int temp3 = graph.add_intermediate_node("exp", temp2);
		unsigned int temp4 = graph.add_terminal_node(1.0);
		unsigned int temp5 = graph.add_intermediate_node("+", temp3, temp4);
		index = graph.add_intermediate_node("/", temp4, temp5);
	}

	inline unsigned int size() const { return num_elements; }

	inline const std::vector<double>& fitted_train() const { return train_values; }

	inline const std::vector<double>& fitted_test() const { return test_values; }

	inline unsigned int get_index() const { return index; }

	static Individual build_random_individual(unsigned int max_height, bool full, unsigned int num_terminals, 
		const std::vector<std::string>& operators, const Dataset& train, const Dataset& test, DAG& graph);

private:
	unsigned int num_elements, index;
	std::vector<double> train_values;
	std::vector<double> test_values;
};

#endif
