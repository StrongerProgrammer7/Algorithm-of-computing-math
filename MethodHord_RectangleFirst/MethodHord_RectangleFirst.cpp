#include <iostream>
#include <omp.h>
#include<cmath>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <stack>
#include <fstream>
#include <regex>
#include "Operation.h"
#include "prefixTopostfix.h"

using namespace std;

enum tokenType
{
	var,/*Перем*/
	cst, /*конст*/
	openBr,/* ( */
	closeBr,/* ) */
	func,/*функция*/
	operation, /*операция*/
	arr,/*массив*/
	openArBr, /* { */
	closeArBr, /* } */
	identify/*неизвестно*/
};

struct token
{
	string name;
	tokenType type;
	token(string str, tokenType typ)
	{
		name = str;
		type = typ;
	}
	token() { name = ""; type = identify; }
};

typedef vector<token> TOKENS;

class NonlinearEquation
{

public:
	NonlinearEquation()
	{
		inputData();
	}

	double getStartTime()
	{
		return this->startTime;
	}
	double getEndTime()
	{
		return this->endTime;
	}
	double getX()
	{
		return this->x;
	}
	double getfunctionFromX()
	{
		return this->resultfunction;

	}

	void calculationFunction()
	{
		calculation();
		cout << " Time calculation : " << endTime - startTime << endl;
	}
private:
	
	typedef map<string, double> VAR_INEXPR;
	typedef map<string, vector<double>> ARRAY_INEXPR;

	typedef double(*func_type)(stack<double>&); //Указатель на функцию(для операций)
	typedef map<string, func_type> MAP_OPERATION;//Array operation

	double beginInterval = 0,endInterval = 0, b = 0, x = 0, resultfunction = 0;
	double startTime = 0, endTime = 0;
	double epsilon = 0.001;
	TOKENS postfixExprissionToken;
	VAR_INEXPR varsExprission;
	ARRAY_INEXPR arrayExprission;

	void inputData()
	{
		cout << "Input begin interval : ";
		cin >> this->beginInterval;
		cout << " Input end interval : ";
		cin >> this->endInterval;
		
		cout << " You can input initial approximation,if you want... (_/0)  ";
		cin >> this->b;
		if (b == 0)
			b = endInterval;

		cout << "Input epsilon : (recommended 0.001)  ";
		cin >> epsilon;

		cout << " Now input integral function from only on variable, please..." << endl;
		inputFunction();
	}
	
	//Функция считывает выражение в строку "expr" и ищем переменные или массивы
	void readExpressionFromStream(ifstream& file, string& exprString, VAR_INEXPR& var, ARRAY_INEXPR& arr)
	{
		getline(file, exprString);
		string temp;
		size_t pos = 0;

		while (!file.eof())
		{
			getline(file, temp);
			pos = temp.find('=');
			if (pos > 0)
			{
				string name = temp.substr(0, pos);
				double value = atof(temp.substr(static_cast<unsigned __int64>(pos) + 1).c_str()); // создает массив из оставшихся символов в строке
				var[name] = value;
			}pos = temp.find('{');
			if (pos > 0)
			{
				string name = temp.substr(0, pos);
				size_t pos1 = pos, pos2;
				do
				{
					pos2 = temp.find(',');
					double value = atof(temp.substr(static_cast<unsigned __int64>(pos1) + 1, pos2).c_str());
					arr[name].push_back(value);
					if (pos2 == -1)
						break;
					temp[pos2] = ' ';
					pos1 = pos2;
				} while (pos2 > 0);
			}

		}
		return;
	}

	void inputFunction()
	{
		TOKENS expressionToken;
		string expression;

		cout << " Load a function from a file or input to string?(0/1) ";
		bool askLoad = false;
		cin >> askLoad;
		do
		{
			if (askLoad == true)
			{
				cout << "check your file and touch enter" << endl;
				cin.get();
				ifstream file("test.txt");
				if (!file.is_open())
				{
					readExpressionFromStream(file, expression, varsExprission, arrayExprission);
					file.close();

				}
				else
				{
					cout << " File is not open! Check your file!";
					exit(0);
				}
			}
			else
			{
				cout << "Don't use space! Example : x^2-4*x+cos(x)-5!+ln(x^2)-(log(4)/2)+tan(50.5)+12.5\n";
				expression = "";
				cin >> expression;
			}
			cout << " expression:  " << expression << endl;
			expressionToken.clear();
			createTokensFromExpression(expression,expressionToken);

		} while (checkExpression(expressionToken) == false);

		createPostfixFromToken(expressionToken, postfixExprissionToken);
	}

	bool checkExpression(TOKENS exprToken)
	{
		regex regExpression("^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)$|(^sin$)|(^cos$)|(^tan$)|(^asin$)|(^acos$)|(^atan$)|(^ln$)|(^log$)|((^x$))|([(]|[)])|([+-]|['*']|['^']|[!]|[/]|[%]|[|]|[&])");
		for (int i = 0; i < exprToken.size(); i++)
		{
			if (regex_match(exprToken[i].name, regExpression) == false)
			{
				cout << " Your expression don't correct!!" << endl;
				return false;
			}
		}
		return true;
	}

	void calculation()
	{

		startTime = omp_get_wtime();
		methodHord(beginInterval, endInterval, b);
		endTime = omp_get_wtime();

	}
	void methodHord(double beginInterval, double endInterval, double b)
	{
		double a = beginInterval;
		double c1 = 0, c0 = a;

		int countIteration = 0;
		double funcForA=0, funcForB=0, tempa = 0, tempb = 0;
#pragma omp parallel num_threads(4)
		{
#pragma omp for schedule(dynamic)
			for (; countIteration < 1500 && abs(c1 - c0) > epsilon; countIteration++)//while (abs(c1 - c0) > epsilon)
			{
				c0 = c1;
#pragma omp sections
				{
					#pragma omp section
					if ((a != tempa && countIteration > 1) || countIteration < 2)
						funcForA = functionG(beginInterval, endInterval, a);
					#pragma omp section
					if ((b != tempb && countIteration > 1) || countIteration < 2)
						funcForB = functionG(beginInterval, endInterval, b);
					
				}
#pragma omp barrier
				c1 = a - ((funcForA / (funcForB - funcForA)) * (b - a));
				double funcForC1 = functionG(beginInterval, endInterval, c1);
				
				if (funcForA * funcForC1 < 0)
				{
					b = c1;
				}
				else
					if (funcForC1 * funcForB < 0)
					{
						a = c1;
					}

				tempa = a;
				tempb = b;
			}
		}

		double x = a - (functionG(beginInterval, endInterval, a) /
			(functionG(beginInterval, endInterval, b) - functionG(beginInterval, endInterval, a))) * (b - a);
		this->resultfunction = functionG(beginInterval, endInterval, x);
		cout << " X : " << x << " g(x) =  " << resultfunction << endl;
		
	}

	double functionG(double beginInterval, double endInterval, double xk)
	{
		double integral = 0;

		if (xk != beginInterval)
		{
			double index = 1;
#pragma omp parralel num_threads(4) for schedule(dynamic)
			for(double i=1,diff=10;i<136485 && diff > epsilon;i*=2)
			{
#pragma omp section
				diff = fabs(integralCentralRect(beginInterval, xk, static_cast<__int64>(endInterval * i)) -
					integralCentralRect(beginInterval, xk, static_cast<__int64>(endInterval * (i + 1.0))));
				index = i;
			} 
			integral = integralCentralRect(beginInterval, xk, static_cast<__int64>(endInterval * (index + 1.0)));
		}

		return integral - endInterval;
	}

	double integralCentralRect(double const a, double const x, __int64 countSegment)
	{
		//unsigned __int64 c = static_cast<unsigned __int64>(countSegment);
		double h = (x - a) / (double)(countSegment - 1);
		double sum = 2 * h * function(x);
#pragma omp parallel num_threads(4)
		{
#pragma omp for reduction(+:sum) schedule(dynamic)
			for (double x1 = a; x1 <= x; x1 += h)//for (int i = a; i < c; i++)
			{
				sum += function(x1 - h / 2.0);//function(a + ((double)(static_cast<__int64>(i) - 0.5) * h));
			}
		}
		return sum * h;
	}

	double function(double x)
	{
		string name = "x";
		double value = x;
		varsExprission[name] = value;
		return calculationExprission(postfixExprissionToken, varsExprission, arrayExprission);
	}
	/*Вычисление функции*/
	double calculationExprission(TOKENS & postfixExpressionToken, VAR_INEXPR & varsInExpression, ARRAY_INEXPR & arrayInExpression)
	{

		MAP_OPERATION operationExprission;
		createOperation(operationExprission);

		stack<double> stackForCalcExprission;
		for (int i = 0; i < postfixExpressionToken.size(); i++)
		{
			
			switch (postfixExpressionToken[i].type)
			{
			case cst:
			{
				stackForCalcExprission.push(atoi(postfixExpressionToken[i].name.c_str()));
			}
			break;
			case var:
			{
				VAR_INEXPR::iterator varIt;
				for (varIt = varsInExpression.begin(); varIt != varsInExpression.end(); varIt++)
				{
					if (varIt->first == postfixExpressionToken[i].name)
					{
						stackForCalcExprission.push(varIt->second);
						break;
					}
				}

			}
			break;
			case func:
			case operation:
			{
				MAP_OPERATION::iterator operationIt;
				for (operationIt = operationExprission.begin(); operationIt != operationExprission.end(); operationIt++)
				{
					if (operationIt->first == postfixExpressionToken[i].name)
					{
						stackForCalcExprission.push(operationIt->second(stackForCalcExprission));
						break;
					}
				}
			}
			break;
			case arr:
			{
				double index = stackForCalcExprission.top();
				stackForCalcExprission.pop();

				ARRAY_INEXPR::iterator arrayIt;
				for (arrayIt = arrayInExpression.begin(); arrayIt != arrayInExpression.end(); arrayIt++)
				{
					if (arrayIt->first == postfixExpressionToken[i].name)
						stackForCalcExprission.push(arrayIt->second[static_cast<__int64>(index)]);
				}
			}
			break;//him this no break;
			default:
				break;
			}
		}
		return stackForCalcExprission.top();
	}
	/*------------------------------------------END comute function*/

	
};

int main()
{
	cout << " \t\tYou're using method hord for calculate expression :\n \t\t\t'integral(a, xk) f(x)dx - b'.  " << endl;
	list <NonlinearEquation> listFunction;
	while (true)
	{
		int countFunction = 0;
		cout << "Input count function, whose you calculate \n(remember what number of functions you designate, how many times you will need to enter) : ";
		cin >> countFunction;
		for (int i = 1; i <= countFunction; i++)
		{
			cout << " function  " << i << endl;
			NonlinearEquation func1 = NonlinearEquation();
			func1.calculationFunction();
			listFunction.push_back(func1);
			cout << "Next function \n";
		}

		cout << " Time any function \n";
		int k = 0;
		for (list<NonlinearEquation>::iterator i = listFunction.begin(); i != listFunction.end(); i++, k++)
		{
			cout << " func " << k << " : " << "EndTime " << i->getEndTime() <<" StarTime " << i->getStartTime() 
				<< " Time calc: " << i->getEndTime() - i->getStartTime() << endl;
		}
		cin.get();
		int choose = 0;
		while (true)
		{
			cout << " Choose: " << endl;
			cout << "1. Show calculate your function" << endl;
			cout << "2. Begin new calculations (with save old) " << endl;
			cout << "3. Begin new calculations (without old) " << endl;
			cout << "4. Exit " << endl;
			cin >> choose;
			switch (choose)
			{
			case 1:
			{
				int numFunction = 0;
				cout << " input num function : ";
				while (true)
				{
					cin >> numFunction;
					if (numFunction > listFunction.size()-1)
						cout << "It's function does't exist! New input \n";
					else
						break;
					if (numFunction < 0)
						break;
				}
				if (numFunction < 0)
				{
					choose = 4;
					break;
				}
				int k = 0;
				for (list<NonlinearEquation>::iterator i = listFunction.begin(); i != listFunction.end(); i++, k++)
				{
					if (k == numFunction)
					{		
						cout << " x : " << i->getX() << " g(x) : " << i->getfunctionFromX();
						cout << "\n StartTIme : " << i->getStartTime() << " EndTime : " << i->getEndTime();
						cout << " \nDIff time : " << i->getEndTime() - i->getStartTime() << endl;
						break;
					}
				}

			}
			break;
			case 2:
				break;
	
			case 3:
				listFunction.clear();
				break;
			case 4:
				break;
			default:
				break;
			}
			if (choose == 2 || choose == 3 || choose == 4)
				break;
		}
		
		if (choose == 4)
			break;
		
	}
	listFunction.clear();
	cout << " Good Bye!!";
	cin.get();
	return 0;
}

