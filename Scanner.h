//
// Created by prabhav on 05/03/22.
//

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

public:
    Scanner(string query);
    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken();
    char peek();
};


#endif //MINIRDBMS_SCANNER_H
