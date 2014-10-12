#include <cstdlib>
#include <cmath>

#include "GP.hpp"

std::pair< unsigned int, std::vector<double> > change_subexpression(Individual* original, unsigned int subexpression, unsigned int change_point, 
	Digraph& digraph, const std::vector<double>& const_values)
{
	std::pair<unsigned int, unsigned int> neighbors = digraph.neighbors(original->index());
	unsigned int nodes_left = 0;
	if (neighbors.first < UINT_MAX)
		nodes_left = digraph.size(neighbors.first);

	if (change_point == nodes_left)
		return std::pair< unsigned int, std::vector<double> >(subexpression, const_values);
	else if (change_point < nodes_left)
	{
		unsigned int num = Individual::num_constants(neighbors.first, digraph);
		std::vector<double> values, old_values = original->get_values();
		for (unsigned int i = 0; i < num; ++i)
			values.push_back(old_values[i]);
		Individual temp(neighbors.first, &digraph, values);

		std::pair< unsigned int, std::vector<double> > result = change_subexpression(&temp, subexpression, change_point, 
			digraph, const_values);
		std::vector<double> final_values;
		for (double v : result.second)
			final_values.push_back(v);
		for (unsigned int i = num; i < old_values.size(); ++i)
			final_values.push_back(old_values[i]);
		IntermediateVertex* v = (IntermediateVertex*) digraph.vertex(original->index());
		Individual result_vertex(result.first, v->operation, neighbors.second, &digraph, final_values);
		return std::pair< unsigned int, std::vector<double> >(result_vertex.index(), final_values);
	}
	else
	{
		unsigned int num = Individual::num_constants(neighbors.first, digraph);
		std::vector<double> values, old_values = original->get_values();
		for (unsigned int i = num; i < old_values.size(); ++i)
			values.push_back(old_values[i]);
		change_point -= nodes_left + 1;
		Individual temp(neighbors.second, &digraph, values);

		std::pair< unsigned int, std::vector<double> > result = change_subexpression(&temp, subexpression, change_point, 
			digraph, const_values);
		std::vector<double> final_values;
		for (unsigned int i = 0; i < num; ++i)
			final_values.push_back(old_values[i]);
		for (double v : result.second)
			final_values.push_back(v);
		IntermediateVertex* v = (IntermediateVertex*) digraph.vertex(original->index());
		Individual result_vertex(neighbors.first, v->operation, result.first, &digraph, final_values);
		return std::pair< unsigned int, std::vector<double> >(result_vertex.index(), final_values);
	}
}

void GP::iteration(const Dataset& train, Digraph& graph)
{
	std::uniform_real_distribution<double> dist_non_neg(0.0, 1.0);
	Population* new_pop = new Population();

	// Elitism: adding best solution to new population
	unsigned int best_index = cur_pop->best(train, 0).first;
	new_pop->add_solution(new Individual((*cur_pop)[best_index]->index(), &graph, (*cur_pop)[best_index]->get_values()));

	for (unsigned int i = 1; i < pop_size; ++i)
	{
		double coin = dist_non_neg(generators[0]);
		if (coin < mut_rate)
		{
			std::vector<unsigned int> selected = selection(1, train);
			new_pop->add_solution(mutation((*cur_pop)[selected[0]], graph));
		}
		else
		{
			std::vector<unsigned int> selected = selection(2, train);
			new_pop->add_solution(crossover((*cur_pop)[selected[0]], (*cur_pop)[selected[1]], graph));
		}
	}

	delete cur_pop;
	cur_pop = new_pop;
}

void GP::initialize_pop(Digraph& graph)
{
	unsigned int num_full = 0;
	unsigned int num_grow = 0;
	if (mode_init == 1)
		num_full = pop_size;
	else if (mode_init == 2)
		num_grow = pop_size;
	else
	{
		num_full = pop_size / 2;
		num_grow = pop_size - num_full;
	}

	unsigned int num_threads = omp_get_max_threads();
	unsigned int full_thread = ceil(num_full / ((double) num_threads));
	unsigned int grow_thread = ceil(num_grow / ((double) num_threads));
	for (unsigned int i = 0; i < num_full; ++i)
		cur_pop->add_solution(Individual::build_random_individual(max_height, true, terminals, operators, graph, generators[i / full_thread]));
	for (unsigned int i = 0; i < num_grow; ++i)
		cur_pop->add_solution(Individual::build_random_individual(max_height, false, terminals, operators, graph, generators[i / grow_thread]));
}

std::vector<unsigned int> GP::selection(unsigned int num, const Dataset& train)
{
	std::vector<double> fitness = cur_pop->evaluate(train, 0);
	std::vector<unsigned int> selected;

	for (unsigned int i = 0; i < num; ++i)
	{
		unsigned int best = generators[0]() % fitness.size();
		for (unsigned int j = 0; j < tour_size; ++j)
		{
			unsigned int index = generators[0]() % fitness.size();
			if (fitness[index] > fitness[best])
				best = index;
		}
		selected.push_back(best);
	}
	return selected;
}

Individual* GP::mutation(Individual* original, Digraph& digraph)
{
	std::uniform_real_distribution<double> dist_non_neg(0.0, 1.0);
	double coin = dist_non_neg(generators[0]);

	//Subtree mutation
	if (coin < 0.5)
	{
		unsigned int height = generators[0]() % max_height + 1;
		Individual* subtree = Individual::build_random_individual(height, false, terminals, operators, digraph, generators[0]);
		unsigned int chosen = generators[0]() % digraph.size(original->index());
		std::pair< unsigned int, std::vector<double> > modified = change_subexpression(original, subtree->index(), chosen, digraph, subtree->get_values());
		Individual* full = new Individual(modified.first, &digraph, modified.second);
		unsigned int count = 0;
		std::vector<double> values = full->get_values();
		unsigned int pruned_height = generators[0]() % max_height + 1;
		unsigned int pruned_index = Individual::prune(full->index(), pruned_height, digraph, terminals, values, count, generators[0]);
		Individual* pruned = new Individual(pruned_index, &digraph, values);
		delete subtree;
		delete full;
		return pruned;
	}
	//Shrink mutation
	else
	{
		Individual* subtree = Individual::build_random_individual(1, false, terminals, operators, digraph, generators[0]);
		unsigned int chosen = generators[0]() % digraph.size(original->index());
		std::pair< unsigned int, std::vector<double> > modified = change_subexpression(original, subtree->index(), chosen, digraph, subtree->get_values());
		Individual* full = new Individual(modified.first, &digraph, modified.second);
		delete subtree;
		return full;
	}

}

Individual* GP::crossover(Individual* ind1, Individual* ind2, Digraph& digraph)
{
	Individual* subtree = ind1->random_subexpression(generators[0]);
	unsigned int chosen = generators[0]() % digraph.size(ind2->index());
	std::pair< unsigned int, std::vector<double> > modified = change_subexpression(ind2, subtree->index(), chosen, digraph, subtree->get_values());
	Individual* full = new Individual(modified.first, &digraph, modified.second);
	unsigned int count = 0;
	std::vector<double> values = full->get_values();
	unsigned int pruned_height = generators[0]() % max_height + 1;
	unsigned int pruned_index = Individual::prune(full->index(), pruned_height, digraph, terminals, values, count, generators[0]);
	Individual* pruned = new Individual(pruned_index, &digraph, values);
	delete subtree;
	delete full;
	return pruned;
}
