#include "pch.h"
#include "CppUnitTest.h"
#include "MethodHoleckogo.cpp"//для подключения св-ва теста Каталоги - Включаемые Каталоги - ИЗменить - новая строка - указать путь ОК

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestFunction
{
	TEST_CLASS(TestFunction)
	{
	public:
		
		TEST_METHOD(initMatrix)
		{
			double** matrixA = new double* [SIZEMATRIX];
			for (int i = 0; i < SIZEMATRIX; i++)
				matrixA[i] = new double[SIZEMATRIX];
			//outPut(matrixA, NULL);
		//	initMatrix(matrixA);
			//ifstream file_numbers("Digit.txt");
			//main(file_numbers);
		}
	};
}
