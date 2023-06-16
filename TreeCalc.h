#ifndef _TREECALC_H_INCLUDED
#define _TREECALC_H_INCLUDED
#include <string>
#include <vector>
#include <utility>
#include <map>

enum CalcError
{
	NoError = 0,
	UnknownNodeType,
	InvalidNodeData,
	UnknownOperator,
	VarUndefined,
	DivZero,
	RootNegative,
	DomainError,
	MathUndefined
};
std::tstring CEtoSTR(CalcError e);

struct Node
{
	enum Type { None = 0, Operator, Number, Variable } type;
	double val;
	std::tstring name;
	std::tstring toString() const;
};

class TreeCalc
{
	std::vector<Node> nodes;
	std::tstring hrf;

public:
	TreeCalc(void);
	TreeCalc(const TreeCalc &other);
	~TreeCalc(void);

	bool ProcessTreeString(std::tstring s);
	std::pair<double, CalcError> substitute(const std::map<std::tstring, double> &variables);
	std::tstring ToInfix();
	TreeCalc OptiSubstitute(const std::map<std::tstring, double> &variables);
	std::vector<double> Solve(const std::tstring &var, double min, double max, int steps);

	void operator=(const TreeCalc &other);
};

#endif
