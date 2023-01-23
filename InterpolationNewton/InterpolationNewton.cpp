
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cmath>
#define PI 3.14159265358979323846
using namespace std;

void read_dataFile(vector <double>& , vector<double>& );
void Newton(vector <double> , vector <double>);
void Input_Decidee();
double function(double);
void printVector(vector<vector<double>>, int );

int main()
{
	setlocale(LC_ALL, "russian");
	cout << " \t\t\tИнтерполяционный полином Ньютона\n";
	for (int i = 0; i < 80; i++)
		cout << "-";

	vector <double>x, y;
	cout << "\n 1. Считать данные с файла.\n"
		" 2. Ввести данные в ручную.\n";
	int chose = 0;
	cin >> chose;
	switch (chose)
	{
	case 1:
		cout << " В файле должно быть следующие расположение: X Y[X]\n";
		read_dataFile(x, y);
		Newton(x, y);
		break;
	case 2:
		Input_Decidee();
		break;
	default:
		break;
	}	
}

void read_dataFile(vector <double>& x, vector<double>& y)
{
	ifstream file("data.txt");
	if (!file.is_open())
	{
		cout << "Файла не существует.";
		return;
	}

	double temp = 0;
	while (!file.eof())
	{
		file >> temp;
		x.push_back(temp);
		file >> temp;
		y.push_back(temp);
	}
	file.close();	
}

void Newton(vector <double> x, vector <double>y)
{
	vector<vector<double>> dy; // аналог двумерный динамический массив
	double h=0; //шаг

	h = (x[1] - x[0]);
	for (unsigned i = 0; i < x.size(); ++i)
		cout << "x[" << i << "] = " << x[i] << "\t" << "y[" << x[i] << "] = " << y[i] << endl;

	int maxIndex = x.size() - 1;
	dy.resize(maxIndex);
	for (int i = 0; i < maxIndex; i++)
	{
		dy[i].resize(maxIndex);
		for (int j = 0; j < maxIndex; j++)
			dy[i][j] = 0;
	}

	//Первые конечные разности
	for (int i = 0; i < maxIndex; i++)
		dy[i][0] = y[i + 1] - y[i];

	//Вторые
	for (int j = 1; j < maxIndex; j++)
	{
		for (int i = 0; i < maxIndex - j; i++) // потому что идем лестницей
			dy[i][j] = dy[i + 1][j - 1] - dy[i][j - 1];
	}
	cout << endl;

	printVector(dy,maxIndex);

	double x0 = PI / 9;
	cout << " \n Ввести свое значения x или задать приближенное?(0/1): ";
	bool flag = false; cin >> flag;
	if (flag == false)
	{
		cin >> x0;
	}
	double mult = 1;
	double sum = y[0];
	double q = (x0 - x[0]) / h;
	int factorial = 1;

	for (int i = 0; i < maxIndex; i++)
	{
		mult *= (q - i);
		factorial *= i + 1;
		sum += mult * dy[0][i] / (factorial);
	}
	cout << "\n Первая формула Ньютона: " << setprecision(5) << sum << endl;

	q = (x0 - x[maxIndex]) / h;
	sum = y[maxIndex];
	mult = 1;
	factorial = 1;
	for (int i = 0; i < maxIndex; i++)
	{
		mult *= (q + i);
		factorial *= i+1;
		sum += mult * dy[maxIndex - i - 1][i] / (factorial); // по мнимой диагонали
	}

	cout << "\n Вторая формула Ньютона: " << setprecision(5) << sum << endl;
}

void Input_Decidee()
{
	vector<double> x, y;
	cout << " Укажите количество точек: ";
	int n; cin >> n;
	for (int i = 0; i < n; i++)
	{
		double temp = 0;
		cin >> temp;
		x.push_back(temp);
		y.push_back(function(temp));
	}
	system("cls");
	cout << endl;
	Newton(x, y);
}

double function(double x)
{
	return cos(x)* sqrt(x);//sin(x)* x;
}

void printVector(vector<vector<double>> V,int maxIndex)
{
	for (int i = 0; i < maxIndex; i++)
	{
		for (int j = 0; j < maxIndex; j++)
			cout  << V[i][j] << setprecision(4) << " ";
		cout << endl;
	}
}
