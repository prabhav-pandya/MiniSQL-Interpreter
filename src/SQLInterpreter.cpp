#include "SQLInterpreter.h"

template<typename T>
map<string, vector<T>> table;

SQLInterpreter::SQLInterpreter() {
    totalRows = 0;
}

void SQLInterpreter::run(string query) {
    if (!checkForSemicolon(query)) {
        cerr << "Expected a Semicolon!" << endl;
        return;
    }

    Scanner scanner(query);
    vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);

    switch (tokens[0].type) {
        case CREATE : {
            CreateTable stmt = parser.parseCreateTable();
            interpreteCreateTable(stmt);
            break;
        }

        case SELECT : {
            Select stmt = parser.parseSelect();
            interpreteSelect(stmt);
            break;
        }

        case INSERT : {
            Insert stmt = parser.parseInsert();
            interpretInsert(stmt);
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

void SQLInterpreter::interpreteCreateTable(CreateTable stmt) {
    fstream schema("../Relations/schema", ios_base::app);
    schema << endl << createSchema(stmt);
    schema.close();

    // TODO: check for duplicate table name

    fstream table;
    table.open("../Relations/" + stmt.tableName, ios::out);
    table << ' ' << endl;
    table.close();
}

string SQLInterpreter::createSchema(const CreateTable& table) {
    string schema = table.tableName;
    schema += "#";

    int numColumns = table.columnNames.size();
    for(int i = 0; i < numColumns; i++) {
        schema += table.columnNames[i];
        schema += "#";

        string type = table.columnTypes[i];
        transform(type.begin(), type.end(), type.begin(), ::toupper);
        schema += type;

        if(i < numColumns - 1) {
            schema += "#";
        }
    }

    return schema;
}

void SQLInterpreter::interpreteSelect(Select stmt) {
    createTableMap(stmt.tableName);

    vector<string> columnsToPrint;

    if (stmt.columnNames[0].compare("*") == 0) columnsToPrint = columns;
    else columnsToPrint = stmt.columnNames;

    for (int i = 0; i < columnsToPrint.size(); i++) {
        cout << columnsToPrint[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < totalRows; i++) {
        if (doesRowSatisfy(i, stmt.condition)) {
            printRow(i, columnsToPrint);
        }
    }

}

void SQLInterpreter::interpretInsert(Insert stmt) {
    createTableMap(stmt.tableName);
    if(columns.size()!=stmt.values.size()){
        cerr<<"Number of values don't match the columns in table: " + stmt.tableName<<endl;
        exit(1);
    }

    fstream table;
    table.open("../Relations/"+stmt.tableName, ios::app);

    if(!table.is_open()){
        cerr << "No such table found";
        exit(1);
    }

    string rowDbFormat="";

    for(string attr: stmt.values) rowDbFormat += attr + "#";

    rowDbFormat = rowDbFormat.substr(0, rowDbFormat.size()-1);

    table << rowDbFormat <<endl;

    cout<<"Table " + stmt.tableName + " updated"<<endl;
    table.close();
}

void SQLInterpreter::printRow(int index, vector<string> columns) {
    for (int i = 0; i < columns.size(); i++) {
        if (colTypes[columns[i]].compare("INT") == 0) {
            cout << table<int>[columns[i]][index] << " ";
        } else if (colTypes[columns[i]].compare("STR") == 0) {
            cout << table<string>[columns[i]][index] << " ";
        }
    }
    cout << endl;
}

vector<string> splitHashStr(string text) {
    vector<string> splitArray;
    string colName = "";
    for (int i = 0; i < text.size(); i++) {
        if (text[i] == '#') {
            splitArray.push_back(colName);
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
    string line, tabStruct;

    while (schema) {
        //cout<<line;
        getline(schema, line);
        if (line.substr(0, tableName.size()) == tableName) {
            tabStruct = line;
            break;
        }
    }
    //cout<<tabStruct<<endl;
    vector<string> tableColType = splitHashStr(tabStruct);

    for (int i = 1; i < tableColType.size(); i += 2) {
        columns.push_back(tableColType[i]);
        colTypes[tableColType[i]] = tableColType[i + 1];
    }

    ifstream tableRaw;

    //cout<<tableName<<" "<<tableName.size();
    tableRaw.open("../Relations/" + tableName);
    string row;

    while (getline(tableRaw, row)) {

//        if(row.empty()) continue;

        vector<string> rowSplit = splitHashStr(row);

        for (int i = 0; i < rowSplit.size(); i++) {
            insertRow(columns[i], rowSplit[i], colTypes[columns[i]]);
        }

        totalRows++;
    }

    tableRaw.close();
    schema.close();
}

void SQLInterpreter::insertRow(string colName, string val, string type) {
    if (type == "INT") {
        table<int>[colName].push_back(stoi(val));
    }

    if (type == "STR") {
        table<string>[colName].push_back(val);
    }
}

// this function is only for specific conditional queries
// this code must never be used in industry systems!
bool SQLInterpreter::doesRowSatisfy(int rowIdx, vector<Token> conditions) {
    // For simplicity we're assuming that there will be same operator in a single condition (AND/OR)
    if (conditions.size() == 0) return true;
    int condIdx = 0;
    int res = -1;
    TokenType opr = NULL_TOKEN; // stores the last operator (AND/OR)
    while (condIdx < conditions.size()) {
        string attrName = conditions[condIdx].lexeme;
        if (colTypes[attrName].compare("INT") == 0) {
            int val = table<int>[attrName][rowIdx];
            condIdx++;
            if (conditions[condIdx].type == GREATER_EQUAL) {
                condIdx++;
                if (val >= stoi(conditions[condIdx].lexeme)) {
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
            } else if (conditions[condIdx].type == GREATER) {
                condIdx++;
                if (val > stoi(conditions[condIdx].lexeme)) {
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
            } else if (conditions[condIdx].type == EQUAL) {
                condIdx++;
                if (val == stoi(conditions[condIdx].lexeme)) {
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
            } else if (conditions[condIdx].type == LESS) {
                condIdx++;
                if (val < stoi(conditions[condIdx].lexeme)) {
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
            } else if (conditions[condIdx].type == LESS_EQUAL) {
                condIdx++;
                if (val <= stoi(conditions[condIdx].lexeme)) {
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
        } else if (colTypes[attrName].compare("STR") == 0) {
            string val = table<string>[attrName][rowIdx];
            condIdx++;
            if (conditions[condIdx].type == GREATER_EQUAL) {
                condIdx++;
                if (val.compare(conditions[condIdx].literal) >= 0) {
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
            } else if (conditions[condIdx].type == GREATER) {
                condIdx++;
                if (val.compare(conditions[condIdx].literal) > 0) {
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
            } else if (conditions[condIdx].type == EQUAL) {
                condIdx++;
                if (val.compare(conditions[condIdx].literal) == 0) {
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
            } else if (conditions[condIdx].type == LESS) {
                condIdx++;
                if (val.compare(conditions[condIdx].literal) < 0) {
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
            if (conditions[condIdx].type == LESS_EQUAL) {
                condIdx++;
                if (val.compare(conditions[condIdx].literal) > 0) {
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

bool SQLInterpreter::checkForSemicolon(string query) {
    if (query[query.size() - 1] != ';') return false;
    else return true;
}
