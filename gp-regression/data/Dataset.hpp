#ifndef DATASET

#define DATASET

#include <vector>
#include <cassert>

#include "Instance.hpp"

class Dataset
{
public:
	// Constructors
	inline Dataset() {}

	inline Dataset(const std::vector<Instance*>& dataset) : dataset(dataset) {}

	// Destructor
	inline virtual ~Dataset()
	{
		for (unsigned int i = 0; i < dataset.size(); ++i)
		{
			if (dataset[i] != NULL)
				delete dataset[i];
		}
	}

	inline void add_instance(Instance* instance)
	{
		dataset.push_back(instance);
		std::vector<std::string> names_inst = instance->names();
		for (std::string& i : names_inst)
		{
			bool exists = false;
			for (std::string& d : names_attrs)
			{
				if (d == i)
					exists = true;
			}
			if (!exists)
				names_attrs.push_back(i);
		}
	}

	inline const std::vector<std::string>& names() const { return names_attrs; }

	inline const Instance& operator[](unsigned int index) const 
	{
		assert(index < size());
		return *(dataset[index]);
	}

	inline unsigned int size() const { return dataset.size(); }

private:
	std::vector<Instance*> dataset;
	std::vector<std::string> names_attrs;
};

Dataset* read_data_file(std::string);

#endif
