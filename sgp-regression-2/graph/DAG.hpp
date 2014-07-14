#ifndef DAG_CLASS
#define DAG_CLASS

#include <sstream>
#include <cassert>

#include "Hash.hpp"

class Node
{
public:
	virtual ~Node() {}

	virtual std::string print() const = 0;

	virtual short type() const = 0;

	virtual unsigned int size() const = 0;
};

class ConstantNode : public Node
{
public:
	const double value;

	inline ConstantNode(double value) : value(value) {}

	inline std::string print() const 
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	inline short type() const { return 0; }

	inline unsigned int size() const { return 1; }
};

class VariableNode : public Node
{
public:
	const unsigned int variable;

	inline VariableNode(unsigned int variable) : variable(variable) {}

	inline std::string print() const 
	{ 
		std::stringstream ss;
		ss << "X" << variable;
		return ss.str(); 
	}

	inline short type() const { return 1; }

	inline unsigned int size() const { return 1; }
};

class IntermediateNode : public Node
{
public:
	const std::string operation;
	const Node *left, *right;

	inline IntermediateNode(const std::string operation, Node* left, Node* right) : operation(operation), left(left), right(right) {}

	inline std::string print() const
	{
		std::stringstream ss;
		if (left != NULL && right != NULL)
			ss << "(" << left->print() << operation << right->print() << ")";
		else if (left != NULL && right == NULL)
			ss << operation << "(" << left->print() << ")";
		return ss.str();
	}

	inline short type() const { return 2; }

	inline unsigned int size() const
	{
		if (left != NULL && right != NULL)
			return 1 + left->size() + right->size();
		else if (left != NULL && right == NULL)
			return 1 + left->size();
		return 1;
	}
};

class DAG
{
public:
	inline DAG(const std::vector<std::string>& operators) : operators(operators) {}

	inline virtual ~DAG()
	{
		for (Node* n : nodes)
		{
			if (n != NULL)
				delete n;
		}
	}

	inline unsigned int add_terminal_node(double value)
	{
		unsigned int index = exists(value);
		if (index == UINT_MAX)
		{
			nodes.push_back(new ConstantNode(value));
			index = nodes.size() - 1;
			KeyValues hash_key(value, UINT_MAX, UINT_MAX);
			hash_table.put(hash_key, index);
		}
		return index;
	}

	inline unsigned int add_terminal_node(unsigned int variable)
	{
		unsigned int index = exists(variable);
		if (index == UINT_MAX)
		{
			nodes.push_back(new VariableNode(variable));
			index = nodes.size() - 1;
			Key hash_key(variable + operators.size(), UINT_MAX, UINT_MAX);
			hash_table.put(hash_key, index);
		}
		return index;
	}

	inline unsigned int add_intermediate_node(const std::string& operation, unsigned int left, unsigned int right)
	{
		unsigned int index = exists(operation, left, right);
		if (index == UINT_MAX)
		{
			nodes.push_back(new IntermediateNode(operation, nodes[left], nodes[right]));
			index = nodes.size() - 1;
			unsigned int index_op;
			for (index_op = 0; index_op < operators.size() && operation != operators[index_op]; ++index_op);
			Key hash_key(index_op, left, right);
			hash_table.put(hash_key, index);
		}
		return index;
	}

	inline unsigned int add_intermediate_node(const std::string& operation, unsigned int expr)
	{
		unsigned int index = exists(operation, expr, UINT_MAX);
		if (index == UINT_MAX)
		{
			nodes.push_back(new IntermediateNode(operation, nodes[expr], NULL));
			index = nodes.size() - 1;
			unsigned int index_op;
			for (index_op = 0; index_op < operators.size() && operation != operators[index_op]; ++index_op);
			Key hash_key(index_op, expr, UINT_MAX);
			hash_table.put(hash_key, index);
		}
		return index;
	}

	inline std::string print_expression(unsigned int index)
	{
		if (index < nodes.size())
			return nodes[index]->print();
		return "";
	}

	inline unsigned int size() { return nodes.size(); }

	inline unsigned int size(unsigned int index)
	{
		if (index < nodes.size())
			return nodes[index]->size();
		return UINT_MAX;
	}

	inline unsigned int get_index_constant() const { return 0; }

private:
	inline unsigned int exists(double value)
	{
		KeyValues hash_key(value, UINT_MAX, UINT_MAX);
		return hash_table.get(hash_key);
	}

	inline unsigned int exists(unsigned int variable)
	{
		Key hash_key(variable + operators.size(), UINT_MAX, UINT_MAX);
		return hash_table.get(hash_key);
	}

	inline unsigned int exists(const std::string& operation, unsigned int left, unsigned int right)
	{
		unsigned int index_op;
		for (index_op = 0; index_op < operators.size() && operation != operators[index_op]; ++index_op);
		Key hash_key(index_op, left, right);
		unsigned int index = hash_table.get(hash_key);
		if (index == UINT_MAX && (operation == "*" || operation == "+"))
		{
			Key alternative_key(index_op, right, left);
			index = hash_table.get(alternative_key);
		}
		return index;
	}

	std::vector<std::string> operators;
	std::vector<Node*> nodes;
	Hash hash_table;
};

#endif
