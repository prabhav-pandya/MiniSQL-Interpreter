#include "SQLInterpreter.h"


SQLInterpreter::SQLInterpreter() {
    table = Table();
}

void SQLInterpreter::run(string query) {
    if (!checkForSemicolon(query)) {
        cerr << "Expected a ;" << endl;
        return;
    }

    Scanner scanner(query);
    vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);

    switch (tokens[0].type) {
        case CREATE : {
            CreateTable stmt = parser.parseCreateTable();
            interpretCreateTable(stmt);
            break;
        }

        case DROP : {
            DropTable stmt = parser.parseDropTable();
            interpretDropTable(stmt);
            break;
        }

        case SELECT : {
            Select stmt = parser.parseSelect();
            interpretSelect(stmt);
            break;
        }

        case INSERT : {
            Insert stmt = parser.parseInsert();
            interpretInsert(stmt);
            break;
        }

        case DELETE : {
            Delete stmt = parser.parseDelete();
            interpretDelete(stmt);
            break;
        }

        case UPDATE : {
            Update stmt = parser.parseUpdate();
            interpretUpdate(stmt);
            break;
        }

        case HELP: {
            Help stmt = parser.parseHelp();
            interpretHelp(stmt);
            break;
        }

        case QUIT: {
            exit(0);
        }

        default: {
            cerr << "Invalid Syntax!" << endl;
        }
    }
};

void SQLInterpreter::interpretCreateTable(CreateTable table) {
    fstream schema("../Relations/schema");
    string line;
    while (getline(schema, line)) {
        if (line[table.tableName.size()] != '#') continue;
        if (line.substr(0, table.tableName.size()) == table.tableName) {
            cerr << "Duplicate Table Name!" << endl;
            return;
        }
    }
    schema.close();

    fstream tableSchema("../Relations/schema", ios_base::app);
    tableSchema << endl << createSchema(table);
    tableSchema << endl << createDomainConstraints(table);
    tableSchema.close();

    fstream tableFile;
    tableFile.open("../Relations/" + table.tableName, ios::out);
    tableFile.close();

    cout << "Table Successfully Created" << endl;
}

string SQLInterpreter::createSchema(const CreateTable &table) {
    string schema = table.tableName;
    schema += "#";

    int numColumns = table.columnNames.size();
    for (int i = 0; i < numColumns; i++) {
        schema += table.columnNames[i];
        schema += "#";

        string type = table.columnTypes[i];
        transform(type.begin(), type.end(), type.begin(), ::toupper);
        schema += type;

        if (i < numColumns - 1) {
            schema += "#";
        }
    }

    return schema;
}

string SQLInterpreter::createDomainConstraints(CreateTable &table){
    string hashConstraint = "";
    for(auto constr: table.domainConstraints) hashConstraint += constr + "#";
    hashConstraint = hashConstraint.substr(0, hashConstraint.size()-1);
    return  hashConstraint;
}

void SQLInterpreter::interpretDropTable(DropTable stmt) {
    bool first = true;
    vector<string> lines;
    ifstream schema("../Relations/schema");
    for (string line; getline(schema, line); lines.push_back(line));
    schema.close();

    ofstream newSchema("../Relations/schema");

    bool isNextDomConstr = false;

    for (string line: lines) {

        if(isNextDomConstr){
            isNextDomConstr=false;
            continue;
        }

        if (line.substr(0, stmt.tableName.size()) == stmt.tableName
            && line[stmt.tableName.size()] == '#') {
            isNextDomConstr = true;
            continue;
        }

        if(first) {
            newSchema << line;
            first = false;
            continue;
        }

        newSchema << endl << line ;
    }
    newSchema.close();

    string filename = "../Relations/" + stmt.tableName;
    if (remove(filename.c_str()) != 0) {
        cerr << "Error in deleting file" << endl;
        return;
    }

    cout << "Table Dropped" << endl;
}

void SQLInterpreter::interpretDelete(Delete stmt){
    createTableMap(stmt.tableName);
    clearTable(stmt.tableName);

    if(stmt.conditions.empty()){
        cout<<table.totalRows<< " rows affected"<<endl;
        return;
    }

    ofstream tableFile("../Relations/"+stmt.tableName, ios::app);

    int rowsAffected = 0;

    for(int i=0;i<table.totalRows;i++){
        if(!doesRowSatisfy(i, stmt.conditions)){
            string row="";
            for(string column: table.columns){
                row += table.getElement(column, i) + "#";
            }
            row = row.substr(0, row.size()-1);
            tableFile<<row<<endl;
        }
        else rowsAffected++;
    }

    cout<<rowsAffected<<" rows affected"<<endl;
    tableFile.close();
}

void SQLInterpreter::interpretUpdate(Update stmt){
    createTableMap(stmt.tableName);

    ofstream tableRaw("../Relations/"+stmt.tableName, ios::app);

    int rowsAffected=0;

    for(int i=0;i<table.totalRows;i++){
        if(doesRowSatisfy(i, stmt.conditions)){
            rowsAffected++;
            for(auto itr: stmt.attrValues){
                table.updateTable(itr.first, i, itr.second);
            }
            if(!doesSatisfyConstraints(i)){
                cerr<<"Domain constraints not satisfied\n";
                return;
            }
        }
    }

    clearTable(stmt.tableName);

    for(int i=0;i<table.totalRows;i++){
        string row="";

        for (auto column: table.columns){
            row += table.getElement(column, i)+"#";
        }

        row = row.substr(0, row.size()-1);

        tableRaw<<row<<endl;
    }


    cout<<rowsAffected<<" rows affected"<<endl;
    tableRaw.close();
}

void SQLInterpreter::interpretSelect(Select stmt) {
    createTableMap(stmt.tableNames);

    vector<string> columnsToPrint;

    if (stmt.columnNames[0].compare("*") == 0) columnsToPrint = table.columns;
    else columnsToPrint = stmt.columnNames;

    for (int i = 0; i < columnsToPrint.size(); i++) {
        printWithSpaces(columnsToPrint[i]);
    }
    cout << endl;

    for (int i = 0; i < table.totalRows; i++) {
        if (doesRowSatisfy(i, stmt.condition)) {
            printRow(i, columnsToPrint);
        }
    }
}

void SQLInterpreter::interpretInsert(Insert stmt) {
    createTableMap(stmt.tableName);
    if (table.columns.size() != stmt.values.size()) {
        cerr << "Number of values don't match the columns in table: " + stmt.tableName << endl;
        exit(1);
    }

    fstream tableFile;
    tableFile.open("../Relations/" + stmt.tableName, ios::app);

    if (!tableFile.is_open()) {
        cerr << "No such table found";
        exit(1);
    }
    // insert values in table
    for(int i=0;i<table.columns.size();i++){

        // Check if datatype matches with value in the query
        if(table.colTypes[table.columns[i]]=="STR"){
            if(stmt.types[i]!=STRING){
                cerr<<"Value type doesn't match!"<<endl;
                return;
            }
        }
        if(table.colTypes[table.columns[i]]=="INT"){
            if(stmt.types[i]==STRING){
                cerr<<"Value type doesn't match!"<<endl;
                return;
            }
        }
        insertRow(table.columns[i], stmt.values[i], table.colTypes[table.columns[i]], table);
    }
    table.totalRows++;

    // check if values satisfy attribute constrains
    if(doesSatisfyConstraints(table.totalRows-1)){
        string rowDbFormat = "";

        for (string attr: stmt.values) rowDbFormat += attr + "#";

        rowDbFormat = rowDbFormat.substr(0, rowDbFormat.size() - 1);

        tableFile << rowDbFormat << endl;

        cout << "Table " + stmt.tableName + " updated" << endl;
    }
    else {
        cerr<<"Attribute constraints not satisfied!"<<endl;
    }

    tableFile.close();
}

void SQLInterpreter::interpretHelp(Help stmt) {
    vector<string> lines;
    ifstream schema("../Relations/schema");
    for (string line; getline(schema, line); lines.push_back(line));
    schema.close();

    switch (stmt.option) {
        case 1: {
            bool alt= true;
            for(string line : lines) {
                if(alt){
                    vector<string> splitTableInfo = splitHashStr(line);
                    cout<<splitTableInfo[0]<<": ";
                    for(int i=1;i<splitTableInfo.size()-1;i+=2){
                        cout<<splitTableInfo[i]<<" "<<splitTableInfo[i+1]<<", ";
                    }
                    cout<<endl;
                    alt = false;
                }
                else alt = true;
            }
            break;
        }

        case 2: {
            for(int i=0;i<lines.size()-1;i+=2) {
                string tableInfo = lines[i];
                if (tableInfo[stmt.tableName.size()] != '#') continue;
                if (tableInfo.substr(0, stmt.tableName.size()) == stmt.tableName) {
                    vector<string> tableInfoSplit = splitHashStr(tableInfo);
                    cout<<tableInfoSplit[0]<<": ";
                    for(int i=1;i<tableInfoSplit.size()-1;i+=2){
                        cout<<tableInfoSplit[i]<<" "<<tableInfoSplit[i+1]<<", ";
                    }
                    cout<<endl;
                    break;
                }
            }
            break;
        }

        case 3: {
            auto commandDescription = stmt.commandDescriptions.find(toLower(stmt.commandName));
            cout << commandDescription->second;
            break;
        }
    }
}

void SQLInterpreter::printRow(int index, vector<string> columns) {
    for (int i = 0; i < columns.size(); i++) {
        printWithSpaces(table.getElement(table.columns[i], index));
    }
    cout << endl;
}

void SQLInterpreter::printWithSpaces(const string &text) {
    int spacesLeft = table.minRowWidth - text.size();
    cout << text;

    for(int i = 0; i < spacesLeft; i++) {
        cout << " ";
    }
}
vector<string> SQLInterpreter::splitHashStr(string text) {
    vector<string> splitArray;
    string colName = "";
    for (int i = 0; i < text.size(); i++) {
        if (text[i] == '#') {
            splitArray.push_back(colName);

            if(colName.size() > table.minRowWidth) {
                table.minRowWidth = colName.size();
            }

            colName = "";
            continue;
        }
        colName += text[i];
    }
    splitArray.push_back(colName);
    return splitArray;
}

void SQLInterpreter::createTableMap(string tableName) {
    ifstream schema;
    schema.open("../Relations/schema");
    string line, tabStruct, constraintStr;


    while (schema) {
        getline(schema, line);
        if (line[tableName.size()] != '#') continue;
        if (line.substr(0, tableName.size()) == tableName) {
            tabStruct = line;
            getline(schema, constraintStr);
            break;
        }
    }
    // store attribute constraints
    table.constraints = createConstraints(constraintStr);

    vector<string> tableColType = splitHashStr(tabStruct);

    for (int i = 1; i < tableColType.size(); i += 2) {
        table.columns.push_back(tableColType[i]);
        table.colTypes[tableColType[i]] = tableColType[i + 1];
    }

    ifstream tableRaw;

    //cout<<tableName<<" "<<tableName.size();
    tableRaw.open("../Relations/" + tableName);
    string row;

    if(!tableRaw.is_open()){
        cerr<<"No such table exists"<<endl;
        exit(1);
    }

    while (getline(tableRaw, row)) {

//        if(row.empty()) continue;

        vector<string> rowSplit = splitHashStr(row);

        for (int i = 0; i < rowSplit.size(); i++) {
            insertRow(table.columns[i], rowSplit[i], table.colTypes[table.columns[i]], table);
        }

        table.totalRows++;
    }

    tableRaw.close();
    schema.close();
}

// overloaded function for select queries
void SQLInterpreter::createTableMap(vector<string> tableNames) {

    int colPad = 0;

    for(auto tableName: tableNames){
        ifstream schema;
        schema.open("../Relations/schema");
        string line, tabStruct, constraintStr;


        while (schema) {
            getline(schema, line);
            if (line[tableName.size()] != '#') continue;
            if (line.substr(0, tableName.size()) == tableName) {
                tabStruct = line;
                getline(schema, constraintStr);
                break;
            }
        }

        vector<string> tableColType = splitHashStr(tabStruct);

        // keep track of table and total columns before merging another table
        Table oldTable = table;
        colPad = table.columns.size();

        for (int i = 1; i < tableColType.size(); i += 2) {
            table.columns.push_back(tableColType[i]);
            table.colTypes[tableColType[i]] = tableColType[i + 1];
        }

        ifstream tableRaw;

        //cout<<tableName<<" "<<tableName.size();
        tableRaw.open("../Relations/" + tableName);
        string row;

        if(!tableRaw.is_open()){
            cerr<<"No such table exists"<<endl;
            exit(1);
        }

        while (getline(tableRaw, row)) {

           if(row.empty()) continue;

            vector<string> rowSplit = splitHashStr(row);


            if(table.getColSize(table.columns[colPad])==table.totalRows){
                for(int i=0;i<oldTable.totalRows;i++){
                    for(int j=0;j<oldTable.columns.size();j++){
                        insertRow(table.columns[j], table.getElement(table.columns[j], i), table.colTypes[table.columns[j]], table);
                    }
                    table.totalRows++;
                }
            }

            if(oldTable.totalRows!=0){
                for(int i=0;i<oldTable.totalRows;i++){
                    for(int j=0;j<rowSplit.size();j++){
                        insertRow(table.columns[colPad+j], rowSplit[j], table.colTypes[table.columns[colPad+j]], table);
                    }
                }
            }
            else{
                for(int i=0;i<rowSplit.size();i++){
                    insertRow(table.columns[colPad+i], rowSplit[i], table.colTypes[table.columns[colPad+i]], table);
                }
                table.totalRows++;
            }
        }

        tableRaw.close();
        schema.close();
    }

}

// Get a Table class object for specified table
Table SQLInterpreter::getTableMap(string tableName) {

    Table table;

    ifstream schema;
    schema.open("../Relations/schema");
    string line, tabStruct, constraintStr;


    while (schema) {
        getline(schema, line);
        if (line[tableName.size()] != '#') continue;
        if (line.substr(0, tableName.size()) == tableName) {
            tabStruct = line;
            getline(schema, constraintStr);
            break;
        }
    }
    // store attribute constraints
    table.constraints = createConstraints(constraintStr);

    vector<string> tableColType = splitHashStr(tabStruct);

    for (int i = 1; i < tableColType.size(); i += 2) {
        table.columns.push_back(tableColType[i]);
        table.colTypes[tableColType[i]] = tableColType[i + 1];
    }

    ifstream tableRaw;

    //cout<<tableName<<" "<<tableName.size();
    tableRaw.open("../Relations/" + tableName);
    string row;

    if(!tableRaw.is_open()){
        cerr<<"No such table exists"<<endl;
        exit(1);
    }

    while (getline(tableRaw, row)) {

//        if(row.empty()) continue;

        vector<string> rowSplit = splitHashStr(row);

        for (int i = 0; i < rowSplit.size(); i++) {
            insertRow(table.columns[i], rowSplit[i], table.colTypes[table.columns[i]], table);
        }

        table.totalRows++;
    }

    tableRaw.close();
    schema.close();

    return table;
}

void SQLInterpreter::insertRow(string colName, string val, string type, Table &table) {
    table.addElement(colName, val);
}

// this function is only for specific conditional queries
// THis function makes me wanna cry
bool SQLInterpreter::doesRowSatisfy(int rowIdx, vector<Token> conditions) {
    // For simplicity we're assuming that there will be same operator in a single condition (AND/OR)
    if (conditions.size() == 0) return true;
    int condIdx = 0;
    int res = -1;
    TokenType opr = NULL_TOKEN; // stores the last operator (AND/OR)
    while (condIdx < conditions.size()) {

        if(conditions[condIdx].type==PRIMARY){
            condIdx++;
            continue;
        }

        string attrName = conditions[condIdx].lexeme;
        if (table.colTypes[attrName].compare("INT") == 0) {

            int val = stoi(table.getElement(attrName, rowIdx));
            condIdx++;
            TokenType compareOp = conditions[condIdx].type;
            condIdx++;

            int compareVal;
            if(isalphanum(conditions[condIdx].lexeme)){
                compareVal = stoi(table.getElement(conditions[condIdx].lexeme, rowIdx));
            }
            else compareVal = stoi(conditions[condIdx].lexeme);

            if (compareOp == GREATER_EQUAL) {
                if (val >= compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == GREATER) {
                if (val > compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == EQUAL) {
                if (val == compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } if (compareOp == BANG_EQUAL) {
                if (val != compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == LESS) {
                if (val < compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == LESS_EQUAL) {
                if (val <= compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            }
        } else if (table.colTypes[attrName].compare("STR") == 0) {
            string val = table.getElement(attrName, rowIdx);
            condIdx++;
            TokenType compareOp = conditions[condIdx].type;
            condIdx++;

            string compareVal;
            if(conditions[condIdx].type!=STRING){
                compareVal = table.getElement(conditions[condIdx].literal, rowIdx);
            }
            else compareVal = conditions[condIdx].literal;

            if (compareOp == GREATER_EQUAL) {
                if (val.compare(compareVal) >= 0) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == GREATER) {
                if (val.compare(compareVal) > 0) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == EQUAL) {
                if (val.compare(compareVal) == 0) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } if (compareOp == BANG_EQUAL) {
                if (val.compare(compareVal)!=0) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == LESS) {
                if (val.compare(compareVal) < 0) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            }
            if (compareOp == LESS_EQUAL) {
                if (val.compare(compareVal) > 0) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            }
        }
        else if (table.colTypes[attrName].compare("DEC") == 0) {

            double val = stod(table.getElement(attrName, rowIdx));
            condIdx++;
            TokenType compareOp = conditions[condIdx].type;
            condIdx++;

            double compareVal;
            if(isalphanum(conditions[condIdx].lexeme)){
                compareVal = stod(table.getElement(conditions[condIdx].lexeme, rowIdx));
            }
            else compareVal = stod(conditions[condIdx].lexeme);

            if (compareOp == GREATER_EQUAL) {
                if (val >= compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == GREATER) {
                if (val > compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == EQUAL) {
                if (val == compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } if (compareOp == BANG_EQUAL) {
                if (val != compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == LESS) {
                if (val < compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            } else if (compareOp == LESS_EQUAL) {
                if (val <= compareVal) {
                    if (res == -1) res = 1;
                    if (opr == AND) {
                        res *= 1;
                    } else res = 1;
                    if (opr == OR && res == 1) return true;
                } else {
                    if (res == -1) res = 0;
                    if (opr == AND) {
                        res *= 0;
                    }
                    if (opr == AND && res == 0) return false;
                }
            }
        }
        condIdx++;
        opr = conditions[condIdx].type;
        condIdx++;
    }
    if (res == 1) return true;
    else return false;
}

vector<vector<Token>> SQLInterpreter::createConstraints(string constraintStr) {
    Scanner scanner(constraintStr);
    vector<Token> constrTokens = scanner.scanTokens();
    vector<Token> attrConstr;

    vector<vector<Token>> constraints;

    for(auto token: constrTokens){
        if(token.type==HASH){
            constraints.push_back(attrConstr);
            attrConstr = vector<Token>();
            continue;
        }
        attrConstr.push_back(token);
    }
    constraints.push_back(attrConstr);
    return constraints;
}

bool SQLInterpreter::checkPrimary(int rowIdx, string col){
    string val = table.getElement(col, rowIdx);

    for(int i=0;i<table.totalRows;i++){
        if(i==rowIdx) continue;
        string rowVal = table.getElement(col, i);
        if(rowVal==val) return false;
    }
    return true;
}

bool SQLInterpreter::checkForeignKeyExists(int rowIdx, string col, string tableName, string attribute){
    Table foreignTable = getTableMap(tableName);
    string keyVal = table.getElement(col, rowIdx);
    for(int i=0;i<foreignTable.totalRows;i++){
        if(keyVal==foreignTable.getElement(attribute, i)) return true;
    }
    return false;
}

bool SQLInterpreter::doesSatisfyConstraints(int rowIdx) {
    for(int i=0;i<table.constraints.size();i++){
        // check if foreign key & its satisfied
        if(table.constraints[i].size()>0 && table.constraints[i][0].type==FOREIGN){
            string tableName = table.constraints[i][1].lexeme;
            string attributeName = table.constraints[i][2].lexeme;
            if(!checkForeignKeyExists(rowIdx, table.columns[i], tableName, attributeName)) return false;
            else return true;
        }
        // check if primary key & its satisfied
        for(auto constr: table.constraints[i]) if(constr.type==PRIMARY){
                if(!checkPrimary(rowIdx, table.columns[i])) return false;
                else return true;
        }
        // check for other conditions
        if(!doesRowSatisfy(rowIdx, table.constraints[i])) return false;
    }
    return true;
}

bool SQLInterpreter::checkForSemicolon(string query) {
    if (query[query.size() - 1] != ';') return false;
    else return true;
}

string SQLInterpreter::toLower(string str){
    string res = "";
    for(char c: str){
        if(c==' ') continue;
        res += tolower(c);
    }
    return res;
}


bool SQLInterpreter::isalphanum(string str) {
    for(auto c: str){
        if(isalpha(c)) return true;
    }
    return false;
}

void SQLInterpreter::clearTable(string tableName){
    ofstream table("../Relations/"+tableName);
    table<<"";
    table.close();
}