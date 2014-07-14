#ifndef INSTANCE

#define INSTANCE

#include <map>
#include <string>

class Instance
{
public:
	const std::map<std::string, double> attrs;
	const double target;

	// Constructors
	inline Instance(const std::map<std::string, double>& attrs, double target) : attrs(attrs), target(target) {}

	// Destructor
	inline virtual ~Instance() {}

	inline bool has(std::string attr_name) const
	{
		std::map<std::string, double>::const_iterator it = attrs.find(attr_name);
		if (it != attrs.end())
			return true;
		else
			return false;
	}

	inline double get(std::string attr_name) const
	{
		assert(has(attr_name));
		std::map<std::string, double>::const_iterator it = attrs.find(attr_name);
		return it->second;
	}

	inline std::vector<std::string> names() const
	{
		std::vector<std::string> names;
		for (std::map<std::string, double>::const_iterator it = attrs.begin(); it != attrs.end(); ++it)
			names.push_back(it->first);
		return names;
	}
};

#endif
