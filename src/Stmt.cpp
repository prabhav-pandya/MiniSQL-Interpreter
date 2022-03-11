#ifndef MINIRDBMS_STMT
#define MINIRDBMS_STMT

#include <bits/stdc++.h>
#include "Token.h"

using namespace std;

class Stmt {

};

class CreateTable : public Stmt {
public:
    string tableName;
    vector<string> columnNames;
    vector<string> columnTypes;

    CreateTable(string tableName, vector<string> columnNames, vector<string> columnTypes) {
        this->tableName = tableName;
        this->columnNames = columnNames;
        this->columnTypes = columnTypes;
    }
};

class DropTable : public Stmt {
public:
    string tableName;

    DropTable(string tableName) {
        this->tableName = tableName;
    }
};

class Select : public Stmt {
public:
    string tableName;
    vector<string> columnNames;
    vector<Token> condition;

    Select(string tableName, vector<string> columnNames, vector<Token> condition) {
        this->tableName = tableName;
        this->columnNames = columnNames;
        this->condition = condition;
    }
};

class Insert : public Stmt {
public:
    string tableName;
    vector<string> values;

    Insert(string tableName, vector<string> vals) {
        this->tableName = tableName;
        this->values = vals;
    }
};

class Help : public Stmt {
public:
    // 1 -> HELP TABLES -> List all Tables
    // 2 -> HELP DESCRIBE <TABLE_NAME> -> Print schema of TABLE_NAME
    // 3 -> HELP <COMMAND> -> Print description of Command
    int option = 0;

    string tableName;
    string commandName;

    map<string, string> commandDescriptions;

    Help(int option, string tableName, string commandName) {
        this->option = option;
        this->tableName = move(tableName);
        this->commandName = move(commandName);

        if (option == 3) populateDictionary();
    }

private:
    void populateDictionary() {
        commandDescriptions.insert(pair<string, string>(
                "create",
                "Usage: CREATE TABLE <TABLE_NAME> (attr1 type1, attr2 type2,...);\nCreates table with the specified attributes\n\n"
        ));

        commandDescriptions.insert(pair<string, string>(
                "drop",
                "Usage: DROP TABLE <TABLE_NAME>;\nDrops the specified table\n\n"
        ));

        commandDescriptions.insert(pair<string, string>(
                "select",
                "Usage: SELECT <ATTR_LIST> FROM <TABLE_NAME> WHERE <CONDITION_LIST>;\nSelects the attributes specified from the \n\n"
        ));

        commandDescriptions.insert(pair<string, string>(
                "insert",
                "Usage: INSERT INTO <TABLE_NAME> VALUES(VAL1, VAL2,...);\n\n"
        ));

        commandDescriptions.insert(pair<string, string>(
                "update",
                "Usage: UPDATE <TABLE_NAME> SET ATTR1 = VAL1, ATTR2 = VAL2,... WHERE <CONDITION_LIST>\n\n"
        ));

        commandDescriptions.insert(pair<string, string>(
                "delete",
                "Usage: DELETE FROM <TABLE_NAME> WHERE <CONDITION_LIST>;\n\n"
        ));

        commandDescriptions.insert(pair<string, string>(
                "help",
                "Usage: HELP TABLES;\nDisplays all tables currently created\n\nUsage: HELP DESCRIBE <TABLE_NAME>\nPrints schema of TABLE_NAME\n\nUsage: HELP <COMMAND>\nPrint description of Command\n\n"
        ));

        commandDescriptions.insert(pair<string, string>(
                "quit",
                "Usage: QUIT;\nQuits the interactive shell.\n\n"
        ));
    }
};

#endif