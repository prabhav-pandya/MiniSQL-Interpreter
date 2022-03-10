#ifndef MINIRDBMS_STMT
#define MINIRDBMS_STMT

#include <bits/stdc++.h>
#include "Token.h"

using namespace std;

class Stmt{

};

class Select: public Stmt{
public:
    string tableName;
    vector<string> columnNames;
    vector<Token> condition;

    Select(string tableName, vector<string> columnNames, vector<Token> condition){
        this->tableName = tableName;
        this->columnNames = columnNames;
        this->condition = condition;
    }
};

#endif