#include <cstdlib>

#include "SemanticGP.hpp"

Individual* SemanticGP::mutation(Individual* original, Digraph& digraph)
{
	Individual* random_ind1 = Individual::build_random_individual(max_height, false, terminals, operators, digraph);
	Individual* random_ind2 = Individual::build_random_individual(max_height, false, terminals, operators, digraph);
	std::vector<double> const_values;
	for (double v : random_ind1->get_values())
		const_values.push_back(v);
	for (double v : random_ind2->get_values())
		const_values.push_back(v);
	Individual temp1(random_ind1->index(), "-", random_ind2->index(), &digraph, const_values);

	const_values.push_back(mut_step);
	Individual temp2(temp1.index(), "*", 0, &digraph, const_values);

	std::vector<double> final_values;
	for (double v : original->get_values())
		final_values.push_back(v);
	for (double v : const_values)
		final_values.push_back(v);
	Individual* i = new Individual(original->index(), "+", temp2.index(), &digraph, final_values);
	delete random_ind1;
	delete random_ind2;
	return i;
}

Individual* SemanticGP::crossover(Individual* ind1, Individual* ind2, Digraph& digraph)
{
	double coeff = ((double) rand()) / RAND_MAX;

	std::vector<double> const_coeff, const_compl;
	const_coeff.push_back(coeff);
	for (double v : ind1->get_values())
		const_coeff.push_back(v);
	const_compl.push_back(1.0 - coeff);
	for (double v : ind2->get_values())
		const_compl.push_back(v);
	Individual temp1(0, "*", ind1->index(), &digraph, const_coeff);
	Individual temp2(0, "*", ind2->index(), &digraph, const_compl);

	std::vector<double> final_const;
	for (double v : const_coeff)
		final_const.push_back(v);
	for (double v : const_compl)
		final_const.push_back(v);
	Individual* i = new Individual(temp1.index(), "+", temp2.index(), &digraph, final_const);
	return i;
}
