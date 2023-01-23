
#include <iostream>
#define SIZE 3

using namespace std;

double* subtractionVector(double* vector1, double* vector2);
double norm(double* vector1, double* vector2);
void print_matrix_vector(double** matrix, double* vector);
double* copy_Vector(double* vector1);


///5 -1 3 5
///1 -4 2 20
///2 -1 5 10
///x1 = -0.7353 x2= -4.4844 x3=1.3972
int main()
{
	setlocale(LC_ALL, "russian");
	//Известно---------------------
	cout << " Input matrix A: SIZE 3x3 \n";
	double** matrixA = new double* [SIZE];
	for (int i = 0; i < SIZE; i++)
		matrixA[i] = new double[SIZE] {0};
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cin >> matrixA[i][j];
		}
	}
	double* vectorb = new double[SIZE] {0};
	cout << " Input vector b: SIZE 1x3\n";
	for (int i = 0; i < SIZE; i++)
		cin >> vectorb[i];
	//-----------------------------------
	//Начальное приближение-------------
	double* vectorX0 = new double[SIZE] {0};
	for (int i = 0; i < SIZE; i++)
	{
		vectorX0[i] = vectorb[i] / matrixA[i][i];
	}
	cout << " Output vector X0: ";
	print_matrix_vector(NULL, vectorX0);
	//-------------------------------------

	double* vectord = new double[SIZE] {0};
	for (int i = 0; i < SIZE; i++)
		vectord[i] = vectorb[i] / matrixA[i][i];
	
	double** matrixC = new double* [SIZE];
	for (int i = 0; i < SIZE; i++)
		matrixC[i] = new double[SIZE];
	
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (i != j)
			{
				matrixC[i][j] = (-1) * (matrixA[i][j] / matrixA[i][i]);
			}
			else
				matrixC[i][j] = 0;
		}
	}
	cout << " Output matrix C:\n ";
	print_matrix_vector(matrixC, NULL);

	double* vectorX1 = new double[SIZE] {0};

	int iteration = 0;
	double eps = 0.001;
	do
	{
		vectorX1 = copy_Vector(vectorX0);
		for (int i = 0; i < SIZE; i++)
		{
			double x = vectord[i];
			for (int j = 0; j < SIZE; j++)
			{
				x += matrixC[i][j] * vectorX0[j];
			}
			vectorX0[i] = x;
		}
		cout << " iteration : " << iteration << " X: ";
		print_matrix_vector(NULL, vectorX0); 
		if(norm(vectorX0, vectorX1) > eps)
			cout << "Точность вычислений " << norm(vectorX0, vectorX1) << " > " << eps << endl;
		else
			cout << "Точность вычислений " << norm(vectorX0, vectorX1) << " < " << eps << endl;
		
		iteration++;
		
	} while (norm(vectorX0, vectorX1) > eps);

	cout << " VECTOR X: ";
	print_matrix_vector(NULL, vectorX0);
	
	//Очищение памяти--------
	for (int i = 0; i < SIZE; i++)
	{
		delete[] matrixA[i];
		delete[] matrixC[i];
	}
	delete[] matrixC;
	delete[] matrixA;
	delete[] vectorb;
	delete[] vectord;
	delete[] vectorX0;
	delete[] vectorX1;
	//--------------------------
	return 0;
}

double* copy_Vector(double* vector1)
{
	double* result_Vector = new double[SIZE];
	for (int i = 0; i < SIZE; i++)
		result_Vector[i] = vector1[i];
	return result_Vector;
}


double* subtractionVector(double* vector1, double* vector2)
{
	double* resultVector = new double[SIZE] {0};
	for (int i = 0; i < SIZE; i++)
	{
		resultVector[i] = vector1[i] - vector2[i];
	}
	return resultVector;
}

double norm(double* vector1, double* vector2)
{
	double* resultVector = new double[SIZE] {0};
	resultVector = subtractionVector(vector1, vector2);
	double sum = 0.0;
	for (int i = 0; i < SIZE; i++)
	{
		resultVector[i] = resultVector[i] * resultVector[i];
		sum += resultVector[i];
	}

	return sqrt(sum);

}

void print_matrix_vector(double** matrix, double* vector)
{
	if (matrix != NULL)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}
	}
	else
		if (vector != NULL)
		{
			for (int i = 0; i < SIZE; i++)
				cout << vector[i] << " ";
		}
	cout << endl << endl;
}