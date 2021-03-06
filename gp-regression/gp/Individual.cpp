#include <cstdlib>
#include <iostream>

#include "Individual.hpp"

Individual::Individual (unsigned int index_left, std::string operation, unsigned int index_right, Digraph* digraph, const std::vector<double>& values) : 
	values(values), digraph(digraph)
{
	index_vertex = digraph->exists(operation, index_left, index_right);
	if (index_vertex == UINT_MAX)
	{
		vertex = new IntermediateVertex(operation);
		index_vertex = digraph->add_vertex(vertex);
		digraph->set_as_neighbor(index_vertex, index_left, index_right);
		digraph->increase_ref_count(index_vertex);
	}
	else
	{
		vertex = digraph->vertex(index_vertex);
		digraph->increase_ref_count(index_vertex);
	}
}

unsigned int Individual::prune(unsigned int index_vertex, unsigned int max_height, Digraph& digraph, const std::vector<std::string> terminals, 
	std::vector<double>& values, unsigned int& cur_index, std::mt19937& generator)
{
	std::uniform_real_distribution<double> dist_non_neg(0.0, 1.0);
	std::uniform_real_distribution<double> dist_pos_neg(-1.0, 1.0);
	Vertex* v = digraph.vertex(index_vertex);
	if (max_height == 0)
	{
		if (!v->is_terminal())
		{
			// Removing constants located in branchs too deep...
			unsigned int to_remove = num_constants(index_vertex, digraph);
			std::vector<double>::iterator it_begin = values.begin();
			for (unsigned int i = 0; i < cur_index; ++i)
				++it_begin;
			std::vector<double>::iterator it_end = it_begin;
			for (unsigned int i = 0; i < to_remove; ++i)
				++it_end;
			values.erase(it_begin, it_end);

			double coin = dist_non_neg(generator);
			if (coin < 0.5)
			{
				unsigned int index = generator() % terminals.size();
				index_vertex = digraph.exists(terminals[index]);
				if (index_vertex == UINT_MAX)
					index_vertex = digraph.add_vertex(new VariableVertex(terminals[index]));
			}
			else
			{
				double value = dist_pos_neg(generator);
				index_vertex = 0;
				values.push_back(value);
				++cur_index;
			}
		}
		return index_vertex;
	}
	else
	{
		if (!v->is_terminal())
		{
			std::pair<unsigned int, unsigned int> neighbors = digraph.neighbors(index_vertex);
			unsigned int left = prune(neighbors.first, max_height - 1, digraph, terminals, values, cur_index, generator);
			unsigned int right = prune(neighbors.second, max_height - 1, digraph, terminals, values, cur_index, generator);
			if (left != neighbors.first || right != neighbors.second)
			{
				IntermediateVertex* inter = (IntermediateVertex*) v;
				index_vertex = digraph.exists(inter->operation, left, right);
				if (index_vertex == UINT_MAX)
				{
					index_vertex = digraph.add_vertex(new IntermediateVertex(inter->operation));
					digraph.set_as_neighbor(index_vertex, left, right);
				}
			}
		}
		else
		{
			TerminalVertex* t = (TerminalVertex*) v;
			if (t->is_constant())
				++cur_index;
		}
		return index_vertex;
	}
}

Individual* Individual::random_subexpression(std::mt19937& generator)
{
	unsigned int chosen = generator() % digraph->size(index_vertex);
	return random_subexpression(chosen);
}

Individual* Individual::random_subexpression(unsigned int chosen)
{
	std::pair<unsigned int, unsigned int> neighbors = digraph->neighbors(index_vertex);
	unsigned int nodes_left = 0;
	if (neighbors.first < UINT_MAX && neighbors.second < UINT_MAX)
		nodes_left = digraph->size(neighbors.first);

	if (chosen < nodes_left)
	{
		unsigned int num = num_constants(neighbors.first, *digraph);
		std::vector<double> new_values;
		for (unsigned int i = 0; i < num; ++i)
			new_values.push_back(values[i]);
		Individual temp(neighbors.first, digraph, new_values);
		return temp.random_subexpression(chosen);
	}
	else if (chosen == nodes_left)
		return new Individual(index_vertex, digraph, values);
	else
	{
		unsigned int num = num_constants(neighbors.first, *digraph);
		std::vector<double> new_values;
		for (unsigned int i = num; i < values.size(); ++i)
			new_values.push_back(values[i]);
		Individual temp(neighbors.second, digraph, new_values);
		chosen -= nodes_left + 1;
		return temp.random_subexpression(chosen);
	}
}

Individual* Individual::build_random_individual(unsigned int max_height, bool full, const std::vector<std::string>& terminals, 
	const std::vector<std::string>& operators, Digraph& graph, std::mt19937& generator)
{
	std::uniform_real_distribution<double> dist_non_neg(0.0, 1.0);
	std::uniform_real_distribution<double> dist_pos_neg(-1.0, 1.0);
	std::vector< std::vector< std::pair<unsigned int, short> > > tree_matrix;
	if (full)
	{
		unsigned int nodes_level = 1;
		// Generating only operators...
		for (unsigned int i = 0; i < max_height - 1; ++i)
		{
			tree_matrix.push_back(std::vector< std::pair<unsigned int, short> >());
			for (unsigned int j = 0; j < nodes_level; ++j)
			{
				unsigned int index = generator() % operators.size();
				tree_matrix[i].push_back(std::pair<unsigned int, short>(index, 0));
			}
			nodes_level *= 2;
		}
		// Generating terminal nodes...
		tree_matrix.push_back(std::vector< std::pair<unsigned int, short> >());
		for (unsigned int i = 0; i < nodes_level; ++i)
		{
			double coin = dist_non_neg(generator);
			if (coin < 2)
			{
				unsigned int index = generator() % terminals.size();
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
			tree_matrix.push_back(std::vector< std::pair<unsigned int, short> >());
			for (unsigned int j = 0; j < nodes_level; ++j)
			{
				double coin = dist_non_neg(generator);
				if (coin < 0.9 && i < max_height - 1)
				{
					unsigned int index = generator() % operators.size();
					tree_matrix[i].push_back(std::pair<unsigned int, short>(index, 0));
					next_level += 2;
				}
				else
				{
					coin = dist_non_neg(generator);
					if (coin < 0.5)
					{
						unsigned int index = generator() % terminals.size();
						tree_matrix[i].push_back(std::pair<unsigned int, short>(index, 1));
					}
					else
						tree_matrix[i].push_back(std::pair<unsigned int, short>(0, 2));
				}
			}
			nodes_level = next_level;
		}
	}

	std::vector<Individual*> roots;
	for (int i = tree_matrix.size() - 1; i >= 0; --i)
	{
		std::vector<Individual*> new_roots;
		unsigned int cur_index = 0;
		for (unsigned int j = 0; j < tree_matrix[i].size(); ++j)
		{
			if (tree_matrix[i][j].second == 0)
			{
				std::vector<double> v1 = roots[cur_index]->get_values();
				std::vector<double> v2 = roots[cur_index + 1]->get_values();
				for (double v : v2)
					v1.push_back(v);
				Individual* temp = new Individual(roots[cur_index]->index(), operators[tree_matrix[i][j].first], 
					roots[cur_index + 1]->index(), &graph, v1);
				new_roots.push_back(temp);
				cur_index += 2;
			}
			else if (tree_matrix[i][j].second == 1)
			{
				std::vector<double> values;
				unsigned int index_vertex = graph.exists(terminals[tree_matrix[i][j].first]);
				if (index_vertex == UINT_MAX)
					index_vertex = graph.add_vertex(new VariableVertex(terminals[tree_matrix[i][j].first]));
				new_roots.push_back(new Individual(index_vertex, &graph, values));
			}
			else
			{
				std::vector<double> values;
				values.push_back(dist_pos_neg(generator));
				new_roots.push_back(new Individual(0, &graph, values));
			}
		}
		
		for (unsigned int j = 0; j < roots.size(); ++j)
		{
			if (roots[j] != NULL)
				delete roots[j];
		}
		roots = new_roots;
	}

	return roots[0];
}
