#ifndef MINIRDBMS_SQLINTERPRETER_H
#define MINIRDBMS_SQLINTERPRETER_H

#include <bits/stdc++.h>
#include "Scanner.h"
#include "Stmt.cpp"
#include "Parser.h"
#include <fstream>

using namespace std;

class SQLInterpreter {
    map<string, string> colTypes;
    vector<string> columns;
    int totalRows;
public:
    SQLInterpreter();
    void run(string query);
    void interpreteSelect(Select stmt);
    void interpretInsert(Insert stmt);
    void createTableMap(string tableName);
    void insertRow(string colName, string val, string type);
    void printRow(int index, vector<string> columns);
    bool doesRowSatisfy(int rowIdx, vector<Token> conditions);

    void interpreteCreateTable(CreateTable table);
};


#endif //MINIRDBMS_SQLINTERPRETER_H
