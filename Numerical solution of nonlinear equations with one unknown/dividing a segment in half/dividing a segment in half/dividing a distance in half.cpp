// dividing a segment in half.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double function(double);

int main()
{
	setlocale(LC_ALL, "Rus");
	//зададим отрезок
	double a, b;
	cout << " Write a: "; cin >> a;
	cout << " Write b: "; cin >> b;
	cout << " Distance: [" << a << " , " << b << "]" << endl;
	double x = 0;
	int iter = 0;
	do
	{
		x = (a + b) / 2;
		if (function(x) == 0)
			break;
		else
			if (function(a) * function(x) < 0)
				b = x;
			else
				if (function(b) * function(x) < 0)
					a = x;
		cout << " Iteration " << iter << " : " << x << endl;
		iter++;
	} while (abs(b - a) > DBL_EPSILON);
	x *= (a + b) / 2;
	cout << " root x = :" << x  << setprecision(5) << "; f(x) = " << function(x) << setprecision(5) << "; count iteration : " << iter << endl;
	return 0;
}

double function(double x)
{
	return x*x*x-3*x*x+9*x-8;
}

//x-sin(x)-1