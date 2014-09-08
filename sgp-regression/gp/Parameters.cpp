#include <fstream>
#include <sstream>
#include <cstdlib>

#include "Parameters.hpp"

void Parameters::read_param_file(std::string filename)
{
	std::ifstream reader(filename.data());
	if (reader.is_open())
	{
		std::string line;
		while (getline(reader, line))
		{
			// Removing whitespaces...
			std::stringstream ss;
			for (unsigned int i = 0; i < line.size(); ++i)
			{
				if (line[i] != ' ')
					ss << (char) tolower(line[i]);
			}

			// Checking which parameter is being set...
			std::string param_name, param_value;
			getline(ss, param_name, '=');
			getline(ss, param_value, '=');
			if (param_name == "initialization")
			{
				if (param_value == "full")
					mode_init = 1;
				else if (param_value == "grow")
					mode_init = 2;
				else if (param_value == "half")
					mode_init = 0;
			}
			else if (param_name == "population")
				pop_size = atoi(param_value.data());
			else if (param_name == "iterations")
				num_iter = atoi(param_value.data());
			else if (param_name == "tournament")
				tour_size = atoi(param_value.data());
			else if (param_name == "height")
				max_height = atoi(param_value.data());
			else if (param_name == "order")
				max_order = atoi(param_value.data());
			else if (param_name == "mutation_height")
				mut_height = atoi(param_value.data());
			else if (param_name == "seed")
				seed = atoi(param_value.data());
			else if (param_name == "mutation_rate")
				mut_rate = atof(param_value.data());
			else if (param_name == "mutation_step")
				mut_step = atof(param_value.data());
			else if (param_name == "error")
				error = atof(param_value.data());
			else if (param_name == "train")
				train_file = param_value;
			else if (param_name == "test")
				test_file = param_value;
			else if (param_name == "operators")
			{
				std::string token;
				std::stringstream ss_operators;
				ss_operators << param_value;
				while (getline(ss_operators, token, ','))
					operators.push_back(token);
			}
		}

		// Checking if required parameters were provided
		if (train_file == "")
		{
			std::cerr << "Train file required!" << std::endl;
			exit(0);
		}
		if (test_file == "")
			test_file = train_file;
		if (operators.size() == 0)
		{
			std::cerr << "At least one operator is required!" << std::endl;
			exit(0);
		}
	}
}

void Parameters::print_parameters(std::ostream& os)
{
	os << "Population size: " << pop_size << std::endl;
	os << "Number of iterations: " << num_iter << std::endl;
	os << "Initilization mode: ";
	if (mode_init == 0)
		os << "Ramped Half-and-Half" << std::endl;
	else if (mode_init == 1)
		os << "Full" << std::endl;
	else
		os << "Grow" << std::endl;
	os << "Maximum height of initial trees: " << max_height << std::endl;
	os << "Maximum height of mutation trees: " << mut_height << std::endl;
	os << "Maximum order of new trees: " << max_order << std::endl;
	os << "Seed: " << seed << std::endl;
	os << "Mutation rate: " << mut_rate << std::endl;
	os << "Mutation step: " << mut_step << std::endl;
	os << "Error allowed: " << error << std::endl;
	os << "Train file: " << train_file << std::endl;
	os << "Test file: " << test_file << std::endl;
	os << "Operators: " << operators[0];
	for (unsigned int i = 1; i < operators.size(); ++i)
		os << ", " << operators[i];
	os << std::endl;
}
