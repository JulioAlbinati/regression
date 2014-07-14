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

			std::map<std::string, double> attrs;
                        while (getline(ss, token, ','))
                        {
				std::stringstream ss_attr(token);
				std::string token_attr;

				getline(ss_attr, token_attr, '=');
				std::string attr_name = token_attr;
				getline(ss_attr, token_attr, '=');
				double attr_value = atof(token_attr.data());
				attrs[attr_name] = attr_value;
                        }
			Instance* instance = new Instance(attrs, target);
                        dataset->add_instance(instance);
                }
                return dataset;
        }

        return NULL;
}
