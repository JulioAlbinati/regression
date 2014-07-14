#include <iostream>
#include <cstdlib>
#include <ctime>

#include "DAG.hpp"

using namespace std;

unsigned int create_random_expr(unsigned int height, const unsigned int num_attrs, const std::vector<std::string>& operators, DAG& graph)
{
	if (height > 1)
	{
		unsigned int index_op = rand() % operators.size();
		unsigned int left = create_random_expr(height - 1, num_attrs, operators, graph);
		unsigned int right = create_random_expr(height - 1, num_attrs, operators, graph);
		return graph.add_intermediate_node(operators[index_op], left, right);
	}
	else
	{
		double coin = ((double) rand()) / ((double) RAND_MAX);
		if (coin < 0.5)
			return graph.add_terminal_node(rand() % num_attrs);
		else
			return graph.get_index_constant();
	}
}

int main()
{
	std::vector<std::string> operators = {"+", "-", "*", "/"};
	DAG graph(operators);
	srand(time(NULL));

	vector<unsigned int> population;
	for (unsigned int i = 0; i < 1000; ++i)
		population.push_back(create_random_expr(6, 100, operators, graph));

	for (unsigned int i = 0; i < 10; ++i)
	{
		cout << "Generation " << i + 1 << ": " << graph.size() << " nodes..." << endl;
		vector<unsigned int> new_pop;
		for (unsigned int j = 0; j < 1000; ++j)
		{
			double coin = ((double) rand()) / ((double) RAND_MAX);
			if (coin < 0.5)
			{
				unsigned int index1 = population[rand() % 1000];
				unsigned int index2 = population[rand() % 1000];
				unsigned int temp1 = graph.add_intermediate_node("*", graph.get_index_constant(), index1);
				unsigned int temp2 = graph.add_intermediate_node("*", graph.get_index_constant(), index2);
				new_pop.push_back(graph.add_intermediate_node("+", temp1, temp2));
			}
			else
			{
				unsigned int index = population[rand() % 1000];
				unsigned int random1 = create_random_expr(6, 100, operators, graph);
				unsigned int random2 = create_random_expr(6, 100, operators, graph);
				unsigned int temp1 = graph.add_intermediate_node("-", random1, random2);
				unsigned int temp2 = graph.add_intermediate_node("*", graph.get_index_constant(), temp1);
				new_pop.push_back(graph.add_intermediate_node("+", index, temp2));
			}
		}
		population = new_pop;
	}

	cout << "size = " << graph.size() << endl;

	cout << graph.print_expression(population[0]) << endl;

	return EXIT_SUCCESS;
}
