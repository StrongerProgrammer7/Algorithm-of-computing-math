// Eigenvalues.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#define SIZEMATRIX 3
#define EPS 0.01 //0.0001 дает более точное приближение6,895
using namespace std;

double* multimatrix_number(double** matrixA, double* matrix_vector);
double* copyvector(double* vector1);
int main()
{
	setlocale(LC_ALL, "RU");
	double** matrixA = new double* [SIZEMATRIX];
	for (int i = 0; i < SIZEMATRIX; i++)
		matrixA[i] = new double[SIZEMATRIX] {};
	double* matrixX0 = new double[SIZEMATRIX];
	
	cout << " Ввод осущствеляется по принципу: \nввод коэффициентов для определенной строки через пробел , "
		"\nих количество равно количеству указанных столбцов.\n "
		"(допускается: '+' ',' '-' '[0-9]' \n";
	cout << " Ввод матрицы А" << endl;
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
			cin >> matrixA[i][j];// 5 1 2 / 1 4 1/ 2 1 3
	}
	cout << " Ввод вектора (начальное приближение)" << endl;
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		cin >> matrixX0[i]; //1 1 1
	}

	cout << "Output matrix" << endl;

	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
		{
			cout << " " << matrixA[i][j] << " ";
		}
		cout << " = " << matrixX0[i] << endl;
	}
	double* matrixX1 = new double[SIZEMATRIX] {};
	matrixX1 = multimatrix_number(matrixA, matrixX0);
	cout << " Output matrixX1 :" << endl;
	for (int i = 0; i < SIZEMATRIX; i++)
		cout << matrixX1[i] << " ";

	double* matrixL = new double[SIZEMATRIX] {};
	cout << " Output matrixL :" << endl;
	matrixL[1] = matrixX1[0] / matrixX0[0];
	cout << "лямбад0 = " << matrixL[1] << endl;

	int k = 1;
	double* matrixX2 = new double[SIZEMATRIX] {};
	do
	{
		k++;
		matrixX2 = multimatrix_number(matrixA, matrixX1);
		cout << " Output matrixX2 :" << endl;
		for (int i = 0; i < SIZEMATRIX; i++)
			cout << matrixX2[i] << " ";

		matrixL[k] = matrixX2[0] / matrixX1[0];
		cout << "лямбад" << k << " = " << matrixL[k] << endl;

		matrixX1 = copyvector(matrixX2);

	} while (abs(matrixL[k]-matrixL[k-1])>EPS);

	cout << " Lambda = " << matrixL[k] << endl;

	for (int i = 0; i < SIZEMATRIX; i++)
		delete[] matrixA[i];
	delete[] matrixA;
	delete[] matrixX0;
	delete[] matrixX2;
	//delete[] matrixL;
}

double* multimatrix_number(double** matrixA, double* matrix_vector)
{
	double* resultMatrix = new double[SIZEMATRIX]{};
	double sum = 0;
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
		{
			resultMatrix[i] += matrixA[i][j] * matrix_vector[j];
		}
	}
	return resultMatrix;
}

double* copyvector(double* vector1)
{
	double* result_vector = new double[SIZEMATRIX] {};
	for (int i = 0; i < SIZEMATRIX; i++)
		result_vector[i] = vector1[i];
	return result_vector;
}


//k=2 58 38 40 l=7.25 0.75
//k=3 408 250 274 l=7.034 0.116
//k=4 2838 1682 1888 l=6.9559 0.078<e