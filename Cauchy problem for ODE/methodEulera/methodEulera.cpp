#include <iostream>
#define SIZE 10
using namespace std;

double function(double x, double y); // y'=3sin(2y)+x Начальное условие y(0)=2 на отрезке [0,1] h=0.1;
double Runge_K(double x, double y, double h);

int main()
{
	setlocale(LC_ALL, "RU");

	double* nodeX = new double[SIZE+1]; //точки
	double* y = new double[SIZE+1]; // приближенное значений функций в этих точках
	double a = 0.0, b = 0.0;//отрезок
	cout << " Введите отрезок: начало : ";
	cin >> a;
	cout << " Введите отрезок: конец : ";
	cin >> b;
	cout << " Введите начальную точку x0 на отрезке: ";
	cin >> nodeX[0];
	cout << " Введите начальное условие : ";
	cin >> y[0];//nodeX[0];
	double h = 0.0; // если известны узлы то x[i-1]-x[i]
	int n = 0;
	cout << " Введите количество разбиений отрезка(целое): ";
	cin >> n;
	h = (b - a) / n;

	int select = 0;
	cout << " Выбирете метод для вычисления ОДУ : \n";
	cout << " 1.Метод Эйлера.\n 2. Метод Рунге-Кутты 2 порядка(частный случай Эйлера)\n 3.Метод Рунге-Кутты 4 порядка\n";
	cin >> select;
	switch (select)
	{
	case 1:
		cout << " Вычисление ОДУ методом Эйлера: 3sin(2y)+x" << endl; // Неявная формула
		for (int i = 1; i <= n; i++)
		{
			nodeX[i] = nodeX[i - 1] + h; //a + i * h;
			y[i] = y[i - 1] + h * function(nodeX[i - 1], y[i - 1]);
			cout << " Итерация: " << i << " --> X[" << i << "] = " << nodeX[i] << "--> Y[" << i << "] = " << y[i] << endl;
		}
		break;
	case 2:
		cout << " Вычисление ОДУ методов Рунге-Кутты второго порядка\n(частный случай Эйлера) :3sin(2y)+x " << endl;
		for (int i = 1; i <= n; i++)
		{
			cout << "Прогноз : " << i << " : ";
			nodeX[i] = nodeX[i - 1] + h;
			y[i] = y[i - 1] + h * function(nodeX[i - 1], y[i - 1]);
			cout << " Y[" << i << "] = " << y[i] << endl;
			cout << "Коррекция : " << i << " ";
			y[i] = y[i - 1] + (h / 2) * (function(nodeX[i - 1], y[i - 1]) + function(nodeX[i], y[i]));
			cout << " Y[" << i << "] = " << y[i] << " --> X[" << i << "]" << nodeX[i] << endl;
		}
		break;
	case 3:
		cout << " Вычисление ОДУ методов Рунге-Кутты 4 порядка :3sin(2y)+x " << endl;
		for (int i = 1; i <= n; i++)
		{
			nodeX[i] = nodeX[i - 1] + h;
			y[i] = y[i - 1] + (h / 6.0) * (Runge_K(nodeX[i - 1], y[i - 1], h));	
			cout << " Итерация: " << i << " --> X[" << i << "] = " << nodeX[i] << "--> Y[" << i << "] = " << y[i] << endl;
		}
		break;
	case 4:
		cout << " Вычисление ОДУ методом Адамса-Баншорта(экстраполяционный) 4 порядка : 3sin(2y)+x  " << endl; //неявный
		break;
	case 5:
		cout << " Вычисление ОДУ методом Адамса-Мультона 2 порядка : 3sin(2y)+x  " << endl; //явный

		break;
	default:
		break;
	}
	
	/*абс погр = max[i](abs(y[i] при h, -y[i] при h/2))/2^p-1*/
	delete[] nodeX;
	delete[] y;
	return 0;

}

double Runge_K(double x, double y,double h)
{
	double k1 = function(x, y);
	double k2 = 2.0 * function(x + h / 2.0, y + (h / 2.0) * k1);
	double k3 = 2.0 * function(x + h / 2.0, y + (h / 2.0) *k2);
	double k4 = function(x + h, y + h * k3);
	return  k1+ k2 + k3 + k4;
}

double function(double x, double y)
{
	return 3 * sin(2 * y) + x; 
}

//function y''+3xy'-2y=1.5; [0.5,1] y(0.5)=1, y'(0.5)=0.5 h=0.1
//y'=z;  y(0.5)=1, z(0.5)=0.5
// z'=-3xz+2y+1.5 
/*
double function(x,y,z)
{
	return -3*x*z+2y+1.5;
}

в main()
{
	//ввод отрезка а,b
	//ввод х0 = a
	//ввод у0 =1
	//ввод z0 = 0.5
	//h = (b-a)/n 
	for(int i=1;i<=n;i++)
	{
		x[i]=x[i-1]+h;
		y[i]=y[i-1]+h*function(x[i-1],y[i-1],z[i-1]);
		z[i]=z[i-1]+h*G(x[i-1],y[i-1],z[i-1]);
	}
}

*/