//Метод левый прямоугольник
// при начальном разибении отрезков на 400 и точностью eps=0.001 выдает более точный результат с меньшим количеством шагов
// через онлайн ответ 23252.55567..
// Метод Симпсона
// при любом разбиении выдает ответ значительно быстрее , при 25000 дает более точный результат
// в цикле while выдает 23252 при 500  разбиении и епсилон 0,0001 но медлненне прямоугольников

//все вычисление производились при [1,10] с заданной точностью

#include <iostream>
#include <cmath>

using namespace std;

double function(double);
double method_LeftRectangle(const double, const double, unsigned int);
double method_Sympsona(const double, const double, unsigned int);

int main()
{
	cout << " Method left Rechangle \n";
	cout << " Input distance : [a,b] : " << " \n a = "; 
	double a; cin >> a;
	cout << " b = "; 
	double b; cin >> b;
	cout << " Distance [a,b]: [" << a << " , " << b << "] \n";
	cout << " Input the number of segment splits : ";
	//int n; cin >> n;
	int split_distance = 0;
	cin >> split_distance;
	//double h = (b - a) / (n);
	//cout << " Step : " << h << endl;
	double eps = 0, diff = 0;
	cout << " Input epsilon for Left Rectangle : "; cin >> eps;
	cout << " Integral : x*exp(x)*sin(x) " << endl;

	//double x0 = a;
	int i = 0;
	do
	{
		i++;
		diff = fabs(method_LeftRectangle(a, b, split_distance * i) - method_LeftRectangle(a, b, split_distance * (i + 1)));
		
		//cout << i << " : " << diff << endl;
	} while (diff > eps);
	
	//double integral = method_LeftRectangle(a,b, begin_split_distance);
	/*for (int i = 0; i <= n - 1; i++)
	{
		if(i!=0)
			x0 = a + i * h;
		cout << " Iteration: " << i << "; xi = " << x0 << endl;
		integral += h*function(x0);
		cout << " f(xi) = " << function(x0) << "; summa = " << integral << endl;
		
	}*/
	/*cout << endl << " Integral on distance [" << a << "," << b << "] f(x) = x*exp(x)*sin(x) = " << integral << endl;
	cout << " All iteration = " << n << endl;*/

	cout << " Method Left Rectangle ";
	cout << endl << " Integral on distance [" << a << "," << b << "] f(x) = x*exp(x)*sin(x) = " << method_LeftRectangle(a,b, split_distance *(i+1)) << endl;
	cout << " Count iteration : " << i << "; accurate is achieved at split distance " << split_distance  << " and epsilon " << eps << endl;

	cout << " Input epsilon for method Sympson = "; cin >> eps;
	int j = 0;
	do
	{
		j++;
		diff = fabs(method_Sympsona(a, b, split_distance * j) - method_Sympsona(a, b, split_distance * (j + 1)));
	} while (diff > eps);
	cout << " Method Sympsona ";
	cout << endl << " Integral on distance [" << a << "," << b << "] f(x) = x*exp(x)*sin(x) = " << method_Sympsona(a, b, split_distance * (j + 1)) << endl;
	cout << " Count iteration : " << j << "; accurate is achieved at split distance " << split_distance << " and epsilon " << eps << endl;
	cin.get();
	return 0;

}

double function(double x)
{
	return x * exp(x)* sin(x);
}

double method_LeftRectangle(const double a,const double b, unsigned int count_segment)
{
	double h = (b - a) / (count_segment - 1);
	double summa =0 , x = a;
	for (int i = 0; i <= count_segment - 1; i++)
	{
		if (i != 0)
			x = a + i * h;
		summa += h * function(x);
	}
	return summa;
}

double method_Sympsona(const double a, const double b, unsigned int count_segment)
{
	double h = (b - a) / count_segment; 
	double summa = function(a) + function(b);
	for (int i = 1; i <= count_segment; i++)
	{
		if(i%2==0)
			summa += 2 * function(a + i * h);
		else
			summa += 4 * function(a + i * h);
	}

	summa *= h / 3;
	return summa;
}
