#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>


constexpr auto PI = 3.14;
#define N 15

using namespace std;

void function(double*, double*);
double functionL_AnyX(double*, double*);
double l(double*, double&, int&, double&);
double functionL_chebyshevs_X(double*, double*);
double chebyshevs_X(int&);
double chebyshevs_X_For_a_b(int&, double&, double&);

int main()
{	
		/*FILE* gp = _popen("C:\\LibraryC++\\gnuplot\\bin\\wgnuplot -persist", "w");
	if (!gp)
		return -1;
	fprintf(gp, "set terminal win\n");
	fprintf(gp, "plot x*x\n");
	fprintf(gp, "pause -1 \"hit enter to exit\"\n");
	fprintf(gp, "pause -1");
	fprintf(gp, "exit\n");
	_pclose(gp);*/



	setlocale(LC_ALL, "Rus");
	ifstream file("pointX.txt");

	double* x = new double[N] {0};
	double* y = new double[N] {0};
	if (file)
	{
		while (!file.eof())
		{
			for(int i=0;i<N;i++)
				file >> x[i];
		}
		
	}
	else
	{
		cout << " Файл пуст "; 
		return 0;
	}
	function(x, y);
	file.close();

	ofstream file_data("data.txt");
	for (int i = 0; i < N; i++)
		file_data << x[i] << " " << y[i] << endl;
	file_data.close();

	cout << " Output : x and y \n";
	for (int i = 0; i < N; i++)
	{
		cout << " x: " << x[i] << " y: " << y[i] << endl;
	}

	cout << " Выберите способ:";
	cout << endl << "1. С любым х" << endl << "2. С узлами " << endl;
	short int chebyshev = 0;
	cin >> chebyshev;
	if (chebyshev == 1)
	{
		
		double L = functionL_AnyX(x, y);
		cout << " L(X) = " << L << endl;
		double sum = 0;
		for (int i = 0; i < N; i++)
		{
			sum += abs((L - y[i]));
		}
		cout << sum;
	}
	else
	{
		double L = functionL_chebyshevs_X(x, y);
		cout << " L(X) = " << L << endl;
		
	}

	system("gnuplot.bat");
	delete[] x;
	delete[] y;
	return 0;
}

//узлы чебышева более точны

void function(double *x,double* y)
{
	for (int i = 0; i < N; i++)
	{
		y[i] = exp(-0.5 * x[i]) * sin(3.2 * x[i]); 
	}
}

double functionL_AnyX(double* vector_X, double *y)
{
	ofstream lagrange("result.txt");
	ofstream lagrangePoint("pointlagrange.txt");
	double sum = 0;
	double x = 0;
	cout << " Input x: ";
	cin >> x;
	double checksum = 0;
	for (int i = 0; i < N; i++)
	{
		sum += y[i] * l(vector_X, x, i, vector_X[i]);
		checksum += abs(sum - exp(-0.5 * x) * sin(3.2 * x));
		lagrange << x << " " << sum <<  " " << checksum << endl;
		cout << " Input x for i = " << i  << " : " ;
		cin >> x;
	}

	cout << " Check : " << checksum << endl;
	lagrangePoint.close();
	lagrange.close();
	return sum;
}


double functionL_chebyshevs_X(double* vector_X, double* y)
{
	ofstream lagrange("result.txt");
	ofstream lagrangePoint("pointlagrange.txt");
	double sum = 0;
	double x = 0;
	cout << "С равностоящими узлами [-1,1](1) или с узлами Чебышева(0)? ";
	bool segment = false;
	cin >> segment;
	double checksum = 0;
	double xk = 0;
	if (segment == true)
	{
		for (int k = 0; k < N; k++)
		{
			xk = chebyshevs_X(k);
			for (int i = 0; i < N; i++)
			{
				sum += y[i] * l(vector_X, xk, i, vector_X[i]);
			}
			checksum += abs(sum - exp(-0.5 * xk) * sin(3.2 * xk));
			lagrangePoint << xk << " " << sum << endl;
			lagrange << xk << " " << sum << " " << checksum << endl;
		}	
	}
	else
	{
		cout << " input a :"; //0.2 3.14
		double a; cin >> a;
		cout << " input b :";
		double b; cin >> b;

		for (int k = 0; k < N; k++)
		{
			xk = chebyshevs_X_For_a_b(k, a, b);
			for (int i = 0; i < N; i++)
			{

				sum += y[i] * l(vector_X, xk, i, vector_X[i]);

			}
			checksum += abs(sum - exp(-0.5 * xk) * sin(3.2 * xk));
			lagrangePoint << xk << " " << sum << endl;
			lagrange << xk << " " << sum << " " << checksum << endl;
		}
	}
	lagrangePoint.close();
	lagrange.close();
	cout << " Check : " << checksum << endl;
	return sum;
}

double l(double* vector_X, double& x, int& i, double& xi)
{
	double p = 1;
	for (int j = 0; j < N; j++)
	{
		if (i != j)
			p *= (x - vector_X[j]) / (xi - vector_X[j]);
	}
	return p;
}

double chebyshevs_X(int &k)
{
	double xk = 0;
	return xk = cos((2. * (k+1.) - 1.) / (2. * N) * PI); //[-1.1];
}

double chebyshevs_X_For_a_b(int &k, double&a, double&b)
{
	 double xk = 0;
	 xk = 0.5*(a+b)+(0.5*(b-a)*cos((2.*(k + 1.)-1. )/ (2.*N) * PI)); // точки нужны чтобы не округлять
	 return xk;
}

