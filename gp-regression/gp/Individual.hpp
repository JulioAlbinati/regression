#ifndef INDIVIDUAL

#define INDIVIDUAL

#include <iostream>
#include <random>

#include "../graph/Digraph.hpp"

class Individual
{
public:
	// Constructors
	inline Individual (unsigned int index_vertex, Digraph* digraph, const std::vector<double>& values) : index_vertex(index_vertex), values(values), 
		digraph(digraph)
	{
		vertex = digraph->vertex(index_vertex);
		digraph->increase_ref_count(index_vertex);
	}

	Individual (unsigned int index_left, std::string operation, unsigned int index_right, Digraph* digraph, const std::vector<double>& values);

	// Destructor
	inline virtual ~Individual()
	{
		if (vertex != NULL && digraph != NULL)
			digraph->decrease_ref_count(index_vertex);
		vertex = NULL;
	}

	inline const std::vector<double>& evaluate(const Dataset& dataset, unsigned int dataset_id)
	{
		if (fitted[dataset_id].size() == 0)
		{
			unsigned int count = 0;
			fitted[dataset_id] = digraph->evaluate(index_vertex, dataset, values, count);
		}
		return fitted[dataset_id];
	}

	Individual* random_subexpression(std::mt19937& generator);

	inline unsigned int size() const { return digraph->size(index_vertex); }

	inline unsigned int index() const { return index_vertex; }

	inline void decrease_index() { --index_vertex; }

	inline const std::vector<double>& get_values() const { return values; }

	static inline unsigned int num_constants(unsigned int index_vertex, const Digraph& digraph)
	{
		Vertex* v = digraph.vertex(index_vertex);
		if (v->is_terminal())
		{
			TerminalVertex* t = (TerminalVertex*) v;
			if (t->is_constant())
				return 1;
			else
				return 0;
		}
		else
		{
			std::pair<unsigned int, unsigned int> neighbors = digraph.neighbors(index_vertex);
			unsigned int left = 0, right = 0;
			if (neighbors.first != UINT_MAX)
				left = num_constants(neighbors.first, digraph);
			if (neighbors.second != UINT_MAX)
				right = num_constants(neighbors.second, digraph);
			return left + right;
		}
	}

	static Individual* build_random_individual(unsigned int max_height, bool full, const std::vector<std::string>& terminals, 
		const std::vector<std::string>& operators, Digraph& graph, std::mt19937& generator);

	static unsigned int prune(unsigned int index_vertex, unsigned int max_height, Digraph& digraph, const std::vector<std::string> terminals, 
		std::vector<double>& values, unsigned int& cur_index, std::mt19937& generator);

private:
	unsigned int index_vertex;
	std::vector<double> values;
	std::map< unsigned int, std::vector<double> > fitted;
	Vertex* vertex;
	Digraph* digraph;

	Individual(const Individual&);

	Individual* random_subexpression(unsigned int index);
};

#endif
