// MethodHoleckogo.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <iostream>
#include <fstream>
#include <iomanip>
#include<cmath>
#include <windows.h>
#include <stddef.h>
#define SIZEMATRIX  3

using namespace std;

void outPut(double**, double*);
double sum_spi_dp(int , double**, double**);
double sum_spi_dp_spj(int, int, double**, double**);
//double** compostionMatrix(double** , double** );
double* compostionMatrix(double** , double* );
double** reverseMatrix(double**);
double* divorceSLAU_iteration(double**, double*);
void roundMatrix(double** );
void roundVector(double* );
int sign(double);
void nrerror(char[]); //Numerical Recipes standard error handler
void checkMatrix(double**);

int main()
{
	setlocale(LC_ALL,"RU");

	//С файла запись в двумерный и одномерный массив
	ifstream file_numbers("Digit.txt");

	double** matrixA = new double* [SIZEMATRIX] ; //  массив содержащий в каждой строке указатель на другие массивы содержащие 5 колонн
	for (int i = 0; i < SIZEMATRIX; i++)
		matrixA[i] = new double[SIZEMATRIX] {}; // создаем массивы и инициализурем нулями на которые ссылается указатель
	double* matrixB = new double[SIZEMATRIX] {};

	//считывание с файла
	while (!file_numbers.eof())
	{
		for (int i = 0; i < SIZEMATRIX * SIZEMATRIX; i++)
			file_numbers >> matrixA[i / SIZEMATRIX][i % SIZEMATRIX];

		for (int i = 0; i < SIZEMATRIX; i++)
			file_numbers >> matrixB[i % SIZEMATRIX];
	}
	file_numbers.close();
	outPut(matrixA, matrixB);

	//проверка на симметричность
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
		{
			if (matrixA[i][j] != matrixA[j][i])
			{
				cout << " Матрица не симметрична.Решение не возможно.";
				return 0;
			}

		}
	}
	cout << endl << " Матрица симметрична" << endl;

	//void nrerror(char error_text[]);
	checkMatrix(matrixA);  // проверка положительно ли опредленная матрица

	//инициализация матриц
	double** matrixS = new double* [SIZEMATRIX]; //Основная матрица(на паре D здесь S)
	double** matrixD = new double* [SIZEMATRIX]; //Диагональная матрица
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		matrixS[i] = new double[SIZEMATRIX] {};
		matrixD[i] = new double[SIZEMATRIX] {};
	}
		 
	 
	//задаем значение диагональной матрице 1 если + и -1 если -
	matrixD[0][0] = sign(matrixA[0][0]);
	//задает значение матрице S
	matrixS[0][0] = sqrt(fabs(matrixA[0][0]));
	//roundMatrix(matrixS);
	cout << "\n s[0][0] : " << matrixS[0][0] << endl << endl;
	//A=S(t)*D*S,  S(t)Dy=b => Sx=y
	for (int j = 1; j < SIZEMATRIX; j++)
	{
		matrixS[j][0] = matrixA[j][0] / matrixS[0][0];
		/*if (matrixS[j][0] != 0 && matrixS[j][0]!=(int)matrixS[j][0])
			matrixS[j][0] = round(matrixS[j][0] * 100.0 + 0.5) / 100.0;*/
	}
	outPut(matrixS, NULL);
	cout << endl;
	for (int i = 1; i < SIZEMATRIX; i++)
	{
		if (i != 0)
		{
			matrixD[i][i] = sign(matrixA[i][i] - sum_spi_dp(i, matrixD, matrixS));
			/*if(matrixD[i][i] !=0 && matrixD[i][i]!=(int)matrixD[i][i])
				matrixD[i][i] = round(matrixD[i][i] * 100.0 + 0.5) / 100.0;*/ //округление*/
			matrixS[i][i] = sqrt(abs(matrixA[i][i] - sum_spi_dp(i, matrixD, matrixS)));
			/*if(matrixS[i][i] !=0 && matrixS[i][i]!=(int)matrixS[i][i])
				matrixS[i][i] = round(matrixS[i][i] * 100.0 + 0.5) / 100.0;*/ // округление 
		}

		for (int j = i + 1; j < SIZEMATRIX; j++)
		{
			matrixS[j][i] = (1 / matrixS[i][i]) * (matrixA[j][i] - sum_spi_dp_spj(i, j, matrixD, matrixS));
			/*if(matrixS[i][j] !=0 && matrixS[i][j]!=(int)matrixS[i][j])
				matrixS[i][j] = round(matrixS[i][j] * 100.0 + 0.5) / 100.0;*/
		}
	}

	cout << endl;
	cout << " Матрица S " << endl;
	outPut(matrixS, NULL);

	//Транспонируем матрицу 
	double** matrixS_transp = new double* [SIZEMATRIX] ;
	for (int i = 0; i < SIZEMATRIX; i++)
		matrixS_transp[i] = new double[SIZEMATRIX] {};

	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
		{
			matrixS_transp[i][j] = matrixS[j][i];
		}
	}

	cout << endl << "Транспонированная матрица " << endl;
	outPut(matrixS_transp,NULL);

	//y=D^(-1)*B
	double** matrixS_inverse = new double* [SIZEMATRIX];
	for (int i = 0; i < SIZEMATRIX; i++)
		matrixS_inverse[i] = new double[SIZEMATRIX] {};

	matrixS_inverse = reverseMatrix(matrixS);
	cout << " \nОбратная матрица S : \n";
	outPut(matrixS_inverse, NULL);
	
	double* y_vector = new double[SIZEMATRIX] {};
	y_vector = compostionMatrix(matrixS_inverse, matrixB);
	//roundVector(y_vector); //FIXME: если округлять то выходит искажение до 0,01
	cout << " вектор столбец Y: \n";
	outPut(NULL, y_vector);


	matrixS_transp = reverseMatrix(matrixS_transp);
	double* x_vector = new double[SIZEMATRIX] {};
	x_vector = compostionMatrix(matrixS_transp, y_vector);
	//roundVector(x_vector); //FIXME: если округлять то выходит искажение до 0,01
	cout << " \nвектор столбец X: \n";
	outPut(NULL, x_vector);

	cout << " Невязка  : \n";
	double* r_vectorNevyaska = new double[SIZEMATRIX] {};
	r_vectorNevyaska = compostionMatrix(matrixA, x_vector);
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		r_vectorNevyaska[i] -=  matrixB[i];
	}
	//roundVector(r_vectorNevyaska);TODO: в округлении все значения в массиве = 0.01
	outPut(NULL, r_vectorNevyaska);
	
	

	//Для того чтобы не было утечки данных в ОЗУ
	//очистка памяти
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		delete[] *(matrixA + i);
		delete[] *(matrixS + i);
		delete[] *(matrixD + i);
		delete[] * (matrixS_transp + i);
		delete[] * (matrixS_inverse + i);
	}
	delete[] matrixA;
	delete[] matrixB;
	delete[] matrixS;
	delete[] matrixD;
	delete[] matrixS_transp;
	delete[] matrixS_inverse;
	delete[] x_vector;
	delete[] y_vector;
	delete[] r_vectorNevyaska;

	return 0;
}

void checkMatrix(double** matrixA)
{
	double sum = 0;
	double* p = new double[SIZEMATRIX] {};
	double** checkmatrixA = new double* [SIZEMATRIX];
	for (int i = 0; i < SIZEMATRIX; i++)
		checkmatrixA[i] = new double[SIZEMATRIX] {};
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
			checkmatrixA[i][j] = matrixA[i][j];
	}

	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = i; j < SIZEMATRIX; j++)
		{
			int k = 0;
			for (sum = checkmatrixA[i][j], k = i-1; k >= 0; k--)
				sum -= checkmatrixA[i][k] * checkmatrixA[j][k];
			if (i == j)
			{
				char string[] = "Матрица не положительно определенная";
				if (sum <= 0.0)
					nrerror(string);
				p[i] = sqrt(sum);

			}
			else
				checkmatrixA[j][i] = sum / p[i];
		}
	}
	//cout << endl << sum << endl;
	//outPut(NULL, p);
	//outPut(checkmatrixA, NULL);
	delete[] p;
	for (int i = 0; i < SIZEMATRIX; i++)
		delete[] * (checkmatrixA + i);
	delete[] checkmatrixA;
}

void nrerror(char error_text[]) //Numerical Recipes standard error handler
{
	fprintf(stderr, "Run time error....\n");
	fprintf(stderr, "%s\n", error_text);
	fprintf(stderr, "Now exiting the system\n");
	exit(1);
}

void outPut(double **matrixA,double *matrixB)
{
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		if (matrixA != NULL)
		{
			for (int j = 0; j < SIZEMATRIX; j++)
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

double sum_spi_dp(int i, double **matrixD, double **matrixS)
{
	double sum = 0;
	for (int p = 0; p <= i - 1; p++)
	{
		sum += (matrixS[i][p] * matrixS[i][p]);
	}
	return sum;
}

double sum_spi_dp_spj(int i, int j, double **matrixD, double **matrixS)
{
	double sum = 0;
	for (int p = 0; p <= i - 1; p++)
		sum += matrixS[i][p] * matrixS[j][p]; 

	return sum;
}

/*double** compostionMatrix(double** matrixS_t, double** matrixD) //умножение матриц
{
	double** resultMatrix = new double* [SIZEMATRIX];// результативная матрица
	for (int i = 0; i < SIZEMATRIX; i++)
		resultMatrix[i] = new double[SIZEMATRIX] {};
	double sum = 0;
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
		{
			resultMatrix[i][j] = 0;
			for (int k = 0; k < SIZEMATRIX; k++)
				resultMatrix[i][j] += matrixS_t[i][k] * matrixD[k][j];
		}
	}
	return resultMatrix;
}*/

double* compostionMatrix(double** matrix, double* matrix_vector)//умножение матрицы на вектор столбец
{
	double* resultMatrix = new double[SIZEMATRIX] {};// результативная матрица
	double sum = 0;
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
		{
			resultMatrix[i] += matrix[i][j] * matrix_vector[j];
		}
	}
	return resultMatrix;
}

double** reverseMatrix(double** matrix)
{
	double** resultMatrix = new double* [SIZEMATRIX];
	for(int i=0;i<SIZEMATRIX;i++)
		resultMatrix[i] = new double[SIZEMATRIX] {};
	double* y = new double[SIZEMATRIX] {}; // для столбца свободных слагаемых
	double* itr; //для хранение реш СЛАУ методом итераций (т.е. находим i-й стб обратной матрицы)

	for (int i = 0; i < SIZEMATRIX; i++)
	{
		//заполнение столбца свободных слагаемых
		for (int j = 0; j < SIZEMATRIX; j++)
		{
			if (i == j) //элемент главной диаг
				y[j] = 1;
			else
				y[j] = 0;
		}

		itr = divorceSLAU_iteration(matrix, y); //получаем i-й стб обратной матрицы
		for (int k = 0; k < SIZEMATRIX; k++)
			resultMatrix[k][i] = itr[k];
	}

	//FIXME:если округлить то вызовет искажение , обратную матрицу нельзя округлять
	
	return resultMatrix;

}

double* divorceSLAU_iteration(double** matrix, double* y)
{
	//x1(k+1)=a1(n+1)/a11 (из вектора нач прибл) - a12/a11*x2(k)-..- где в роли k Для x2 выступает начальное приближ, x1(k+1)=Xn
	double* resultvector = new double[SIZEMATRIX]; // результирующий массив
	for (int i = 0; i < SIZEMATRIX; i++)
		resultvector[i] = y[i] / matrix[i][i];// начальное приближение, т.е. массив = свободный слаг/диаг элемент

	double eps = 0.0001;
	double* Xn = new double[SIZEMATRIX] {};
	do
	{
		for (int i = 0; i < SIZEMATRIX; i++)
		{
			Xn[i] = y[i] / matrix[i][i]; // следующее приближение
			for (int j = 0; j < SIZEMATRIX; j++)
			{
				if (i == j) //т.к. будет диаг элемент который равен 0
					continue;
				else
					Xn[i]/*a1(n+1)/a11*/ -= matrix[i][j] / matrix[i][i] * resultvector[j];
			}
		}

		bool flag = true; //предполагаем что уже нашли
		for (int i = 0; i < SIZEMATRIX - 1; i++)
		{
			if (abs(Xn[i] - resultvector[i]) > eps) //если вып то продолжаем итерацию while
			{
				flag = false;
				break;
			}
		}

		for (int i = 0; i < SIZEMATRIX; i++)
			resultvector[i] = Xn[i]; //храним предыдущее значение

		if (flag) //итерация закончена
			break;
	} while (1);

	return resultvector;
}

int sign(double elements)
{
	if (elements > 0)
		return 1;
	else
		return -1;
}

void roundMatrix(double** matrix)
{
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		for (int j = 0; j < SIZEMATRIX; j++)
		{
			if (matrix[i][j] != 0)
				matrix[i][j] = round(matrix[i][j] * 100.0 + 0.5) / 100.0;
		}
	}
}

void roundVector(double* vector)
{
	for (int i = 0; i < SIZEMATRIX; i++)
	{
		if (vector[i] != 0 && vector[i]!=(int)vector[i])
			vector[i] = round(vector[i] * 100.0 + 0.5) / 100.0;
	}
}


//красивый вывод столбиков
/*#define SETPOZ(_x,_y) SCRN_INFO.dwCursorPosition.X = _x*8;\
                      SCRN_INFO.dwCursorPosition.Y = _y;   \
                      SetConsoleCursorPosition(hSTDOut,SCRN_INFO.dwCursorPosition)


					  CONSOLE_SCREEN_BUFFER_INFO SCRN_INFO;
	HANDLE hSTDOut = GetStdHandle(STD_OUTPUT_HANDLE);
/*for (int i = 0; i < SIZEMATRIX; i++)
{
	for (int j = 0; j < SIZEMATRIX; j++)
	{
		SETPOZ(j, i);
		cout << matrixS[i][j] << setprecision(3) << " ";
	}
	cout << endl;
}*/