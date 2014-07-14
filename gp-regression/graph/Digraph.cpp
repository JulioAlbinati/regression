#include <iostream>

#include "Digraph.hpp"

const std::vector<double> Digraph::evaluate(unsigned int index, const Dataset& dataset, unsigned int dataset_id, const std::vector<double>& values, 
	unsigned int& cur_index)
{
	assert(index < size());
	std::map< unsigned int, std::vector<double> >::iterator it = previous_results[index].find(dataset_id);
	
	// If we have previous results, just send it back...
	if (it != previous_results[index].end())
		return it->second;

	// Otherwise, compute it!
	if (!vertices[index]->is_terminal())
	{
		IntermediateVertex* vertex = (IntermediateVertex*) vertices[index];
		std::vector<double> left_results = evaluate(adj_lists[index].first, dataset, dataset_id, values, cur_index);
		std::vector<double> right_results = evaluate(adj_lists[index].second, dataset, dataset_id, values, cur_index);
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

		previous_results[index][dataset_id] = final_results;
	}
	else
	{
		TerminalVertex* vertex = (TerminalVertex*) vertices[index];
		if (vertex->is_constant())
		{
			std::vector<double> to_return;
			for (unsigned int i = 0; i < dataset.size(); ++i)
				to_return.push_back(values[cur_index]);
			++cur_index;
			return to_return;
		}
		else
		{
			VariableVertex* var_vertex = (VariableVertex*) vertex;
			std::vector<double> results;
			for (unsigned int i = 0; i < dataset.size(); ++i)
				results.push_back(dataset[i].get(var_vertex->variable));
			previous_results[index][dataset_id] = results;
		}
	}

	return previous_results[index][dataset_id];
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
			std::vector< std::map< unsigned int, std::vector<double> > >::iterator it_res = previous_results.begin();
			for (unsigned int j = i; j > 0; --j)
			{
				++it_vert;
				++it_adj;
				++it_res;
			}
			delete vertices[i];
			vertices.erase(it_vert);
			adj_lists.erase(it_adj);
			previous_results.erase(it_res);
			--i;
		}
	}


	return deleted;
}
