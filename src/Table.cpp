#include "Table.h"

Table::Table() {
    minRowWidth=10;
    totalRows=0;
}

Table::Table(const Table &t) {
    columns = t.columns;
    colTypes = t.colTypes;
    tableName = t.tableName;

    intTable = t.intTable;
    decTable = t.decTable;
    strTable = t.strTable;

    minRowWidth = t.minRowWidth;
    totalRows = t.totalRows;
    constraints = t.constraints;
}

void Table::addElement(string column, string val){
    if(colTypes[column].compare("INT")==0){
        intTable[column].push_back(stoi(val));
    }
    else if(colTypes[column].compare("STR")==0){
        strTable[column].push_back(val);
    }
    else if(colTypes[column].compare("DEC")==0){
        decTable[column].push_back(stod(val));
    }
}

string Table::getElement(string column, int index) {
    if(colTypes[column].compare("INT")==0){
        return to_string(intTable[column][index]);
    }
    else if(colTypes[column].compare("STR")==0){
        return strTable[column][index];
    }
    else if(colTypes[column].compare("DEC")==0){
        return to_string(decTable[column][index]);
    }
}

void Table::updateTable(string column, int index, string newVal) {
    if(colTypes[column].compare("INT")==0){
        intTable[column][index] = stoi(newVal);
    }
    else if(colTypes[column].compare("STR")==0){
        strTable[column][index] = newVal;
    }
    else if(colTypes[column].compare("DEC")==0){
        decTable[column][index] = stod(newVal);
    }
}

int Table::getColSize(string column) {
    if(colTypes[column].compare("INT")==0){
        return intTable[column].size();
    }
    else if(colTypes[column].compare("STR")==0){
        return strTable[column].size();
    }
    else if(colTypes[column].compare("DEC")==0){
        return decTable[column].size();
    }
}