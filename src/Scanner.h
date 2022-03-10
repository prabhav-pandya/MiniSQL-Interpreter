#ifndef MINIRDBMS_SCANNER_H
#define MINIRDBMS_SCANNER_H

#include <bits/stdc++.h>
#include "Token.h"

using namespace std;

class Scanner {
    string query;
    int start = 0;
    int current = 0;
    vector<Token> tokens;

    map<string, TokenType> keywords;

public:
    Scanner(string query);
    bool isAtEnd();
    void scanToken();
    vector<Token> scanTokens();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, string literal);
    char peek();
    bool match(char expected);
    void value();
    void identifier();
};


#endif //MINIRDBMS_SCANNER_H
