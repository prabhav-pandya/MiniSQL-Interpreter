#ifndef MINIRDBMS_STMT
#define MINIRDBMS_STMT

#include <bits/stdc++.h>
#include "Token.h"

using namespace std;

class Stmt{

};

class CreateTable: public Stmt{
public:
    string tableName;
    vector<string> columnNames;
    vector<string> columnTypes;

    CreateTable(string tableName, vector<string> columnNames, vector<string> columnTypes){
        this->tableName = tableName;
        this->columnNames = columnNames;
        this->columnTypes = columnTypes;
    }
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

class Insert: public Stmt{
public:
    string tableName;
    vector<string> values;

    Insert(string tableName, vector<string> vals){
        this->tableName = tableName;
        this->values = vals;
    }
};

#endif