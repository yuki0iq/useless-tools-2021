#include "stdafx.h"
#include "TreeCalc.h"
#include <algorithm>
#include <math.h>
using namespace std;

#define MATH_E        2.71828182845904523536
#define MATH_LOG2E    1.44269504088896340736
#define MATH_LOG10E   0.434294481903251827651
#define MATH_LN2      0.693147180559945309417
#define MATH_LN10     2.30258509299404568402
#define MATH_PI       3.14159265358979323846
#define MATH_PI_2     1.57079632679489661923
#define MATH_PI_4     0.785398163397448309616
#define MATH_1_PI     0.318309886183790671538
#define MATH_2_PI     0.636619772367581343076
#define MATH_2_SQRTPI 1.12837916709551257390
#define MATH_SQRT2    1.41421356237309504880
#define MATH_SQRT1_2  0.707106781186547524401
#define MATH_EPS      0.000000000000000000001 //10^-21

tstring CEtoSTR(CalcError e)
{
	switch (e)
	{
	case NoError:         return _T("No error occurred");
	case UnknownNodeType: return _T("Unknown TC Node type");
	case InvalidNodeData: return _T("TC Node data is corupted");
	case UnknownOperator: return _T("Unknown operator");
	case VarUndefined:    return _T("Variable is undefined");
	case DivZero:         return _T("Division by zero");
	case RootNegative:    return _T("Square root of a negative number");
	case DomainError:     return _T("Expression is not in a domain of a function");
	case MathUndefined:   return _T("Result of operation is undefined");
	default:              return _T("Unknown error");
	}
	// UNREACHABLE POINT
}

vector<tstring> StringTokenizer(tstring in, tstring separators)
{
	size_t pos, lastPos = 0, length = in.length();
	vector<tstring> tokens;
	while(lastPos < length + 1)
	{
		pos = in.find_first_of(separators, lastPos);
		if(pos == tstring::npos)
		{
			pos = length;
		}
		if(pos != lastPos)
		{
			tokens.push_back(tstring(in.data() + lastPos, pos - lastPos));
		}
		lastPos = pos + 1;
	}
	return tokens;
}

bool Contains(const vector<tstring> &a, tstring what)
{
	return a.end() != find(a.begin(), a.end(), what);
}

double ToNumber(tstring what)
{
	return stod(what);
}

bool IsNumber(tstring what)
{
	return (what[0] < 255 && isdigit(what[0])
		|| ((what[0] == _T('.') || what[0] == _T('-') || what[0] == _T('+')) && what.size() > 1));
}

tstring Node::toString() const
{
	switch (type)
	{
	case Operator:
		return name;
	case Variable:
		return name;
	case Number:
		return to_tstring((long double)val);
	default:
		return _T("<error-node>");
	}
	// UNREACHABLE POINT
}


map<tstring, pair<int, bool>> operators;
vector<tstring> functions;
bool isOperator(tstring s)
{
	return operators.find(s) != operators.end();
}
bool isFunction(tstring s)
{
	return Contains(functions, s);
}
int op_preced(tstring s)
{
	if (isOperator(s)) return operators[s].first;
	return 0;
}
bool op_left(tstring s)
{
	if (isOperator(s)) return operators[s].second;
	return true;
}

tstring InfixToRPN(tstring s)
{
	tstring res;

	tstring inp;
	for (int i = 0; i < s.size(); ++i)
		if (s[i] != ' ')
			inp.push_back(s[i]);
	inp.push_back(';');

	vector<tstring> operator_stack;
	tstring cur;
	int prev_type = -1;
	auto CharType = [](TCHAR c) -> int
	{
		if (c == _T(';')) return -1; // Stop.
		if (c >= _T('0') && c <= _T('9') || c == _T('.')) return 0; //Number
		if (c >= _T('A') && c <= _T('Z')) return 1; // function
		if (c >= _T('a') && c <= _T('z')) return 2; // variable
		if (c == _T('(')) return 3;
		if (c == _T(')')) return 4;
		if (c == _T(',')) return 5;
		return 6; // operator
	};
	for (int i = 0; i < inp.size(); ++i)
	{
		int cur_type = CharType(inp[i]);
		if (prev_type != cur_type || inp[i] == _T('(') || inp[i] == _T(')'))
		{
			// Flush cur
			switch (prev_type)
			{
			case -1: break;
			case 0: res += cur + _T(" "); break;
			case 1: operator_stack.push_back(cur); break;
			case 2: res += cur + _T(" "); break;
			case 3: operator_stack.push_back(cur); break;
			case 4:
				{
					bool ok = false;
					while(!ok && !operator_stack.empty())
					{
						tstring c = operator_stack.back();
						operator_stack.pop_back();
						if (c == _T("("))
						{
							ok = true;
						}
						else
						{
							res += c + _T(" ");
						}
					}
					if (!ok)
					{
						return _T("<Brackets error>");
					}
					if (!operator_stack.empty()
						&& CharType(operator_stack.back()[0]) == 1)
					{
						res += operator_stack.back() + _T(" ");
						operator_stack.pop_back();
					}
				}
				break;
			case 5:
				{
					bool ok = false;
					while(!ok && !operator_stack.empty())
					{
						tstring c = operator_stack.back();
						operator_stack.pop_back();
						if (c == _T("("))
						{
							ok = true;
						}
						else
						{
							res += c + _T(" ");
						}
					}
					operator_stack.push_back(_T("("));
					if (!ok)
					{
						return _T("<Brackets error>");
					}
				}
				break;
			case 6:
				{
					while(!operator_stack.empty())
					{
						tstring sc = operator_stack.back();
						if(CharType(sc[0]) == 6
							&& (    (op_left(cur) && op_preced(cur) <= op_preced(sc))
								|| (!op_left(cur) && op_preced(cur) <  op_preced(sc)) ))
						{
							res += sc + _T(" ");
							operator_stack.pop_back();
						}
						else
						{
							break;
						}
					}
					operator_stack.push_back(cur);
				}
				break;
			default: return _T("<Internal parsing error>");
			}

			cur.clear();
		}
		cur.push_back(inp[i]);
		prev_type = cur_type;
	}
	while(!operator_stack.empty())
	{
		res += operator_stack.back() + _T(" ");
		operator_stack.pop_back();
	}
	res.pop_back();

	return res;
}


TreeCalc::TreeCalc(void)
{
	if (operators.empty())
	{
		operators[_T("#")] = make_pair(-1, false);
		operators[_T("+")] = make_pair(1, true);
		operators[_T("-")] = make_pair(1, true);
		operators[_T("*")] = make_pair(2, true);
		operators[_T("/")] = make_pair(2, true);
		operators[_T("%")] = make_pair(2, true);
		operators[_T("**")] = make_pair(3, true);
		operators[_T("^")] = make_pair(3, false);
	}
	if (functions.empty())
	{
		functions.push_back(_T("NEG"));
		functions.push_back(_T("SQRT"));
		functions.push_back(_T("SIN"));
		functions.push_back(_T("COS"));
		functions.push_back(_T("TAN"));
		functions.push_back(_T("ASIN"));
		functions.push_back(_T("ACOS"));
		functions.push_back(_T("ATAN"));
		functions.push_back(_T("SINH"));
		functions.push_back(_T("COSH"));
		functions.push_back(_T("TANH"));
		functions.push_back(_T("ASINH"));
		functions.push_back(_T("ACOSH"));
		functions.push_back(_T("ATANH"));
		functions.push_back(_T("POW"));
		functions.push_back(_T("ABS"));
	}
}

TreeCalc::TreeCalc(const TreeCalc &other)
	: nodes(other.nodes)
{
}

TreeCalc::~TreeCalc(void)
{
}


bool TreeCalc::ProcessTreeString(tstring s)
{
	if (s.size() < 3 ||
		!(tolower(s[0]) == _T('r') && tolower(s[1]) == _T('p') && tolower(s[2]) == _T('n')))
	{
		hrf = s + _T(" ");
		s = InfixToRPN(s);
	}
	else
	{
		reverse(begin(s), end(s));
		s.pop_back();
		s.pop_back();
		s.pop_back();
		reverse(begin(s), end(s));
	}
	hrf += _T("<RPN ") + s + _T(">");

	vector<tstring> tok = StringTokenizer(s, _T(" \r\n\t"));
	int iNumTokens = tok.size();
	nodes.clear();
	nodes.resize(1);
	for (int i = 0; i < iNumTokens; ++i)
	{
		// if number
		// - curr = number
		// if variable
		// - curr = variable
		// if operator
		// - curr = operator
		if (IsNumber(tok[i]))
		{
			nodes.back().type = Node::Number;
			nodes.back().val = ToNumber(tok[i]);
		}
		else if (isFunction(tok[i]) || isOperator(tok[i]))
		{
			nodes.back().type = Node::Operator;
			nodes.back().name = tok[i];
		}
		else
		{
			nodes.back().type = Node::Variable;
			nodes.back().name = tok[i];
		}
		nodes.resize(i+2);
	}
	nodes.pop_back();

	// Optimize by constant elimination

	return true;
}

pair<double, CalcError> TreeCalc::substitute(const map<tstring, double> &variables)
{
	// Process this....
	vector<double> st;

	for (int i = 0; i < nodes.size(); ++i)
	{
		Node &cur = nodes[i];
		if (cur.type == Node::Type::Number)
		{
			st.push_back(cur.val);
		}
		else if (cur.type == Node::Type::Variable)
		{
			if(variables.find(cur.name) != variables.end())
			{
				double tmp = variables.at(cur.name);
				st.push_back(tmp);
			}
			else
			{
				return make_pair(0, CalcError::VarUndefined);
			}
		}
		else if (cur.type == Node::Type::Operator)
		{
			if (!isOperator(cur.name) && !isFunction(cur.name))
			{
				return make_pair(0, CalcError::UnknownOperator);
			}
			if (cur.name == _T("#"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.push_back(st.back());
			}
			else if (cur.name == _T("NEG"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = -st.back();
			}
			else if (cur.name == _T("SQRT"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				if (st.back() < 0) return make_pair(0, CalcError::RootNegative);
				st.back() = sqrt(st.back());
			}
			else if (cur.name == _T("SIN"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = sin(st.back());
			}
			else if (cur.name == _T("COS"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = cos(st.back());
			}
			else if (cur.name == _T("TAN"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = tan(st.back());
			}
			else if (cur.name == _T("ASIN"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				if (abs(st.back()) > 1) return make_pair(0, CalcError::DomainError);
				st.back() = asin(st.back());
			}
			else if (cur.name == _T("ACOS"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				if (abs(st.back()) > 1) return make_pair(0, CalcError::DomainError);
				st.back() = acos(st.back());
			}
			else if (cur.name == _T("ATAN"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = atan(st.back());
			}
			else if (cur.name == _T("SINH"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = sinh(st.back());
			}
			else if (cur.name == _T("COSH"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = cosh(st.back());
			}
			else if (cur.name == _T("TANH"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = tan(st.back());
			}
			else if (cur.name == _T("ASINH"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = asin(st.back());
			}
			else if (cur.name == _T("ACOSH"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				if (st.back() < 1) return make_pair(0, CalcError::DomainError);
				st.back() = acos(st.back());
			}
			else if (cur.name == _T("ATANH"))
			{
				if (st.empty()) return make_pair(0, CalcError::InvalidNodeData);
				if (abs(st.back()) > 1) return make_pair(0, CalcError::DomainError);
				st.back() = atan(st.back());
			}
			else if (cur.name == _T("+"))
			{
				if (st.size() < 2) return make_pair(0, CalcError::InvalidNodeData);
				double o2 = st.back(); st.pop_back();
				st.back() = (st.back() + o2);
			}
			else if (cur.name == _T("-"))
			{
				if (st.size() < 2) return make_pair(0, CalcError::InvalidNodeData);
				double o2 = st.back(); st.pop_back();
				st.back() = (st.back() - o2);
			}
			else if (cur.name == _T("*"))
			{
				if (st.size() < 2) return make_pair(0, CalcError::InvalidNodeData);
				double o2 = st.back(); st.pop_back();
				st.back() = (st.back() * o2);
			}
			else if (cur.name == _T("/"))
			{
				if (st.size() < 2) return make_pair(0, CalcError::InvalidNodeData);
				double o2 = st.back(); st.pop_back();
				if (abs(o2) < 1e-18) return make_pair(0, CalcError::DivZero);
				st.back() = (st.back() / o2);
			}
			else if (cur.name == _T("%"))
			{
				if (st.size() < 2) return make_pair(0, CalcError::InvalidNodeData);
				double o2 = st.back(); st.pop_back();
				if (abs(o2) < 1e-15) return make_pair(0, CalcError::DivZero);
				st.back() = fmod(st.back(), o2);
			}
			else if (cur.name == _T("**") || cur.name == _T("^") || cur.name == _T("POW"))
			{
				if (st.size() < 2) return make_pair(0, CalcError::InvalidNodeData);
				double o2 = st.back(); st.pop_back();
				st.back() = pow(st.back(), o2);
			}
			else if (cur.name == _T("ABS"))
			{
				if (st.size() < 1) return make_pair(0, CalcError::InvalidNodeData);
				st.back() = fabs(st.back());
			}
			else
			{
				return make_pair(0, CalcError::InvalidNodeData);
			}
		}
		else
		{
			return make_pair(0, CalcError::InvalidNodeData);
		}
	}

	return make_pair(st.back(), CalcError::NoError);
}

tstring TreeCalc::ToInfix()
{
	return hrf;
}

TreeCalc TreeCalc::OptiSubstitute(const map<tstring, double> &variables)
{
	// What the hell?
	TreeCalc tc(*this);
	/*for(int i = 0; i < tc.nodes.size(); ++i)
	{
		if(tc.nodes[i].type == Node::Variable)
		{
			if(variables.count(tc.nodes[i].name) != 0)
			{
				tc.nodes[i].type = Node::Number;
				tc.nodes[i].val = variables.at(tc.nodes[i].name);
			}
		}
	}*/
	return tc;
}

int sign(double a)
{
	if(abs(a) < 1e-6)
	{
		return 0;
	}
	else if (a > 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

vector<double> TreeCalc::Solve(const tstring &var, double dmin, double dmax, int steps)
{
	double dStep = (dmax - dmin) / steps;
	vector<double> res;
	/*map<string, double> empty;
	double x = dmin - dStep;
	double x1 = x + dStep;
	empty[var] = x;
	double fx = substitute(empty).first;
	empty[var] = x1;
	double fx1 = substitute(empty).first;
	while (x1 <= dmax)
	{
		if (abs(fx1) < 1e-6)
		{
			res.push_back(fx1);
		}
		else
		{
			if(sign(fx) != sign(fx1))
			{
				res.push_back(fx1);
			}
		}
		x = x1;
		fx = fx1;
		x1 += dStep;
		empty[var] = x1;
		fx1 = substitute(empty).first;
	}*/
	return res;
}

void TreeCalc::operator=(const TreeCalc &other)
{
	nodes = other.nodes;
}
