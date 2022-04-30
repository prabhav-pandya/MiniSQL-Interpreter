#ifndef MINIRDBMS_SQLINTERPRETER_H
#define MINIRDBMS_SQLINTERPRETER_H

#include <bits/stdc++.h>
#include "Scanner.h"
#include "Stmt.cpp"
#include "Parser.h"
#include <fstream>
#include "Table.h"

using namespace std;

class SQLInterpreter {
    Table table;

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
    void createTableMap(vector<string> tableNames);

    void insertRow(string colName, string val, string type);
    void printRow(int index, vector<string> columns);
    void printWithSpaces(const string &text);
    bool doesRowSatisfy(int rowIdx, vector<Token> conditions);
    void clearTable(string tableName);

    bool doesSatisfyConstraints(int rowIdx);
    void createConstraints(string constraintStr);

    bool checkForSemicolon(string basicString);
    string toLower(string str);
    bool isalphanum(string str);

    string createSchema(const CreateTable& table);
    string createDomainConstraints(CreateTable &table);
    vector<string> splitHashStr(string text);
    void interpretHelp(Help help);
};


#endif //MINIRDBMS_SQLINTERPRETER_H
