#pragma once
#ifndef prefixTopostfix_h
#include <vector>
#include <string>
#include <set>

using namespace std;

struct token;
typedef vector<token> TOKENS;

//Функция считывает выражение в строку "expr" и ищем переменные или массивы
void createSetOfDelimiters(set<char>&);
bool isDelimiter(char, set<char>);
//Функция расчленения на токены
void createTokensFromExpression(string& expr, TOKENS& tokExpr);
void createPriority(map<string, int>&);
bool isCloseBreketsOrOperation(TOKENS, __int64 );
bool isOpenBrekets(TOKENS , __int64 );
/*Постфиксная запись*/
void createPostfixFromToken(TOKENS& tokExpr, TOKENS& pExpr);


#endif // !prefixTopostfix_h

