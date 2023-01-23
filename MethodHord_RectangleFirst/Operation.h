#pragma once
#ifndef  Operation_h
#define Operation_h
#include <iostream>
#include <stack>
#include <map>

using namespace std;

double factorial(double n, double acc, double val);
double fact(double n);
double oper_plus(stack <double>& s);
double oper_minus(stack <double>& s);
double oper_mult(stack <double>& s);
double oper_div(stack <double>& s);
double oper_deg(stack <double>& s);
double oper_opposite(stack <double>& s);
double oper_fact(stack <double>& s);
double oper_mod(stack <double>& s);
double oper_and(stack <double>& s);
double oper_or(stack <double>& s);
double oper_not(stack <double>& s);
double oper_sin(stack <double>& s);
double oper_cos(stack <double>& s);
double oper_tan(stack <double>& s);
double oper_asin(stack <double>& s);
double oper_acos(stack <double>& s);
double oper_atan(stack <double>& s);
double oper_ln(stack <double>& s);
double oper_log(stack <double>& s);

typedef double(*func_type)(stack<double>&);
typedef map<string, func_type> MAP_OPERATION;//Array operation

void createOperation(MAP_OPERATION& operationExprission)
{
	operationExprission["+"] = oper_plus;
	operationExprission["-"] = oper_minus;
	operationExprission["*"] = oper_mult;
	operationExprission["/"] = oper_div;
	operationExprission["^"] = oper_deg;
	operationExprission["opposite"] = oper_opposite;
	operationExprission["factorial"] = oper_fact;
	operationExprission["%"] = oper_mod;
	operationExprission["&"] = oper_and;
	operationExprission["|"] = oper_or;
	operationExprission["!"] = oper_not;
	operationExprission["sin"] = oper_sin;
	operationExprission["cos"] = oper_cos;
	operationExprission["tan"] = oper_tan;
	operationExprission["acos"] = oper_acos;
	operationExprission["asin"] = oper_asin;
	operationExprission["atan"] = oper_atan;
	operationExprission["ln"] = oper_ln;
	operationExprission["log"] = oper_log;
}

#endif // ! Operation_h

