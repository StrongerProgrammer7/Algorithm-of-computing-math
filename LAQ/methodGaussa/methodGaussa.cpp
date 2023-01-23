#include <iostream>
#include <fstream>
#include<clocale>
#include <string>
#include <float.h>
#include <iomanip>  
#include <cassert>  
#include <regex>


using namespace std;

bool checkCorrectInputNum(string&, int);
void outputequlities(double**, double*, int);
double* methodGaussa(double**, double*, int, int&, int&);
double* compostionMatrix(double** , double* , int );
//void outputChangeMatrix(int, double**, double*);
void checkDataFile(ifstream&, int);
string pointToComma(string number);


int main()
{
	setlocale(LC_ALL, "RU");
	cout << " \t\t    Программа решает CЛАУ в форме Ах=В, \n\t\t  (где А- квадратная матрица, размера nxn ,\n\t\t     B-вектор столбец , размера 1xn) \n\t\t\tМетодом Гаусса\n";
	bool repeat = 1;
	ofstream clearsFile("Linear equation.txt");
	clearsFile.close();

	while (repeat == true)
	{
		//Ввод
		string pathFile;
		int countOfVariables;
		cout << " Введите количество переменных: ";
		cin >> countOfVariables;
		while (countOfVariables == 0 || countOfVariables < 0)
		{
			cout << " Система не имеет решений, т.к. либо ваша система это 0 либо\n вовсе не существует , но скорее всего вам лень вводить :)."
				"\nВсего доброго или повторим(1/0)?";
			bool repeat = 0;
			cin >> repeat;
			if (repeat == 1)
			{
				cout << " Введите количество переменных: ";
				cin >> countOfVariables;
			}
			else
				return 0;
		}


		cout << " Ввод осуществить самостоятельно или через файл? (Файл-1, самостоятельно-0): ";
		bool input = 0;
		cin >> input;
		if (input == 1)
		{
			cout << " В файле должна быть следующая структура ввода: \n1-я строка и последующие (в зависимости от того сколько переменных) через пробел записаны числа."
				"\n последняя строка содержит коэффициенты матрицы В.  "
				"\nПревышение количество чисел не допускается.Допускаются символы: '-' '+' '.' \n";

			cout << " Введите путь к файлу (формата Диск:\{папка}\ файл.формат ) : ";
			cin >> pathFile;
			ifstream fileData(pathFile);
			while (!fileData)
			{
				pathFile = "";
				cout << " Путь к файл указан не вверно либо файла не существует. \n Укажите заново путь: ";
				cin >> pathFile;
				fileData.open(pathFile);
			}
			cin.get();
			checkDataFile(fileData, countOfVariables);
			cout << " Все верно! Коэффициенты введены.\n";
			fileData.close();
		}
		else
		{
			cin.get();//cin.ignore() //kill space
			//Создаем файл и заносим в него данные 
			ofstream recordDataInFile("Linear equation.txt");
			cout << " Ввод матрицы А" << endl;
			cout << " Ввод осущствеляется по принципу: \nввод коэффициентов для определенной строки через пробел , "
				"\nих количество равно количеству указанных столбцов.\n "
				"(допускается: '+' ',' '-' '[0-9]' \n";
			for (int line = 0; line < countOfVariables; line++)
			{
				string setOfNumbers;
				cout << line + 1 << " строка из " << countOfVariables << " столбцов :";
				getline(cin, setOfNumbers);

				while ((checkCorrectInputNum(setOfNumbers, countOfVariables)))
				{
					getline(cin, setOfNumbers);

				}
				recordDataInFile << setOfNumbers << endl;
			}

			cout << " Ввод матрицы В" << endl;
			for (int line = 0; line < countOfVariables; line++)
			{
				string setOfNumbers;
				cout << line + 1 << " строка , " << countOfVariables + 1 << "-й столбец :";
				cin >> setOfNumbers;
				while (checkCorrectInputNum(setOfNumbers, 1))
				{
					cin >> setOfNumbers;
				}
				recordDataInFile << setOfNumbers << " ";
			}
			recordDataInFile.close();
		}


		cout << endl;

		double** matrixA = new double* [countOfVariables];
		double* matrixB = new double[countOfVariables];

		if (input == 1)
		{
			ifstream outDataFile(pathFile);
			while (!outDataFile.eof())
			{
				string number;
				for (int i = 0; i < countOfVariables; i++)
				{
					matrixA[i] = new double[countOfVariables];
					for (int j = 0; j < countOfVariables; j++)
					{
						number = "";
						outDataFile >> number;
						number = pointToComma(number);

						char const* c[] = { number.c_str() };
						//cout << "C " << *c << endl;
						matrixA[i][j] = atof(*c);
						//	cout << " " << matrixA[i][j];
					}
					cout << endl;
				}
				for (int i = 0; i < countOfVariables; i++)
				{
					number = "";
					outDataFile >> number;
					number = pointToComma(number);

					char const* c[] = { number.c_str() };
					//cout << "C " << *c << endl;
					matrixB[i] = atof(*c);
				}
				break;
			}
			outDataFile.close();

		}
		else
		{
			ifstream outDataFile("Linear equation.txt");
			while (!outDataFile.eof())
			{
				string number;
				for (int i = 0; i < countOfVariables; i++)
				{
					matrixA[i] = new double[countOfVariables];
					for (int j = 0; j < countOfVariables; j++)
					{
						outDataFile >> number;
						char const* c[] = { number.c_str() };
						//cout << "C " << *c << endl;
						matrixA[i][j] = atof(*c);
					}
				}
				for (int i = 0; i < countOfVariables; i++)
				{

					outDataFile >> number;
					char const* c[] = { number.c_str() };
					//cout << "C " << *c << endl;
					matrixB[i] = atof(*c);
				}
				break;
			}
			outDataFile.close();
		}

		
		outputequlities(matrixA, matrixB, countOfVariables);

		int rang = 0, rangMatrixAB = 0;
		double* varaibles = methodGaussa(matrixA, matrixB, countOfVariables, rang, rangMatrixAB);

		cout << "Ранг обыкновенной матрицы : " << rang << "\n Ранг расширенной матрицы: " << rangMatrixAB << endl;
		cout << endl;

		ofstream recordDecisionAndAnswersFile("Linear equation.txt", ios::app);
		recordDecisionAndAnswersFile << endl;
		recordDecisionAndAnswersFile << endl << " Решение методом гаусса : " << endl;
		for (int i = 0; i < countOfVariables; i++) {
			for (int j = 0; j < countOfVariables; j++)
			{
				if (matrixA[i][j] < 0 && matrixA[i][j] != 0)
				{
					recordDecisionAndAnswersFile << "" << setprecision(4) << matrixA[i][j] << "" << "(x" << j + 1 << ")";
				}
				else
					if (matrixA[i][j] != 0)
						recordDecisionAndAnswersFile << setprecision(4) << matrixA[i][j] << "(x" << j + 1 << ")";
					else
						recordDecisionAndAnswersFile << "   ";

				if (j != countOfVariables - 1 && matrixA[i][j + 1] > 0 && matrixA[i][j] != 0)
					recordDecisionAndAnswersFile << "+";
			}
			if (matrixB[i] != 0)
				recordDecisionAndAnswersFile << " = " << matrixB[i] << endl;
		}
		recordDecisionAndAnswersFile << endl;
		if (varaibles != 0)
		{
			for (int i = 0; i < countOfVariables; i++)
			{
				recordDecisionAndAnswersFile << "x" << i + 1 << " = " << varaibles[i] << " ";
				if (i == 5)
					recordDecisionAndAnswersFile << endl;
			}
		}
		else
			recordDecisionAndAnswersFile << " Система несовместна (не имеет решений) ";
		recordDecisionAndAnswersFile.close();

		ifstream readoutFile("Linear equation.txt");
		while (!readoutFile.eof())
		{
			string s;
			getline(readoutFile, s);
			cout << s << endl;
		}
		readoutFile.close();

		cin.get();

		cout << " \n Невязка  : \n";
		double* r_vectorNevyaska = new double[countOfVariables] {};
		r_vectorNevyaska = compostionMatrix(matrixA, varaibles, countOfVariables);
		for (int i = 0; i < countOfVariables; i++)
		{
			r_vectorNevyaska[i] -= matrixB[i];
		}
		outputequlities(NULL, r_vectorNevyaska, countOfVariables);


		//Clean Memory
		for (int i = 0; i < countOfVariables; i++)
			delete[] matrixA[i];
		delete[] matrixA;
		delete[] varaibles;
		delete[] matrixB;
		delete[] r_vectorNevyaska;
		cout << " \nПовторить ввод или закрыть программу? (1/0)";
		cin >> repeat;

	}
	return 0;
}

double* compostionMatrix(double** matrix, double* matrix_vector, int countOfVariables)//умножение матрицы на вектор столбец
{
	double* resultMatrix = new double[countOfVariables] {};// результативная матрица
	double sum = 0;
	for (int i = 0; i < countOfVariables; i++)
	{
		for (int j = 0; j < countOfVariables; j++)
		{
			resultMatrix[i] += matrix[i][j] * matrix_vector[j];
		}
	}
	return resultMatrix;
}

bool checkCorrectInputNum(string & setOfNumbers, int countOfVariables)
{
	int indexElemMatrix = 0;
	regex v("[+-]?([0-9]+([,][0-9]*)?|[.][0-9]+)");
	string s2;
	size_t pos = 0;
	if (setOfNumbers.size() != 0)
	{
		setOfNumbers = ' ' + setOfNumbers + ' ';
		for (int i = 1; i < setOfNumbers.size(); i = pos)
		{
			pos = setOfNumbers.find(' ', i);

			s2 = setOfNumbers.substr(i, pos - i);

			if (setOfNumbers[pos] == ' ')
			{
				char const* c[] = { s2.c_str() };
				while (!(regex_match(*c, v)))
				{
					cout << " Ошибка! Введен неверный символ. Допустимы только цифры.\nПовторите ввод: ";
					return true;
				}
				indexElemMatrix++;
				s2 = "";
			}
			pos++;
		}
	}

	if (indexElemMatrix > countOfVariables)
	{
		cout << " Ошибка!Введено больше переменных чем требуется.\nПовторите ввод:";
		return true;
	}
	else
	{
		if (indexElemMatrix < countOfVariables)
		{
			for (int i = indexElemMatrix; i < countOfVariables; i++)
			{
				setOfNumbers += " ";
				setOfNumbers += "0";
			}

		}
	}
	return false;
}

void outputequlities(double** matrixA, double* matrixB, int countOfVariables)
{
	cout << endl;
	for (int i = 0; i < countOfVariables; i++)
	{
		if (matrixA != NULL)
		{
			for (int j = 0; j < countOfVariables; j++)
			{
				//if(matrixA[i][j]>0)


				if (j <= countOfVariables - 1 && j != 0 && matrixA[i][j] >= 0)
					cout << "+" << "" << matrixA[i][j] << "" << "(x" << j + 1 << ")";
				else
					if ((j < countOfVariables - 1 || j == countOfVariables - 1) && matrixA[i][j] < 0)
						cout << "" << matrixA[i][j] << "" << "(x" << j + 1 << ")";
					else
						if (j < countOfVariables && matrixA[i][j] >= 0)
							cout << "" << matrixA[i][j] << "" << "(x" << j + 1 << ")";
			}
		}
		
		cout << " = " << setw(3) << matrixB[i] << "" << endl;
	}
}

double* methodGaussa(double** matrixA, double* matrixB, int countOfVariables, int& rang, int& rangMatrixAB)
{
	double* varaibles = new double[countOfVariables], maxElementMartix;
	int indexElemMatrix = 0, indexLineMaxElm;
	const double eps = FLT_EPSILON;

	while (indexElemMatrix < countOfVariables)
	{
		//Search line with maxElementMartix matrixA[i][j]
		maxElementMartix = abs(matrixA[indexElemMatrix][indexElemMatrix]);
		indexLineMaxElm = indexElemMatrix;
		for (int i = indexElemMatrix + 1; i < countOfVariables; i++)
		{
			if (fabs(matrixA[i][indexElemMatrix]) > maxElementMartix)
			{
				maxElementMartix = fabs(matrixA[i][indexElemMatrix]);
				indexLineMaxElm = i;
				cout << "\n Max(matrixA " << i + 1 << " " << indexElemMatrix + 1 << ")=" << maxElementMartix << endl;
			}
		}
		if (maxElementMartix <= eps)
		{
			for (int i = indexElemMatrix; i < countOfVariables; i++)
				matrixA[i][indexElemMatrix] = 0.0;
			rangMatrixAB = indexElemMatrix;
			if (matrixB[indexElemMatrix] != 0)
				rangMatrixAB++;
			indexElemMatrix++; // расширенная матрица ранг
			outputequlities(matrixA, matrixB, countOfVariables);
			//outputChangeMatrix(countOfVariables, matrixA, matrixB);
			continue;
		}

		cout << endl;

		//Меняем местами строки
		if (indexLineMaxElm != indexElemMatrix) //исключаем "само на себя"
		{
			for (int j = 0; j < countOfVariables; j++)
			{
				if (matrixA[indexElemMatrix][j] == 0 && matrixA[indexLineMaxElm][j] == 0) // если нули то игнор
					continue;
				double temp = matrixA[indexElemMatrix][j];
				matrixA[indexElemMatrix][j] = matrixA[indexLineMaxElm][j];
				matrixA[indexLineMaxElm][j] = -temp;
			}
			double temp = matrixB[indexElemMatrix];
			matrixB[indexElemMatrix] = matrixB[indexLineMaxElm];
			matrixB[indexLineMaxElm] = -temp;

			outputequlities(matrixA, matrixB, countOfVariables);
			//outputChangeMatrix(countOfVariables, matrixA, matrixB);
		}

		maxElementMartix = matrixA[indexElemMatrix][indexElemMatrix];
		assert(fabs(maxElementMartix) > eps); // макс дб больше епс для продолжения, иначе ошибка в программе

		for (int i = indexElemMatrix + 1; i < countOfVariables; i++)
		{
			double temp = (-matrixA[i][indexElemMatrix]) / maxElementMartix;

			matrixA[i][indexElemMatrix] = 0.0;//после вычитыния слишком маленькое число получается , чтобы мусора не было явно присваиваем 0
			for (indexLineMaxElm = indexElemMatrix + 1; indexLineMaxElm < countOfVariables; ++indexLineMaxElm)
			{
				matrixA[i][indexLineMaxElm] += temp * matrixA[indexElemMatrix][indexLineMaxElm];
			}
			matrixB[i] += temp * matrixB[indexElemMatrix];

			outputequlities(matrixA, matrixB, countOfVariables);
			//outputChangeMatrix(countOfVariables, matrixA, matrixB);
		}
		cout << endl;

		rang++;
		++indexElemMatrix;
	}

	if (rang == countOfVariables)
		rangMatrixAB = rang;
	bool  appliedFCR = 0;
	double freeVariables = 0;
	if (rangMatrixAB > rang)
	{
		cout << " Ранг расширенной матрицы больше ранга \nобыкновенной матрицы ";
		return 0;
	}
	else
	{
		if (rangMatrixAB == rang && rangMatrixAB < countOfVariables)
		{
			cout << " Решений бесконечное множество. \nКоличество переменных больше ,"
				"количества строк , введем свободную переменную " << endl;
			cin >> freeVariables;
			appliedFCR = 1;
		}
	}
	/*int g = 0;
	for (int i = 0; i < countOfVariables; i++)
	{
		for (int j = 0; j < countOfVariables; j++)
		{
			if (matrixA[i][j] == 0)
				g++;
		}
		if (g == countOfVariables && matrixB[i] == 0)
		{

			break;
		}
		else
			if (g == countOfVariables)
			{
				cout << " Решений бесконечное множесство решений. \nКоличество переменных больше , количества строк , введем свободную переменную " << endl;
				cin >> freeVariables;
				appliedFCR = 1;
			}

		g = 0;
	}*/

	//Outside
	double* temp = new double[countOfVariables];
	for (int i = 0; i < countOfVariables; i++)
		temp[i] = matrixB[i];
	for (indexElemMatrix = countOfVariables - 1; indexElemMatrix >= 0; indexElemMatrix--)
	{
		double element = 0;
		if (appliedFCR == 1)
		{
			varaibles[indexElemMatrix] = freeVariables;
			appliedFCR = 0;
		}
		else
		{
			if (indexElemMatrix == countOfVariables - 1)
			{

				for (int j = 0; j < countOfVariables; j++)
					if (matrixA[countOfVariables - 1][j] != 0)
						element = matrixA[countOfVariables - 1][j];
				if (element != 0)
					varaibles[indexElemMatrix] = matrixB[indexElemMatrix] / element;
			}
			else
			{
				varaibles[indexElemMatrix] = temp[indexElemMatrix];
			}

		}
		element = 0;
		if (indexElemMatrix < countOfVariables - 1)
		{
			if (matrixA[indexElemMatrix][indexElemMatrix] != 0)
			{
				element = matrixA[indexElemMatrix][indexElemMatrix];
				varaibles[indexElemMatrix] = temp[indexElemMatrix] / element;
			}
		}

		for (int i = 0; i < indexElemMatrix; i++)
		{
			temp[i] = temp[i] - matrixA[i][indexElemMatrix] * varaibles[indexElemMatrix];
		}
	}
	return varaibles;

}

/*void outputChangeMatrix(int countOfVariables, double** matrixA, double* matrixB)
{
	for (int i = 0; i < countOfVariables; i++)
	{
		for (int j = 0; j < countOfVariables; j++)
			cout << setprecision(3) << matrixA[i][j] << "(x" << j+1 << ")";
		cout <<  setw(2) << "|" << setprecision(3) << matrixB[i] << endl;
	}
	cout << endl;
}*/

void checkDataFile(ifstream & fileData, int countOfVariables)
{
	while (!fileData.eof())
	{
		int countCoefficients = 0;
		size_t pos = 0;
		string number, lineNumbers;
		getline(fileData, lineNumbers);
		int j = 0;
		lineNumbers = ' ' + lineNumbers + ' ';
		for (int i = 1; i < lineNumbers.size(); i = pos)
		{
			pos = lineNumbers.find(' ', i);

			number = lineNumbers.substr(i, pos - i);
			number = pointToComma(number);

			if (lineNumbers[pos] == ' ' && number != "")
			{
				char const* c[] = { number.c_str() };
				if (!((atof(*c) > 0 || atof(*c) < 0 || atof(*c) == 0) || lineNumbers[0] == '0' || lineNumbers[0] == '-'))
				{
					cout << " Некорректный ввод ! Исправьте данные в файле: ";
					cout << " " << *c << endl;
					exit(0);
				}
				else
					countCoefficients++;
				number = "";
			}
			pos++;

			if (countCoefficients > countOfVariables)
			{
				cout << " Превышено количество чисел в строке ! Исправьте файл.";
				exit(0);
			}
		}

	}
}

string pointToComma(string number)
{
	for (int k = 0; k < number.size(); k++)
		if (number[k] == '.')
			number[k] = ',';
	return number;
}