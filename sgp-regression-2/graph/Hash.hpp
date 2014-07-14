#ifndef HASH

#define HASH

#define SIZE 1000000

#include <string>
#include <vector>
#include <list>
#include <climits>

class Key
{
public:
	const unsigned int description, left, right;

	inline Key(unsigned int description, unsigned int left, unsigned int right) : description(description), left(left), right(right) {}
};

class KeyValues
{
public:
	const double value;
	const unsigned int left, right;

	inline KeyValues(double value, unsigned int left, unsigned int right) : value(value), left(left), right(right) {}
};

inline bool operator==(const Key& a, const Key& b)
{
	return (a.description == b.description && a.left == b.left && a.right == b.right);
}

inline bool operator==(const KeyValues& a, const KeyValues& b)
{
	return (a.value == b.value && a.left == b.left && a.right == b.right);
}

class Hash
{
public:
	inline Hash() { values.resize(SIZE); values_constant.resize(SIZE); }

	inline void put(const Key& key, unsigned int index)
	{
		if (get(key) == UINT_MAX)
		{
			std::list< std::pair<Key, unsigned int> > colision_list = values[hash(key)];
			colision_list.push_back(std::pair<Key, unsigned int>(key, index));
		}
	}

	inline void put(const KeyValues& key, unsigned int index)
	{
		if (get(key) == UINT_MAX)
		{
			std::list< std::pair<KeyValues, unsigned int> > colision_list = values_constant[hash(key)];
			colision_list.push_back(std::pair<KeyValues, unsigned int>(key, index));
		}
	}

	inline void remove(const Key& key)
	{
		std::list< std::pair<Key, unsigned int> > colision_list = values[hash(key)];
		for (std::list< std::pair<Key, unsigned int> >::iterator it = colision_list.begin(); it != colision_list.end(); ++it)
		{
			if (key == it->first)
			{
				colision_list.erase(it);
				break;
			}	
		}
	}

	inline void remove(const KeyValues& key)
	{
		std::list< std::pair<KeyValues, unsigned int> > colision_list = values_constant[hash(key)];
		for (std::list< std::pair<KeyValues, unsigned int> >::iterator it = colision_list.begin(); it != colision_list.end(); ++it)
		{
			if (key == it->first)
			{
				colision_list.erase(it);
				break;
			}	
		}
	}

	inline unsigned int get(const Key& key)
	{
		std::list< std::pair<Key, unsigned int> > colision_list = values[hash(key)];
		for (std::list< std::pair<Key, unsigned int> >::iterator it = colision_list.begin(); it != colision_list.end(); ++it)
		{
			if (key == it->first)
				return it->second;
		}
		return UINT_MAX;
	}

	inline unsigned int get(const KeyValues& key)
	{
		std::list< std::pair<KeyValues, unsigned int> > colision_list = values_constant[hash(key)];
		for (std::list< std::pair<KeyValues, unsigned int> >::iterator it = colision_list.begin(); it != colision_list.end(); ++it)
		{
			if (key == it->first)
				return it->second;
		}
		return UINT_MAX;
	}
private:
	inline unsigned int hash(const Key& key)
	{
		unsigned int p1 = key.left << 16;
		unsigned int p2 = key.right << 16;
		p2 = p2 >> 16;
		return (p1 + p2) % SIZE;
	}

	inline unsigned int hash(const KeyValues& key)
	{
		unsigned int p1 = key.left << 16;
		unsigned int p2 = key.right << 16;
		p2 = p2 >> 16;
		return (p1 + p2) % SIZE;
	}

	std::vector< std::list< std::pair<Key, unsigned int> > > values;
	std::vector< std::list< std::pair<KeyValues, unsigned int> > > values_constant;
};

#endif
