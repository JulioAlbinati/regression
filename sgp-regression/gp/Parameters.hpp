#ifndef PARAMETERS

#define PARAMETERS

#include <string>
#include <vector>
#include <iostream>

class Parameters
{
public:
	short mode_init;
	unsigned int pop_size, num_iter, tour_size, max_height, max_order, mut_height;
	double mut_rate, mut_step, error;

	std::string train_file, test_file;
	std::vector<std::string> operators;

	inline Parameters() : mode_init(0), pop_size(10), num_iter(100), tour_size(2), max_height(6), max_order(6), mut_height(6), mut_rate(0.5), 
		mut_step(0.01), error(0.0), train_file(""), test_file("") {}

	void read_param_file(std::string filename);

	void print_parameters(std::ostream& os);
};

#endif
