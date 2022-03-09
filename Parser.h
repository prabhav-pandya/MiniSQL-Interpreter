#ifndef MINIRDBMS_PARSER_H
#define MINIRDBMS_PARSER_H

#include <bits/stdc++.h>
#include "Token.h"
#include "Stmt.cpp"

class Parser {
 vector<Token> tokens;
 int current = 0;
public:
    Parser(vector<Token> tokensList);
    Stmt parse();
    Select parseSelect();
    Token peek();
    Token advance();
    bool isAtEnd();
    void consume(TokenType expected, string error);
};


#endif //MINIRDBMS_PARSER_H
