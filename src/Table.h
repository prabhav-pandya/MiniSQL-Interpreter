#ifndef MINIRDBMS_TABLE_H
#define MINIRDBMS_TABLE_H

#include <bits/stdc++.h>
#include "Token.h"

using namespace std;

class Table {
public:
    string tableName;
    vector<string> columns;
    map<string, string> colTypes;

    map<string, vector<int>> intTable;
    map<string, vector<double>> decTable;
    map<string, vector<string>> strTable;

    vector<vector<Token>> constraints;
    int totalRows;
    int minRowWidth;

    Table();

    void addElement(string column, string val);
    void updateTable(string column, int index, string newVal);
    string getElement(string column, int index);

};


#endif //MINIRDBMS_TABLE_H
