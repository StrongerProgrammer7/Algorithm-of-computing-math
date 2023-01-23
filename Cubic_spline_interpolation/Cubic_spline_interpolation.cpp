// Cubic_spline_interpolation.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>

#define COUNT_POINT 6
#define	PI 3.1415926
using namespace std;

double function(double);
double* divorceSLAU_iteration(double** matrix, double* y);
double** reverseMatrix(double** matrix);
double* compostionMatrix(double** matrix, double* matrix_vector);
void vectorC(double* vectorB, double maindiag[], double downdiag[], double updiag[], double(&c)[COUNT_POINT]);

int main()
{
	cout << " Input x:\n";
	double x[COUNT_POINT];// from 0
	for (int i = 0; i < COUNT_POINT; i++)
	{
		cout << " x" << i << " = "; cin >> x[i];
	}

	cout << " function(X) : " << endl;
	double y[COUNT_POINT]; // from 0
	y[0] = 2;
	y[1] = 3;
	y[2] = 1.5;
	y[3] = 1;
	y[4] =2;
	y[5] = 0.5;
	for (int i = 0; i < COUNT_POINT; i++)
	{
		//y[i] = function(x[i]);
		cout << "F(x" << i << ") = " << y[i] << endl;
	}

	ofstream file_data("data.txt");
	for (int i = 0; i < COUNT_POINT; i++)
		file_data << x[i] << " " << y[i] << endl;
	file_data.close();

	double h[COUNT_POINT];//from 1
	for (int i = 1; i < COUNT_POINT; i++)
	{
		h[i-1] =  x[i] - x[i-1];
	}

	double a[COUNT_POINT]{};
	a[0] = y[0];
	for (int i = 1; i < COUNT_POINT; i++)
		a[i] = y[i];

	double maindiag[COUNT_POINT]{};
	double up_maindiag[COUNT_POINT]{};
	double down_maindiag[COUNT_POINT]{};
	double* vectorB = new double[COUNT_POINT+1] {};
	for (unsigned int i = 1; i < COUNT_POINT - 1; i++)
	{
		maindiag[i] = 2.0 * (h[i-1] + h[i]);
		down_maindiag[i] = h[i];
		up_maindiag[i] = h[i];
		vectorB[i] = 6.0 * (y[i + 1] - 2.0 * y[i] + y[i - 1]);
	}
	double c[COUNT_POINT];
	vectorC(vectorB, maindiag, up_maindiag, down_maindiag,c);
	for (int i = 1; i < COUNT_POINT - 1; i++)
		c[i] = round(c[i] * 10000.0) / 10000.0;
		
	double d[COUNT_POINT]{}; d[0] = 0;
	for (unsigned int i = 1; i < COUNT_POINT; i++)
		d[i] = c[i] - c[i - 1];

	double b[COUNT_POINT]{}; b[0] = 0;
	for (int i = 1; i < COUNT_POINT; i++)
		b[i] = (c[i] / 2) - (d[i] / 6) + y[i] - y[i - 1];
 

	cout << " c : \n";
	for (int i = 0; i < COUNT_POINT-1; i++)
	{
		cout << " c[" << i << "] = " << c[i] << " \n";
	}

	cout << " b : \n";
	for (int i = 1; i < COUNT_POINT; i++)
	{
		cout << " b[" << i << "] = " << b[i] << " \n";
	}

	cout << " d : \n";
	for (int i = 1; i < COUNT_POINT; i++)
	{
		cout << " d[" << i << "] = " << d[i] << " \n";
	}

	double cubicSpline[COUNT_POINT];
	double t[COUNT_POINT];
	cout << " t [x0,xn] :\n";
	for (int i = 0; i < COUNT_POINT; i++)
	{
		cout << "t[" << i << "] = "; cin >> t[i];
	}
	cubicSpline[0] = a[0];
	for (int i = 0; i < COUNT_POINT; i++)
		cubicSpline[i] = a[i] + (b[i]*(t[i]-x[i])) + (c[i]/2) * (t[i]-x[i]) * (t[i]-x[i]) + (d[i]/6) * (t[i]-x[i]) * (t[i]-x[i]) * (t[i]-x[i]);

	ofstream filecubic("cubicSpline.txt");
	cout << "output splint[t,i] " << endl;
	for (int i = 0; i < COUNT_POINT; i++)
	{
		cout << "spline(" << t[i] << "," << x[i] << ") = " << cubicSpline[i] << endl;
		filecubic << x[i] << " " << cubicSpline[i] << endl;
	}
	filecubic.close();

	system("gnuplot.bat");
	delete[] vectorB;
}


double function(double x)
{
	return sin(4 * x);//*PI/180);
}


void vectorC(double* vectorB, double maindiag[], double downdiag[], double updiag[],double (&c)[COUNT_POINT])
{
	double** matrixA = new double* [COUNT_POINT];
	for (int i = 0; i < COUNT_POINT; i++)
		matrixA[i] = new double[COUNT_POINT];

	for (int i = 1; i < COUNT_POINT - 1; i++)
		matrixA[i][i] = maindiag[i];

	for (int i = 1; i < COUNT_POINT - 1; i++)
	{
		for (int j = 1; j < COUNT_POINT - 1; j++)
		{
			if (i < j && j <= i + 1)
				matrixA[i][j] = updiag[i];
			else
				if (i != j && i<j)
					matrixA[i][j] = 0;
			if (i > j && j + 1 == i)
				matrixA[i][j] = downdiag[i];
			else
				if (i != j && i > j)
					matrixA[i][j] = 0;
		}
	}

	for (int i = 1; i < COUNT_POINT-1; i++)
	{
		for (int j = 1; j < COUNT_POINT-1; j++)
			cout << matrixA[i][j] << " ";
		cout << endl;
	}

	matrixA = reverseMatrix(matrixA);
	vectorB = compostionMatrix(matrixA, vectorB);

	for (int i = 1; i < COUNT_POINT - 1; i++)
		c[i] = vectorB[i];
	c[0] = 0;
	c[COUNT_POINT-1] = 0;

	
	for (int i = 0; i < COUNT_POINT; i++)
		delete[]matrixA[i];
	delete[] matrixA;
}
double* compostionMatrix(double** matrix, double* matrix_vector)//умножение матрицы на вектор столбец
{
	double* resultMatrix = new double[COUNT_POINT] {};// результативная матрица
	double sum = 0;
	for (int i = 1; i < COUNT_POINT-1; i++)
	{
		for (int j = 1; j < COUNT_POINT-1; j++)
		{
			resultMatrix[i] += matrix[i][j] * matrix_vector[j];
		}
	}
	return resultMatrix;
}



double** reverseMatrix(double** matrix)
{
	double** resultMatrix = new double* [COUNT_POINT];
	for (int i = 0; i < COUNT_POINT; i++)
		resultMatrix[i] = new double[COUNT_POINT] {};
	double* y = new double[COUNT_POINT] {}; // для столбца свободных слагаемых
	double* itr; //для хранение реш СЛАУ методом итераций (т.е. находим i-й стб обратной матрицы)

	for (int i = 1; i < COUNT_POINT-1; i++)
	{
		//заполнение столбца свободных слагаемых
		for (int j = 1; j < COUNT_POINT-1; j++)
		{
			if (i == j) //элемент главной диаг
				y[j] = 1;
			else
				y[j] = 0;
		}

		itr = divorceSLAU_iteration(matrix, y); //получаем i-й стб обратной матрицы
		for (unsigned int k = 1; k < COUNT_POINT-1; k++)
			resultMatrix[k][i] = itr[k];
	}

	//FIXME:если округлить то вызовет искажение , обратную матрицу нельзя округлять

	return resultMatrix;

}

double* divorceSLAU_iteration(double** matrix, double* y)
{
	//x1(k+1)=a1(n+1)/a11 (из вектора нач прибл) - a12/a11*x2(k)-..- где в роли k Для x2 выступает начальное приближ, x1(k+1)=Xn
	double* resultvector = new double[COUNT_POINT]; // результирующий массив
	for (int i = 0; i < COUNT_POINT-1; i++)
		resultvector[i] = y[i] / matrix[i][i];// начальное приближение, т.е. массив = свободный слаг/диаг элемент

	double eps = 0.000001;
	double* Xn = new double[COUNT_POINT] {};
	do
	{
		for (int i = 1; i < COUNT_POINT-1; i++)
		{
			Xn[i] = y[i] / matrix[i][i]; // следующее приближение
			for (int j = 1; j < COUNT_POINT-1; j++)
			{
				if (i == j) //т.к. будет диаг элемент который равен 0
					continue;
				else
					Xn[i]/*a1(n+1)/a11*/ -= matrix[i][j] / matrix[i][i] * resultvector[j];
			}
		}

		bool flag = true; //предполагаем что уже нашли
		for (int i = 1; i < COUNT_POINT- 1; i++)//-1
		{
			if (abs(Xn[i] - resultvector[i]) > eps) //если вып то продолжаем итерацию while
			{
				flag = false;
				break;
			}
		}

		for (int i = 1; i < COUNT_POINT-1; i++)
			resultvector[i] = Xn[i]; //храним предыдущее значение

		if (flag) //итерация закончена
			break;
	} while (1);
	/*for (int i = 1; i < COUNT_POINT - 1; i++)
		cout << " " << resultvector[i] << " ";*/

	return resultvector;
}

