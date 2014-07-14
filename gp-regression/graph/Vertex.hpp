#ifndef VERTEX

#define VERTEX

#include <string>

class Vertex
{
public:
	inline Vertex() : ref_count(0) {}

	inline virtual ~Vertex() {}

	virtual bool is_terminal() const = 0;

	inline void increase_ref_count() { ++ref_count; }

	inline void decrease_ref_count() { --ref_count; }

	inline unsigned int get_ref_count() const { return ref_count; }
private:
	unsigned int ref_count;
};

class IntermediateVertex : public Vertex
{
public:
        const std::string operation;

        inline IntermediateVertex(const std::string& operation) : operation(operation) {}

	inline virtual ~IntermediateVertex() {}

        inline bool is_terminal() const { return false; }
};


class TerminalVertex : public Vertex
{
public:
	inline virtual ~TerminalVertex() {}

        inline bool is_terminal() const { return true; }

        virtual bool is_constant() const = 0;
};

class ConstantVertex : public TerminalVertex
{
public:
	inline virtual ~ConstantVertex() {}

	inline bool is_constant() const { return true; }
};

class VariableVertex : public TerminalVertex
{
public:
	const std::string variable;

	inline VariableVertex(const std::string& variable) : variable(variable) {}

	inline virtual ~VariableVertex() {}

	inline bool is_constant() const { return false; }
};

#endif
