#include "Parser.h"

Parser::Parser(vector<Token> tokensList) {
    current=0;
    for (auto token: tokensList) {
        tokens.push_back(token);
    }
}

Stmt Parser::parse() {
    return Stmt();
}

CreateTable Parser::parseCreateTable() {
    string tableName;
    vector<string> columnNames;
    vector<string> columnTypes;
    vector<string> domainConstraints;

    consume(CREATE, "Expected Token Create!");
    consume(TABLE, "Expected Token Table!");

    // consume table name
    if (peek().type == IDENTIFIER) tableName = peek().lexeme;
    else cerr << "Table name not specified!" << endl;
    advance();

    consume(LEFT_PAREN, "Expected a '('!");

    do {
        if (peek().type == IDENTIFIER) {
            columnNames.push_back(peek().lexeme);
            advance();

            if (peek().type == TYPE) {
                columnTypes.push_back(peek().lexeme);
                advance();
            }

            // domain constraints
            if(peek().type == CHECK){
                consume(CHECK, "");
                string constraint = "";
                while(peek().type!=COMMA && peek().type!=RIGHT_PAREN){
                    if(peek().type==AND || peek().type==OR){
                        constraint += peek().type==AND?" AND ":" OR ";
                    }
                    else constraint += peek().lexeme;
                    advance();
                }
                domainConstraints.push_back(constraint);
            }

            if (peek().type != RIGHT_PAREN) consume(COMMA, "Expected a Comma!");
        } else {
            cerr << "Invalid Column Name!";
        }
    } while (peek().type != RIGHT_PAREN);


    consume(RIGHT_PAREN, "Expected a ')'!");
//    consume(SEMICOLON, "Expected a ';'!");

    return CreateTable(tableName, columnNames, columnTypes, domainConstraints);
}

DropTable Parser::parseDropTable() {
    string tableName;

    consume(DROP, "Expected Token DROP!");
    consume(TABLE, "Expected Token Table!");

    // consume table name
    if (peek().type == IDENTIFIER) tableName = peek().lexeme;
    else cerr << "Table name not specified!" << endl;
    advance();

    return DropTable(tableName);
}

Insert Parser::parseInsert() {
    // consume INSERT INTO
    consume(INSERT, "Expected Token Insert");
    consume(INTO, "Expected Token Into");

    // read table name
    string tableName;

    if (peek().type == IDENTIFIER) {
        tableName = peek().lexeme;
        advance();
    } else cerr << "Expected table name";

    // consume VALUES
    consume(VALUES, "Expected Token Values");
    consume(LEFT_PAREN, "Expected Parenthesis");

    // consume values
    vector<string> values;
    while (peek().type != RIGHT_PAREN) {
        if (peek().type == IDENTIFIER || peek().type == STRING) {
            values.push_back(peek().type == IDENTIFIER ? peek().lexeme : peek().literal);
            advance();
        } else {
            cerr << "Expected values!";
            exit(1);
        }

        if (peek().type == COMMA) {
            consume(COMMA, "");
        } else break;
    }

    consume(RIGHT_PAREN, "Expected )");

    consume(SEMICOLON, "Expected ;");

    return Insert(tableName, values);

}

Select Parser::parseSelect() {
    // consume SELECT
    consume(SELECT, "Expected Token Select!");

    // consume and store column names or *
    bool toConsume = true;
    vector<string> columns;
    string tableName;

    if (peek().type == STAR) {
        columns.push_back("*");
        advance();
    } else {
        while (toConsume) {
            columns.push_back(peek().lexeme);
            advance();
            toConsume = false;
            if (peek().type == COMMA) {
                consume(COMMA, "Expected Token Comma!");
                toConsume = true;
            }
        }
    }

    // consume keyword FROM
    consume(FROM, "Syntax error");

    // consume table name
    if (peek().type == IDENTIFIER) tableName = peek().lexeme;
    else cerr << "Table name not specified";
    advance();

    vector<Token> condition = parseConditions();

    consume(SEMICOLON, "Expected ;");

    return Select(tableName, columns, condition);
}

Delete Parser::parseDelete(){
    // consume DELETE & FROM
    consume(DELETE, "Delete token expected");
    consume(FROM, "FROM token expected");

    // consume table name
    string tableName;
    if(peek().type==IDENTIFIER){
        tableName = peek().lexeme;
        advance();
    }
    else {
        consume(IDENTIFIER, "Expected table name");
    }

    vector<Token> conditions = parseConditions();

    consume(SEMICOLON, "Expected ;");

    return Delete(tableName, conditions);
}

Update Parser::parseUpdate(){
    // UPDATE table_name SET attr1 = val1, attr2 = val2… WHERE condition_list;
    // consume UPDATE
    consume(UPDATE, "Update token expected");

    // consume table name
    string tableName;
    if(peek().type==IDENTIFIER) tableName = peek().lexeme;
    else consume(IDENTIFIER, "Expected table name!");
    advance();
    // consume SET
    consume(SET, "Set token expected");

    map<string, string> attrValues;
    while(peek().type!=WHERE){
        string attribute, value;
        if(peek().type==IDENTIFIER){
            attribute = peek().lexeme;
            advance();
        }
        else consume(IDENTIFIER, "Syntax error");

        consume(EQUAL, "Syntax error");

        if(peek().type==IDENTIFIER || peek().type==STRING){
            value = peek().type==IDENTIFIER? peek().lexeme:peek().literal;
            advance();
        } else {
            cerr << "Unexpected value provided";
            exit(1);
        }
        attrValues[attribute] = value;

        if(peek().type==COMMA){
            consume(COMMA, "");
        }
        else break;
    }

    vector<Token> conditions = parseConditions();

    consume(SEMICOLON, "Expected ;");

    return Update(tableName, conditions, attrValues);
}

Help Parser::parseHelp() {
    string tableName;
    string commandName;
    consume(HELP, "Expected token HELP!");

    switch (peek().type) {
        case TABLES: {
            consume(TABLES, "Expected token TABLES!");
            return Help(1, "", "");
            break;
        }

        case DESCRIBE: {
            consume(DESCRIBE, "Expected token DESCRIBE!");

            if (peek().type == IDENTIFIER) tableName = peek().lexeme;
            else cerr << "Table name not specified!" << endl;

            advance();
            return Help(2, tableName, "");
        }

        default: {
            if (peek().literal == "") {
                cerr << "Invalid Keyword!" << endl;
                exit(1);
            }

            commandName = peek().literal;
            advance();

            if (peek().type == CREATE || peek().type == DROP) {
                advance();
            }

            return Help(3, "", commandName);
        }
    }
}

vector<Token> Parser::parseConditions(){
    vector<Token> conditions;
    if(peek().type==WHERE){
        consume(WHERE, "");
        while(peek().type!=SEMICOLON){
            vector<TokenType> comparisonOps = {LESS_EQUAL, LESS, EQUAL, GREATER, GREATER_EQUAL, BANG_EQUAL};

            if(peek().type==IDENTIFIER){
                conditions.push_back(peek());
                advance();
            }
            else consume(IDENTIFIER, "Syntax error");

            if(find(comparisonOps.begin(), comparisonOps.end(), peek().type)!=comparisonOps.end()){
                conditions.push_back(peek());
                advance();
            }
            else {
                cerr << "Syntax error";
                exit(1);
            }

            if(peek().type==IDENTIFIER || peek().type==STRING){
                conditions.push_back(peek());
                advance();
            }
            else consume(IDENTIFIER, "Syntax error");

            if(peek().type==AND || peek().type==OR){
                conditions.push_back(peek());
                advance();
            }
            else break;
        }
    }
    return conditions;
}

void Parser::consume(TokenType expected, string error) {
    if (isAtEnd() || tokens[current].type != expected) cerr << error<<endl;
    current++;
}

Token Parser::peek() {
    if (isAtEnd()) return Token(NULL_TOKEN, "", "");
    return tokens[current];
}

Token Parser::advance() {
    if (isAtEnd()) return Token(NULL_TOKEN, "", "");
    current++;
    return tokens[current];
}

bool Parser::isAtEnd() {
    return current >= tokens.size();
}