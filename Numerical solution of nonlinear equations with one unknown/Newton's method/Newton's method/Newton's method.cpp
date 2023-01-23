// Newton's method.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double function(double);
double derivative_first(double(double), double);
/*double derivative_second(double(double), double);*/

int main()
{
	//Distance
	double a = 0, b = 0;
	cout << " Input a: "; cin >> a;
	cout << " Input b: "; cin >> b;
	cout << " Distance: [" << a << " , " << b << "]" << endl;

	double x0 = 0.0; // начальное приближение
	cout << " Initial value x0 in [a,b] : "; cin >> x0;
	if (x0<a || x0>b)
	{
		while (x0 < a || x0 > b)
		{
			cout << " !!! x !>b | x!<a. Input x0 in [a,b] : ";
			cin >> x0;
		}
	}
	/*if (function(a) * derivative_second(function, a) > 0 || function(a) * derivative_second(function, a) < 0)
		x0 = a;
	else
		if (function(b) * derivative_second(function, b) > 0 || function(b) * derivative_second(function, b) < 0)
			x0 = b;
	if (x0 == a)
		cout << " x0 = a: " << x0 << endl;
	else
		if(x0==b)
			cout << " x0 = b: " << x0 << endl;
		else
		{
			cout << " x0 = inf";
			return 0;
		}*/

	//y=f(x0)+f'(x0)(x-x0)
	double x = 0;
	int iter = 0;
	cout << " Epsilon : " << DBL_EPSILON << endl;
	for(;;)
	{
		double F = function(x0);
		double F1 = derivative_first(function,x0);
		x = x0 - F / F1;
		cout << "Iteration : " << iter << " F = " << F << setprecision(5) << ";" << setprecision(5) <<" F1 = " << F1 << ";" << setprecision(5) << " x = " << x << endl;

		if (abs(x - x0) < DBL_EPSILON)
			break;
		else
			x0 = x;
		
		iter++;
			
		if (iter > 1000)
		{
			cout << " Count iteration very much. The initial value(x0) is not selected correctly. Return programm...";
			return 0;
		}
	} 

	
	cout << endl;
	cout << " Count iteration : " << iter << "; root x = " << x << setprecision(5) << "; f(x) = " << function(x) << setprecision(5);
	cout << endl;
	return 0;
}

double function(double x)
{
	return x*x*x+3*x*x-3; //[0.5,1.5]
}

double derivative_first(double func(double), double x)
{
	const double dx = 1e-10;
	return (function(x + dx) - function(x)) / dx;
}

/*double derivative_second(double func(double), double x)
{
	const double dx = 1e-10;
	return ((function(x + dx) - function((x + dx) + dx)) / ((x + dx) - ((x + dx) + dx)) - (function(x) - function(x + dx)) / (x - (x + dx)))/dx;
}

/*double derivative_second(double derivative_first(double,double), double x)
{
	const double dx = 1e-10;
	double y = x + dx;
	return (derivative_first(function(x),(x + dx)) - derivative_first(function(x), x)) / dx;
}*/
