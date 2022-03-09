#include "SQLInterpreter.h"

template<typename T>
map<string, vector<T>> table;

SQLInterpreter::SQLInterpreter() {
    totalRows=0;
}

void SQLInterpreter::run(string query){
    Scanner scanner(query);
    vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);

    if(tokens[0].type==SELECT){
        Select stmt = parser.parseSelect();
        interpreteSelect(stmt);
    }



};

void SQLInterpreter::interpreteSelect(Select stmt){
    createTableMap(stmt.tableName);

    vector<string> columnsToPrint;

    if(stmt.columnNames[0].compare("*")==0) columnsToPrint = columns;
    else columnsToPrint = stmt.columnNames;

    for(int i=0;i<columnsToPrint.size();i++){
        cout<<columnsToPrint[i]<<" ";
    }
    cout<<endl;

    for(int i=0;i<totalRows;i++){
        printRow(i, columnsToPrint);
    }

}

void SQLInterpreter::printRow(int index, vector<string> columns) {
    for(int i=0;i<columns.size();i++){
        if(colTypes[columns[i]].compare("INT")==0){
            cout<<table<int>[columns[i]][index]<<" ";
        }
        else if(colTypes[columns[i]].compare("STR")==0){
            cout<<table<string>[columns[i]][index]<<" ";
        }
    }
    cout<<endl;
}

vector<string> splitHashStr(string text){
    vector<string> splitArray;
    string colName = "";
    for(int i = 0; i< text.size(); i++){
        if(text[i]=='#'){
            splitArray.push_back(colName);
            colName="";
            continue;
        }
        colName+=text[i];
    }
    splitArray.push_back(colName);
    return splitArray;
}

void SQLInterpreter::createTableMap(string tableName) {
    ifstream schema;
    schema.open("/home/prabhav/Desktop/College/DBMS/MiniRDBMS/Database Files/schema");
    string line, tabStruct;
    while(schema){
        //cout<<line;
        getline(schema, line);
        if(line.substr(0, tableName.size())==tableName){
            tabStruct=line;
            break;
        }
    }
    //cout<<tabStruct<<endl;
    vector<string> tableColType = splitHashStr(tabStruct);

    for(int i=1;i<tableColType.size();i+=2){
        columns.push_back(tableColType[i]);
        colTypes[tableColType[i]] = tableColType[i+1];
    }

    ifstream tableRaw;
    //cout<<tableName<<" "<<tableName.size();
    tableRaw.open("/home/prabhav/Desktop/College/DBMS/MiniRDBMS/Relations/"+tableName);
    string row;
    while(tableRaw){
        getline(tableRaw, row);
        vector<string> rowSplit = splitHashStr(row);
        for(int i=0;i<rowSplit.size();i++){
            insertRow(columns[i], rowSplit[i], colTypes[columns[i]]);
        }
        totalRows++;
    }

    tableRaw.close();
    schema.close();
}

void SQLInterpreter::insertRow(string colName, string val, string type) {
    if(type=="INT"){
        table<int>[colName].push_back(stoi(val));
    }
    if(type=="STR"){
        table<string>[colName].push_back(val);
    }
}