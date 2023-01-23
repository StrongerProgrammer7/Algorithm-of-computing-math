// MethodSquare.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <conio.h>
#include <fstream>
#include<iomanip>
#include <malloc.h>
#include <cmath>
#include <vector>
#include <omp.h>
#define SIZEMATRIX 2000
#define MAX_SIZE 16384
#define EPSILON 0.001

using namespace std;

//МНК
double* MLS(double* x, double* y, int n, int k);
void forwardStroke(int n, double** a, double* b);
void reverseStroke(int n, double** a, double* b, double* x);
//Сред квадртач отклонение
double summSquareDeviation(double*, double*, double*, int, int);
double standartDeviation(double, int);
double standartDeviationStable(double Q, int size, int k);

bool stable_or_up_Sigma(vector<double>);
double roundNumFive(double);

void outPut(double**, double*, int);
void freeArray(double*, double**, int);



int main()
{
	//double* vector_numberA = new double[SIZEMATRIX] {};
	//double* vector_valueFunction = new double[SIZEMATRIX] {};

	setlocale(LC_ALL, "RU"); //В файле считывает , все что после . является 0
	double* x = new double[SIZEMATRIX];
	double* y = new double[SIZEMATRIX];

	ifstream file_numbers("Value.txt");

	//count value (string)
	char buf[256];
	int size = 0;
	while (file_numbers.getline(buf, 256))
		size++;

	file_numbers.clear();
	file_numbers.seekg(0);//cursor in begin

	while (!file_numbers.eof())
	{
		for (int i = 0; i < size+1; i++)
		{
			if (!file_numbers.eof())
			{
				string number = "";
				file_numbers >> number;
				char const* c[] = { number.c_str() };
				x[i] = atof(*c);
				number = "";
				file_numbers >> number;
				//char const* c2[] = { number.c_str() };
				y[i]=atof(*c);
			}
			else
				break;	
		}
	}


	file_numbers.close();

	int degreePolynom;
	cout << "Введите начальную степень полинома k= ";
	cin >> degreePolynom;

	double* coefficient;
	omp_set_num_threads(3); //3 0.014 1 0.028
	double st = omp_get_wtime();
	vector <double> sigma;// для нахождения оптимальной степени полинома
	do
	{
		coefficient = MLS(x, y, size, degreePolynom);
		sigma.push_back(standartDeviationStable(summSquareDeviation(x, y, coefficient, size, degreePolynom), size, degreePolynom));
		if (sigma.back() > EPSILON)
		{
			bool optimatDegree = false;
			if (sigma.size() >= 2)
				optimatDegree = stable_or_up_Sigma(sigma);
			if (optimatDegree == true)
				break;
			cout << " Увеличиваем степень на 1 ";
			degreePolynom++;
			cout << " K = " << degreePolynom << endl;
		}
		else
		{
			break;
		}	
		cin.get();
		
	} while (true);
	 
	

	double* F = new double[SIZEMATRIX] {};
	for (int i = 0; i < size; i++)
	{
		double sum = 0;
		int j = 0;
#pragma omp parallel private(j)
		{
#pragma omp for reduction(+:sum)
			for (j = 0; j < degreePolynom; j++)
			{
				sum += coefficient[j] * pow(x[i], j);
			}
		}
		
		F[i] = y[i]-sum;
	}
	double end = omp_get_wtime();
	cout << " TIme :" << end - st << endl;

	ofstream recordInFile("Result vector.txt", ios::app);
	recordInFile << " Приближенные значения : " << endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 10; j++, i++)
			recordInFile << fixed << setprecision(6) << F[i] << "   |   ";
		recordInFile << endl;
	}
	recordInFile.close();
	ofstream recordCoef("Coef.txt");
	for (int i = 0; i < degreePolynom; i++)
		recordCoef << fixed << setprecision(6) << coefficient[i] << endl;
	recordCoef.close();
	freeArray(coefficient, NULL, degreePolynom);
	freeArray(x, NULL, size);
	freeArray(y, NULL, size);
}


double* MLS(double* x, double* y, int n, int k)
{
	double** a = new double* [k];
	double* b = new double[k];
	double* c = new double[k];
	for (int m = 0; m < k; m++)
	{
		a[m] = new double[k];
		for (int j = 0; j < k; j++)
		{
			a[m][j] = 0;
			double s = 0;
			int i = 0;
#pragma omp parallel private(i)
			{
#pragma omp for reduction(+:s)
				for (i = 0; i < n; i++)
					s += pow(x[i], m + j);
			}
			a[m][j] = s;
			
		}
		
		
		b[m] = 0;
		double s = 0;
		int i = 0;
#pragma omp parallel private(i)
		{
#pragma omp for reduction(+:s)
			for (i = 0; i < n; i++)
			{
				s += pow(x[i], m) * y[i];
			}
		}
		b[m] = s;
		
	}
	forwardStroke(k, a, b);
	reverseStroke(k, a, b, c);
	freeArray(b, a, k);
	return c;
}
void forwardStroke(int n, double** a, double* b)
{
	
	for (int k = 0; k < n - 1; k++)//n
	{
		double v = 0;
		int im = k;
		for (int i = k + 1; i < n; i++)
		{
			if (fabs(a[im][k]) < fabs(a[i][k]))
			{
				im = i;
			}
		}
		if (im != k)
		{
			for (int j = 0; j < n; j++)
			{
				swap(a[im][j], a[k][j]);
				/*v = a[im][j];
				a[im][j] = a[k][j];
				a[k][j] = v;*/
			}
			swap(b[im], b[k]);
			/*v = b[im];
			b[im] = b[k];
			b[k] = v;*/
		}
		for (int i = k + 1; i < n; i++)
		{
			v = a[i][k] / a[k][k];
			a[i][k] = 0;
			b[i] = b[i] - v * b[k];
			for (int j = k + 1; j < n; j++)
			{
				a[i][j] -= v * a[k][j];
			}
				
		}
	}
}
void reverseStroke(int n, double** a, double* b, double* x)
{
	x[n - 1] = b[n - 1] / a[n - 1][n - 1];
	for (int i = n - 2; i>=0 ; i--)
	{
		double s = 0;
		int j = 0;
#pragma omp parallel private(j)
		{
#pragma omp for reduction(+:s)
			for (j = i + 1; j < n; j++)
			{
				s += a[i][j] * x[j];
			}
		}
		
		x[i] = (b[i] - s) / a[i][i];
	}
	
}


double standartDeviation(double Q,int size) // Общее среднее квадратичное отклонение
{
	const __int64 y = static_cast<__int64>(size) + 1;
	return (double)sqrt(Q /y); 
}

double standartDeviationStable(double Q, int size, int k) //Нахождение оптимальной степени полинома
{
	const __int64 y = static_cast<__int64>(size) - k;
	return (double)sqrt(Q / y);
}

double summSquareDeviation(double* x, double* y, double* coefficient,int size, int countCoefficient)
{
	double Q = 0;
	for (int i = 0; i < size; i++)
	{
		double sum = 0;
		int j = 0;
#pragma omp parallel private(j)
		{
#pragma omp for reduction(+:sum)
			for (j = 0; j < countCoefficient; j++)
			{
				sum += coefficient[j] * pow(x[i], j);
			}
		}
		
		Q += pow(sum-y[i], 2); // в нек учебниках  y[i]-sum
	}
	return Q;
}

void freeArray(double* vector, double** matrix,int size)
{
	if (matrix != NULL)
	{
		for (int i = 0; i < size; i++)
			delete[] matrix[i];
		delete[] matrix;
	}
	if(vector!=NULL)
		delete[] vector;
}

void outPut(double** matrixA, double* matrixB,int size)
{
	for (int i = 0; i < size; i++)
	{
		if (matrixA != NULL)
		{
			for (int j = 0; j < size; j++)
			{
				cout << matrixA[i][j] << "\t";
				//cout << " " <<  matrixA[i][j] << setprecision(3) << setw(5);
			}
		}
		if (matrixB != NULL)
			cout << setw(4) << "|" << matrixB[i] << endl;
		else
			cout << endl;
	}
}

bool stable_or_up_Sigma(vector<double> sigma)
{
	//Stable
	for (int j = 0; j < sigma.size(); j++)
	{
		double cur = roundNumFive(sigma[j]);
		for (int k = j + 1; k < sigma.size(); k++)
		{
			double tmp = roundNumFive(sigma[k]);
			if (cur == tmp)
			{
				return true;
			}
		}

	}

	//Up
	bool flag = false;
	for (int j = 0; j < sigma.size()-1; j++)
	{
		//cout << ":: " << sigma[j] << endl << " :: " << sigma[j + 1] << endl;
		if (sigma[j] <= sigma[j + 1])
		{
			flag = true;
		}
		else
		{
			flag = false;
		}
	}
	if (flag == true)
		return true;
	else
		return false;
}

double roundNumFive(double num)
{
	return num<0? round(-num * 100000) / 100000: round(num * 100000) / 100000;
}


//Источники
//http://www.machinelearning.ru/wiki/index.php?title=Проведение_поверхностей_наилучшего_приближения
//https://ru.stackoverflow.com/questions/383716/МНК-алгоритм
//https://www.programmersforum.ru/showthread.php?t=207160