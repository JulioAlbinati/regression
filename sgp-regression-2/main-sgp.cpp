#include <iostream>
#include <cstdlib>

#include "gp/Parameters.hpp"
#include "gp/SGP.hpp"

using namespace std;

/*
*** Standard Semantic GP ***
*/

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cerr << "You must inform the parameters file to be used!" << endl;
		return EXIT_FAILURE;
	}

	Parameters param;
	param.read_param_file(argv[1]);

	Dataset* train = read_data_file(param.train_file);
	Dataset* test = read_data_file(param.test_file);

	unsigned int num_terminals = train->num_attrs();
	SGP gp(param.pop_size, param.num_iter, param.max_height, param.mut_height, param.tour_size, param.mut_rate, param.mut_step, param.mode_init, 
		num_terminals, param.operators);
	cerr << "----- PARAMETERS USED -----" << endl;
	param.print_parameters(cerr);

	cout << gp.run(*train, *test) << endl;

	delete train;
	delete test;
}