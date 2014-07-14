#ifndef INSTANCE

#define INSTANCE

#include <vector>
#include <string>

class Instance
{
public:
	const std::vector<double> attrs;
	const double target;

	// Constructors
	inline Instance(const std::vector<double>& attrs, double target) : attrs(attrs), target(target) {}

	// Destructor
	inline virtual ~Instance() {}

	inline unsigned int size() const { return attrs.size(); }
};

#endif
