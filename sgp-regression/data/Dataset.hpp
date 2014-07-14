#ifndef DATASET

#define DATASET

#include <vector>
#include <cassert>

#include "Instance.hpp"

class Dataset
{
public:
	// Constructors
	inline Dataset() : size_attrs(0) {}

	inline Dataset(const std::vector<Instance*>& dataset) : dataset(dataset), size_attrs(0) {}

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
		if (instance->size() > size_attrs)
			size_attrs = instance->size();
	}

	inline unsigned int num_attrs() const { return size_attrs; }

	inline Instance* operator[](unsigned int index) const  { return dataset[index]; }

	inline unsigned int size() const { return dataset.size(); }

private:
	std::vector<Instance*> dataset;
	unsigned int size_attrs;
};

Dataset* read_data_file(std::string);

#endif
