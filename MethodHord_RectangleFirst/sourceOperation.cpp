#include <iostream>
#include <stack>

using namespace std;

double factorial(double n, double acc, double val)
{
	if (acc != n)
	{
		acc++;
		return factorial(n, acc, val * acc);
	}
	else
		return val;
}

double fact(double n)
{
	double val = 1;
	return factorial(n, 1, val);
}

double oper_plus(stack <double>& s)
{
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a + b;
}

double oper_minus(stack <double>& s)
{
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return b - a;
}
double oper_mult(stack <double>& s)
{
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a * b;
}

double oper_div(stack <double>& s)
{
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	if (a == 0)
		return INFINITY;
	return b / a;
}

double oper_deg(stack <double> & s)
{
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	if (a == 0)
		return 1;
	return pow(b, a);
}

double oper_opposite(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return -a;
}

double oper_fact(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return fact(a);
}

double oper_mod(stack <double> & s)
{
	long long a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return b % a;
}

double oper_and(stack <double> & s)
{
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a && b;
}

double oper_or(stack <double> & s)
{
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a || b;
}
double oper_not(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return !a;
}
double oper_sin(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return sin(a);
}
double oper_cos(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return cos(a);
}
double oper_tan(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return tan(a);
}
double oper_asin(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return asin(a);
}
double oper_acos(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return acos(a);
}
double oper_atan(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return atan(a);
}

double oper_ln(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return log(a);
}
double oper_log(stack <double> & s)
{
	double a;
	a = s.top();
	s.pop();
	return log2(a);
}