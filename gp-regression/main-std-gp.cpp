#include <iostream>
#include <cstdlib>

#include "gp/Parameters.hpp"
#include "gp/GP.hpp"

using namespace std;

/*
*** Standard GP for symbolic regression ***
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

	vector<string> terminals = train->names();
	GP gp(param.pop_size, param.num_iter, param.max_height, param.tour_size, param.mut_rate, param.mode_init, terminals, param.operators);
	cerr << "----- PARAMETERS USED -----" << endl;
	param.print_parameters(cerr);

	cerr << gp.run(*train, *test) << endl;

	delete train;
	delete test;
}
