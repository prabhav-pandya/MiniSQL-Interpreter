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
    vector<vector<Token>> constraints;
    int totalRows;
public:
    SQLInterpreter();
    void run(string query);
    void interpretSelect(Select stmt);
    void interpretInsert(Insert stmt);
    void interpretCreateTable(CreateTable table);
    void interpretDropTable(DropTable table);
    void interpretDelete(Delete stmt);
    void interpretUpdate(Update stmt);

    void createTableMap(string tableName);
    void insertRow(string colName, string val, string type);
    void printRow(int index, vector<string> columns);
    bool doesRowSatisfy(int rowIdx, vector<Token> conditions);
    void clearTable(string tableName);

    bool doesSatisfyConstraints(int rowIdx);
    void createConstraints(string constraintStr);

    bool checkForSemicolon(string basicString);
    string toLower(string str);

    string createSchema(const CreateTable& table);

    void interpretHelp(Help help);
};


#endif //MINIRDBMS_SQLINTERPRETER_H
