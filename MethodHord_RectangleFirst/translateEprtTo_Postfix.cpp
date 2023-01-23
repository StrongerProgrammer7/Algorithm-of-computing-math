
#include <map>
#include <vector>
#include <string>
#include <set>
#include <stack>

using namespace std;

enum tokenType
{
	var,/*Перем*/
	cst, /*конст*/
	openBr,/* ( */
	closeBr,/* ) */
	func,/*функция*/
	operation, /*операция*/
	arr,/*массив*/
	openArBr, /* { */
	closeArBr, /* } */
	identify/*неизвестно*/
};

struct token
{
	string name;
	tokenType type;
	token(string str, tokenType typ)
	{
		name = str;
		type = typ;
	}
	token() { name = ""; type = identify; }
};
typedef vector<token> TOKENS;

//Функция считывает выражение в строку "expr" и ищем переменные или массивы
void createSetOfDelimiters(set<char> &mathSymbolSet)
{
	const string mathSymbol = " ()+/*-^&|!%[]";
	for (int i = 0; i < mathSymbol.size(); i++)
	{
		mathSymbolSet.insert(mathSymbol[i]);
	}
	return;
}

bool isDelimiter(char symbol, set<char> mathSymbolSet)
{
	return mathSymbolSet.count(symbol) > 0;
}

bool isOpenBrekets(TOKENS expr, __int64 j)
{
	if (expr[j - 1].type == openBr)
		return true;
	return false;
}
bool isCloseBreketsOrOperation(TOKENS expr, __int64 j)
{
	if (expr[j + 1].type == closeBr || expr[j + 1].type == operation)
		return true;
	return false;
}

//Функция расчленения на токены
void createTokensFromExpression(string & expression, TOKENS & expressionToToken)
{
	set<char> mathSymbolSet;
	createSetOfDelimiters(mathSymbolSet);

	string ex = expression + " ";
	string name = "";

	int i = 0;
	while (i < ex.size() - 1)
	{
		name = "";
		if (isDelimiter(ex[i], mathSymbolSet))
		{
			if (ex[i] == ' ') //Пробел просто переписываем
			{
				i++;
				continue;
			}

			name = ex[i]; // Люб др добав в имя токена
			i++;
		}
		else
		{
			while (!isDelimiter(ex[i], mathSymbolSet)) /*Цифры тоже пихает в структуру*/
			{
				//if variable or name_array
				name += ex[i];
				i++;
			}
		}
		expressionToToken.push_back(token(name, var));
	}

	//Даем каждому токену имя
	for (int j = 0; j < expressionToToken.size(); j++)
	{
		if (expressionToToken[j].name[0] == '[')
		{
			expressionToToken[j].type = openArBr;
			continue;
		}
		if (expressionToToken[j].name[0] == ']')
		{
			expressionToToken[j].type = closeArBr;
			continue;
		}
		if (expressionToToken[j].name[0] == '(')
		{
			expressionToToken[j].type = openBr;
			continue;
		}
		if (expressionToToken[j].name[0] == ')')
		{
			expressionToToken[j].type = closeBr;
			continue;
		}
		if (isdigit(expressionToToken[j].name[0]))
		{
			expressionToToken[j].type = cst;
			continue;
		}

		if (isalpha(expressionToToken[j].name[0]))
		{
			if (j < expressionToToken.size() - 1 && expressionToToken[static_cast<__int64>(j) + 1].name[0] == '[')
				expressionToToken[j].type = arr;
			//continue;
		}
		if (isalpha(expressionToToken[j].name[0]))
		{
			if (j < expressionToToken.size() - 1 && expressionToToken[static_cast<__int64>(j) + 1].name[0] == '(')
				expressionToToken[j].type = func;
			continue;
		}

		expressionToToken[j].type = operation;
	}

	//Пров - и !, что это префиксные операции
	for (int j = 0; j < expressionToToken.size(); j++)
	{
		if (expressionToToken[j].name == "-" && (j == 0 || isOpenBrekets(expressionToToken,j)))
			expressionToToken[j].name = "opposite";
		if (expressionToToken[j].name == "!" && (j == expressionToToken.size() - 1 || isCloseBreketsOrOperation(expressionToToken,j)))
			expressionToToken[j].name = "factorial";
	}
	return;
}

//----------------------------------------End Функция расчленения на токены

void createPriority(map<string, int> &priority)
{
	priority["+"] = 10;
	priority["-"] = 10;
	priority["*"] = 25;
	priority["/"] = 20;
	priority["^"] = 30;
	priority["opposite"] = 10;
	priority["factorial"] = 30;
	priority["%"] = 20;
	priority["&"] = 5;
	priority["|"] = 5;
	priority["!"] = 40;
}


/*Постфиксная запись*/
void createPostfixFromToken(TOKENS & expressionInToken, TOKENS & postfixExpressionToken)
{
	map<string, int> priority;
	createPriority(priority);

	stack<token>stackMathSymbol;

	for (int i = 0; i < expressionInToken.size(); i++)
	{
		switch (expressionInToken[i].type)
		{
		case var:
		case cst:
			postfixExpressionToken.push_back(expressionInToken[i]);
			break;
		case openBr:
			stackMathSymbol.push(expressionInToken[i]);
			break;
		case closeBr:
			while (stackMathSymbol.top().type != openBr)
			{
				postfixExpressionToken.push_back(stackMathSymbol.top());
				stackMathSymbol.pop();
			}
			stackMathSymbol.pop();
			break;
		case openArBr:
			stackMathSymbol.push(expressionInToken[i]);
			break;
		case closeArBr:
			while (stackMathSymbol.top().type != openArBr)
			{
				postfixExpressionToken.push_back(stackMathSymbol.top());
				stackMathSymbol.pop();
			}
			stackMathSymbol.pop();
			break;
		case operation:
			if (stackMathSymbol.size())
			{
				while (stackMathSymbol.size() != 0
					&& ((stackMathSymbol.top().type == operation && priority[expressionInToken[i].name] <= priority[stackMathSymbol.top().name])
						|| stackMathSymbol.top().type == func
						|| stackMathSymbol.top().type == arr))
				{
					postfixExpressionToken.push_back(stackMathSymbol.top());
					stackMathSymbol.pop();
				}
			}
			stackMathSymbol.push(expressionInToken[i]);
			break;
		case arr:
			while (stackMathSymbol.size() && stackMathSymbol.top().type == arr)
			{
				postfixExpressionToken.push_back(stackMathSymbol.top());
				stackMathSymbol.pop();
			}
			stackMathSymbol.push(expressionInToken[i]);
			break;
		case func:
			while (stackMathSymbol.size() && stackMathSymbol.top().type == func)
			{
				postfixExpressionToken.push_back(stackMathSymbol.top());
				stackMathSymbol.pop();
			}
			stackMathSymbol.push(expressionInToken[i]);
			break;
		default:
			break;
		}
	}

	while (stackMathSymbol.size())
	{
		postfixExpressionToken.push_back(stackMathSymbol.top());
		stackMathSymbol.pop();
	}
	return;
}
/*---------------------------------------END /*Постфиксная запись*/
