#ifndef MINIRDBMS_TOKEN_H
#define MINIRDBMS_TOKEN_H

#include "TokenType.cpp"
#include <bits/stdc++.h>

using namespace std;

class Token {
public:
    TokenType type;
    string lexeme;
    string literal;

    Token(TokenType type, string lexeme, string literal);

    string toString();
    string getTokenType();
};


#endif //MINIRDBMS_TOKEN_H
