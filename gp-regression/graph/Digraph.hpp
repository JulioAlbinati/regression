#ifndef DIGRAPH

#define DIGRAPH

#include <vector>
#include <map>
#include <climits>
#include <sstream>
#include <iostream>
#include <omp.h>

#include "Vertex.hpp"
#include "../data/Dataset.hpp"

class Digraph
{
public:
	inline Digraph()
	{
		vertices.push_back(new ConstantVertex());
		vertices[0]->increase_ref_count();
		adj_lists.push_back(std::pair<unsigned int, unsigned int>(UINT_MAX, UINT_MAX));
		previous_results.push_back(std::map< unsigned int, std::vector<double> >());
	}

	inline virtual ~Digraph()
	{
		for (unsigned int i = 0; i < vertices.size(); ++i)
		{
			if (vertices[i] != NULL)
				delete vertices[i];
			vertices[i] = NULL;
		}
	}

	inline unsigned int add_vertex(Vertex* vertex)
	{
		vertices.push_back(vertex);
		adj_lists.push_back(std::pair<unsigned int, unsigned int>(UINT_MAX, UINT_MAX));
		previous_results.push_back(std::map< unsigned int, std::vector<double> >());
		return vertices.size() - 1;
	}

	inline void set_as_neighbor(unsigned int from, unsigned int left, unsigned int right)
	{
		if (from < vertices.size() && left < vertices.size() && right < vertices.size())
			adj_lists[from] = std::pair<unsigned int, unsigned int>(left, right);
	}

	const std::vector<double> evaluate(unsigned int index, const Dataset& dataset, unsigned int dataset_id, const std::vector<double>& values, 
		unsigned int& cur_index);

	inline unsigned int exists(std::string operation, unsigned int left, unsigned int right) const
	{
		unsigned int ret = UINT_MAX;
		for (unsigned int i = 0; i < vertices.size(); ++i)
		{
			if (!vertices[i]->is_terminal())
			{
				IntermediateVertex* v = (IntermediateVertex*) vertices[i];
				if (v->operation == operation)
				{
					if (((operation == "+" || operation == "*") && adj_lists[i].first == right && adj_lists[i].second == left) ||
					    (adj_lists[i].first == left && adj_lists[i].second == right))
					{
						ret = i;
						break;
					}
				}
			}
		}
		return ret;
	}

	inline unsigned int exists(std::string variable) const
	{
		for (unsigned int i = 0; i < vertices.size(); ++i)
		{
			if (vertices[i]->is_terminal())
			{
				TerminalVertex* v = (TerminalVertex*) vertices[i];
				if (!v->is_constant())
				{
					VariableVertex* var_vertex = (VariableVertex*) v;
					if (var_vertex->variable == variable)
						return i;
				}
			}
		}

		return UINT_MAX;
	}

	inline const std::pair<unsigned int, unsigned int>& neighbors(unsigned int index) const
	{
		assert(index < adj_lists.size());
		return adj_lists[index];
	}

	inline std::string subgraph_as_string(unsigned int index, const std::vector<double>& values, unsigned int& cur_index) const
	{
		if (index < vertices.size())
		{
			if (vertices[index]->is_terminal())
			{
				TerminalVertex* v = (TerminalVertex*) vertices[index];
				if (v->is_constant())
				{
					std::stringstream ss;
					ss << values[cur_index];
					++cur_index;
					return ss.str();
				}
				else
					return ((VariableVertex*) v)->variable;
			}
			else
			{
				std::string left = subgraph_as_string(adj_lists[index].first, values, cur_index);
				std::string right = subgraph_as_string(adj_lists[index].second, values, cur_index);
				IntermediateVertex* v = (IntermediateVertex*) vertices[index];
				return "(" + left + v->operation + right + ")";
			}
		}
		return "";
	}

	inline void increase_ref_count(unsigned int index)
	{
		if (index < vertices.size())
		{
			vertices[index]->increase_ref_count();
			std::pair<unsigned int, unsigned int> neighbors = adj_lists[index];
			if (neighbors.first != UINT_MAX)
				increase_ref_count(neighbors.first);
			if (neighbors.second != UINT_MAX)
				increase_ref_count(neighbors.second);
		}
	}

	inline void decrease_ref_count(unsigned int index)
	{
		if (index < vertices.size())
		{
			vertices[index]->decrease_ref_count();
			std::pair<unsigned int, unsigned int> neighbors = adj_lists[index];
			if (neighbors.first != UINT_MAX)
				decrease_ref_count(neighbors.first);
			if (neighbors.second != UINT_MAX)
				decrease_ref_count(neighbors.second);
		}
	}

	inline void print_ref_count(unsigned int index)
	{
		if (index < vertices.size())
		{
			std::cout << vertices[index]->get_ref_count() << " ";	
			std::pair<unsigned int, unsigned int> neighbors = adj_lists[index];
			if (neighbors.first != UINT_MAX)
				print_ref_count(neighbors.first);
			if (neighbors.second != UINT_MAX)
				print_ref_count(neighbors.second);
		}
	}

	std::vector<unsigned int> remove_unused();

	inline Vertex* vertex(unsigned int index) const
	{
		if (index < vertices.size())
			return vertices[index];
		else
			return NULL;
	}

	inline unsigned int size() const { return vertices.size(); }

	inline unsigned int size(unsigned int root) const
	{
		if (root < vertices.size())
		{
			if (vertices[root]->is_terminal())
				return 1;
			else
				return 1 + size(adj_lists[root].first) + size(adj_lists[root].second);
		}

		return 0;
	}

private:
	std::vector<Vertex*> vertices;
	std::vector< std::pair<unsigned int, unsigned int> > adj_lists;
	std::vector< std::map< unsigned int, std::vector<double> > > previous_results;
};

#endif
