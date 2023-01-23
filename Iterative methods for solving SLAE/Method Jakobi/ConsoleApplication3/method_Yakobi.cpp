// ConsoleApplication1.cpp : Defines the entry point for the console application.
//


#include <iostream>
#define SIZE 3

using namespace std;

double** reverseMatrix(double** matrix);
double* divorceSLAU_iteration(double** matrix, double* y);
double* compostionMatrix_Vector(double** matrix, double* matrix_vector);
double** compostionMatrix(double** matrix1, double** matrix2);//умножение матрицы на вектор столбец
double** subtractionMatrix(double** matrix1, double** matrix2);
double* subtractionVector(double* vector1, double* vector2);
double norm(double* vector1, double* vector2);
void print_matrix_vector(double** matrix, double* vector);
double* addition(double* vector1, double* vector2);

int main()
{
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
	cout << " Input vector X0: SIZE 1x3\n";
	for (int i = 0; i < SIZE; i++)
		cin >> vectorX0[i];
	//-------------------------------------

	double* vectorg = new double[SIZE] {0};
	// Диагональная матрица -------------------------
	double** matrixD = new double* [SIZE];
	for (int i = 0; i < SIZE; i++)
		matrixD[i] = new double[SIZE] {0};
	for (int i = 0; i < SIZE; i++)
		matrixD[i][i] = matrixA[i][i];

	cout << "Diagonal matrix: \n";
	print_matrix_vector(matrixD, NULL);
	double** matrixD_reverse = new double* [SIZE];
	for (int i = 0; i < SIZE; i++)
		matrixD_reverse[i] = new double[SIZE] {0};
	matrixD_reverse = reverseMatrix(matrixD);
	cout << "Reverse Diagonal  matrix: \n";
	print_matrix_vector(matrixD_reverse, NULL);
	//*------------------------------

	double** matrixB = new double* [SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		matrixB[i] = new double[SIZE];
	}
	matrixB = compostionMatrix(matrixD_reverse,subtractionMatrix(matrixD,matrixA));
	cout << "Matrix B: \n";
	print_matrix_vector(matrixB, NULL);

	vectorg = compostionMatrix_Vector(matrixD_reverse,vectorb);
	cout << "Vector g: ";
	print_matrix_vector(NULL, vectorg);

	vectorX0 = addition(compostionMatrix_Vector(matrixB, vectorX0),vectorg);
	cout << "Vector X0: ";
	print_matrix_vector(NULL, vectorX0);

	double* vectorX1 = new double[SIZE] {0};
	bool firstiteration_complete = false;
	double eps = 0.001;
	do
	{	
		if (firstiteration_complete == false)
		{
			vectorX1 = addition(compostionMatrix_Vector(matrixB, vectorX0), vectorg);
			cout << "Vector X1: ";
			print_matrix_vector(NULL, vectorX1);
		}
		else
		{
			for (int i = 0; i < SIZE; i++)
				vectorX0[i] = vectorX1[i];
			vectorX1 = addition(compostionMatrix_Vector(matrixB, vectorX0), vectorg);
			cout << "Vector X1: ";
			print_matrix_vector(NULL, vectorX1);

		}

		if (norm(vectorX1, vectorX0) > eps)
			cout << "Точность вычислений " << norm(vectorX1, vectorX0) << " > " << eps << endl;
		else
			cout << "Точность вычислений " << norm(vectorX1, vectorX0) << " < " << eps << endl;
		
	} while (norm(vectorX1,vectorX0)>eps);

	cout << " VECTOR X: ";
	for (int i = 0; i < SIZE; i++)
		cout << vectorX1[i] << " ";

	//Очищение памяти--------
	for (int i = 0; i < SIZE; i++)
	{
		delete[] matrixA[i];
		delete[] matrixD[i];
		delete[] matrixD_reverse[i];
		delete[] matrixB[i];
	}
	delete[] matrixA;
	delete[] matrixD;
	delete[] matrixD_reverse;
	delete[] matrixB;
	delete[] vectorX0;
	delete[] vectorX1;
	delete[] vectorb;
	delete[] vectorg;
	//--------------------------
	return 0;
}

double** reverseMatrix(double** matrix)
{
	double** resultMatrix = new double* [SIZE];
	for (int i = 0; i < SIZE; i++)
		resultMatrix[i] = new double[SIZE] {};
	double* y = new double[SIZE] {}; // для столбца свободных слагаемых
	double* itr; //для хранение реш СЛАУ методом итераций (т.е. находим i-й стб обратной матрицы)

	for (int i = 0; i < SIZE; i++)
	{
		//заполнение столбца свободных слагаемых
		for (int j = 0; j < SIZE; j++)
		{
			if (i == j) //элемент главной диаг
				y[j] = 1;
			else
				y[j] = 0;
		}

		itr = divorceSLAU_iteration(matrix, y); //получаем i-й стб обратной матрицы
		for (unsigned int k = 0; k < SIZE ; k++)
			resultMatrix[k][i] = itr[k];
	}

	//FIXME:если округлить то вызовет искажение , обратную матрицу нельзя округлять

	return resultMatrix;

}

double* divorceSLAU_iteration(double** matrix, double* y)
{
	//x1(k+1)=a1(n+1)/a11 (из вектора нач прибл) - a12/a11*x2(k)-..- где в роли k Для x2 выступает начальное приближ, x1(k+1)=Xn
	double* resultvector = new double[SIZE]; // результирующий массив
	for (int i = 0; i < SIZE; i++)
		resultvector[i] = y[i] / matrix[i][i];// начальное приближение, т.е. массив = свободный слаг/диаг элемент

	double eps =0.000001;
	double* Xn = new double[SIZE] {};
	do
	{
		for (int i = 1; i < SIZE ; i++)
		{
			Xn[i] = y[i] / matrix[i][i]; // следующее приближение
			for (int j = 1; j < SIZE ; j++)
			{
				if (i == j) //т.к. будет диаг элемент который равен 0
					continue;
				else
					Xn[i]/*a1(n+1)/a11*/ -= matrix[i][j] / matrix[i][i] * resultvector[j];
			}
		}

		bool flag = true; //предполагаем что уже нашли
		for (int i = 1; i < SIZE ; i++)//-1
		{
			if (abs(Xn[i] - resultvector[i]) > eps) //если вып то продолжаем итерацию while
			{
				flag = false;
				break;
			}
		}

		for (int i = 1; i < SIZE ; i++)
			resultvector[i] = Xn[i]; //храним предыдущее значение

		if (flag) //итерация закончена
			break;
	} while (1);
	/*for (int i = 1; i < SIZE - 1; i++)
	cout << " " << resultvector[i] << " ";*/

	return resultvector;
}

double** subtractionMatrix(double** matrix1, double** matrix2) // вычитание
{

	double** resultMatrix = new double* [SIZE];
	for (int i = 0; i < SIZE; i++)
		resultMatrix[i] = new double[SIZE] {};
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			resultMatrix[i][j] = matrix1[i][j] - matrix2[i][j];
		}
	}
	return resultMatrix;

}

double* compostionMatrix_Vector(double** matrix, double* matrix_vector)//умножение матрицы на вектор столбец
{
	double* resultMatrix = new double[SIZE] {};// результативная матрица
	double sum = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			resultMatrix[i] += matrix[i][j] * matrix_vector[j];
		}
	}
	return resultMatrix;
}

double** compostionMatrix(double** matrix1, double** matrix2)//умножение матрицы на вектор столбец
{
	double** resultMatrix = new double* [SIZE];
	for (int i = 0; i < SIZE; i++)
		resultMatrix[i] = new double[SIZE] {0};
	double sum = 0;
	for (int k = 0; k < SIZE; k++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				resultMatrix[k][i] += matrix1[k][j] * matrix2[j][i];
			}
		}
	}

	return resultMatrix;
}

double* addition(double* vector1, double* vector2)
{
	double* resultVector = new double[SIZE] {0};
	for (int i = 0; i < SIZE; i++)
	{
		resultVector[i] = vector1[i] + vector2[i];
	}
	return resultVector;
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
	}else
		if (vector != NULL)
		{
			for (int i = 0; i < SIZE; i++)
				cout << vector[i] << " ";
		}
	cout << endl << endl;
}