#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "Dataset.hpp"

Dataset* read_data_file(std::string filename)
{
        std::ifstream reader(filename.data());
        if (reader.is_open())
        {
                Dataset* dataset = new Dataset();
                std::string line;
                while (getline(reader, line))
                {
                        std::stringstream ss(line);
                        std::string token;

                        getline(ss, token, ',');
			double target = atof(token.data());

			std::vector<double> attrs;
                        while (getline(ss, token, ','))
				attrs.push_back(atof(token.data()));
			Instance* instance = new Instance(attrs, target);
                        dataset->add_instance(instance);
                }
                return dataset;
        }

        return NULL;
}
