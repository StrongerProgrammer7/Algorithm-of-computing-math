// Inverse_MethodGaussa.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#define SIZE 3
using namespace std;
int  INVERSE(double** a, double** y);
int SLAU(double** matrixA, double* vectorB, double* x);

int main()
{
	int result;
	double** A = new double*[SIZE];
	for (int i = 0; i < SIZE; i++)
		A[i] = new double[SIZE];
	double** B = new double*[SIZE];
		for (int i = 0; i < SIZE; i++)
			B[i] = new double[SIZE];
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				cin >> A[i][j];
		result = INVERSE(A, B);
		if (result == 0)
		{
			cout << "Inverse matrix" << endl;
			for (int i = 0; i < SIZE; cout << endl, i++)
				for (int j = 0; j < SIZE; j++)
					cout << B[i][j] << "\t";
		}
		else cout << "No Inverse matrix" << endl;
		for (int i = 0; i < SIZE; i++)
			delete[] A[i];
		delete[] A;
		for (int i = 0; i < SIZE; i++)
			delete[] B[i];
		delete[] B;

}

int SLAU(double** matrixA, double* vectorB,double* x)
{
	double c=0.0, M=0.0, max=matrixA[0][0], s=0.0, ** a, * b;
	a = new double* [SIZE];
	for(int i = 0; i < SIZE; i++)
		a[i] = new double[SIZE];

	b = new double[SIZE];
	//Copy
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			a[i][j] = matrixA[i][j];

	for (int i = 0; i < SIZE; i++)
		b[i] = vectorB[i];
	//--------------------

	int r=0;
	for (int k = 0; k < SIZE; k++)
	{
		//сначала берем элем по гл диаг как макс и запом позиц
		max = fabs(a[k][k]);
		r = k;
		//ищем макс элемент для k стб
		for (int i = k + 1; i < SIZE; i++)
		{
			if (fabs(a[i][k]) > max)
			{
				max = fabs(a[i][k]);
				r = i;
			}
		}
		//----------------------------
		//меняем местами строки(строку с найд макс элементом и текущей)
		for (int j = 0; j < SIZE; j++)
		{
			c = a[k][j];
			a[k][j] = a[r][j];
			a[r][j] = c;
		}
		c = b[k];
		b[k] = b[r];
		b[r] = c;
		//------------------------------
		for (int i = k + 1; i < SIZE; i++)
		{
			int j;
			for (M = a[i][k] / a[k][k],j = k; j < SIZE; j++)
			{
				
				a[i][j] -= M * a[k][j];
			}
				
			b[i] -= M * b[k];
		}
	}
	if (a[SIZE - 1][SIZE - 1] == 0)
		if (b[SIZE - 1] == 0)
			return -1;
		else
			return -2;
	else
	{
		int j = 0;
		for (int i = SIZE - 1; i >= 0; i--)
		{
			for ( s = 0, j = i + 1; j < SIZE; j++)
				s += a[i][j] * x[j];
			x[i] = (b[i] - s) / a[i][i];
		}
		return 0;
	}

	for (int i = 0; i < SIZE; i++)
		delete[] a[i];
	delete[]a;
	delete[]b;

}

int INVERSE(double** a, double** y)
{
	int res = 0;
	double* b = new double[SIZE];
	double* x = new double[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (j == i)
				b[j] = 1;
			else
				b[j] = 0;
			
		}
		res = SLAU(a, b, x);
		if (res != 0)
			break;
		else
			for (int j = 0; j < SIZE; j++)
				y[j][i] = x[j];

	}
	delete[]x;
	delete[]b;
	if (res != 0)
		return -1;
	else
		return 0;
}