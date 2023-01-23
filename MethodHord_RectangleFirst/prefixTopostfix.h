#pragma once
#ifndef prefixTopostfix_h
#include <vector>
#include <string>
#include <set>

using namespace std;

struct token;
typedef vector<token> TOKENS;

//������� ��������� ��������� � ������ "expr" � ���� ���������� ��� �������
void createSetOfDelimiters(set<char>&);
bool isDelimiter(char, set<char>);
//������� ����������� �� ������
void createTokensFromExpression(string& expr, TOKENS& tokExpr);
void createPriority(map<string, int>&);
bool isCloseBreketsOrOperation(TOKENS, __int64 );
bool isOpenBrekets(TOKENS , __int64 );
/*����������� ������*/
void createPostfixFromToken(TOKENS& tokExpr, TOKENS& pExpr);


#endif // !prefixTopostfix_h

