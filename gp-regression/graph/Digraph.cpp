#include <iostream>

#include "Digraph.hpp"

std::vector<double> Digraph::evaluate(unsigned int index, const Dataset& dataset, const std::vector<double>& values, 
	unsigned int& cur_index) const
{
	assert(index < size());

	if (!vertices[index]->is_terminal())
	{
		IntermediateVertex* vertex = (IntermediateVertex*) vertices[index];
		std::vector<double> left_results = evaluate(adj_lists[index].first, dataset, values, cur_index);
		std::vector<double> right_results = evaluate(adj_lists[index].second, dataset, values, cur_index);
		std::vector<double> final_results;

		if (vertex->operation == "+")
		{
			for (unsigned int i = 0; i < left_results.size() && i < right_results.size(); ++i)
				final_results.push_back(left_results[i] + right_results[i]);
		}
		if (vertex->operation == "-")
		{
			for (unsigned int i = 0; i < left_results.size() && i < right_results.size(); ++i)
				final_results.push_back(left_results[i] - right_results[i]);
		}
		if (vertex->operation == "*")
		{
			for (unsigned int i = 0; i < left_results.size() && i < right_results.size(); ++i)
				final_results.push_back(left_results[i] * right_results[i]);
		}
		if (vertex->operation == "/")
		{
			for (unsigned int i = 0; i < left_results.size() && i < right_results.size(); ++i)
			{
				if (right_results[i] > 0.000001 || right_results[i] < -0.000001)
					final_results.push_back(left_results[i] / right_results[i]);
				else
					final_results.push_back(0.0);
			}
		}

		return final_results;
	}
	else
	{
		TerminalVertex* vertex = (TerminalVertex*) vertices[index];
		if (vertex->is_constant())
		{
			std::vector<double> to_return(dataset.size(), values[cur_index]);
			++cur_index;
			return to_return;
		}
		else
		{
			VariableVertex* var_vertex = (VariableVertex*) vertex;
			std::vector<double> results;
			for (unsigned int i = 0; i < dataset.size(); ++i)
				results.push_back(dataset[i].get(var_vertex->variable));
			return results;
		}
	}
}

std::vector<unsigned int> Digraph::remove_unused()
{
	std::vector<unsigned int> deleted;
	for (unsigned int i = 1; i < vertices.size(); ++i)
	{
		if (vertices[i]->get_ref_count() <= 0)
		{
			deleted.push_back(i);
			for (unsigned int j = 0; j < vertices.size(); ++j)
			{
				if (adj_lists[j].first > i)
					--adj_lists[j].first;
				if (adj_lists[j].second > i)
					--adj_lists[j].second;
			}

			std::vector<Vertex*>::iterator it_vert = vertices.begin();
			std::vector< std::pair<unsigned int, unsigned int> >::iterator it_adj = adj_lists.begin();
			for (unsigned int j = i; j > 0; --j)
			{
				++it_vert;
				++it_adj;
			}
			delete vertices[i];
			vertices.erase(it_vert);
			adj_lists.erase(it_adj);
			--i;
		}
	}


	return deleted;
}
