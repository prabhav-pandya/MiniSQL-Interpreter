#include "Parser.h"

Parser::Parser(vector<Token> tokensList) {
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

            if(peek().type != RIGHT_PAREN) consume(COMMA, "Expected a Comma!");
        } else {
            cerr << "Invalid Column Name!";
        }
    } while (peek().type != RIGHT_PAREN);


    consume(RIGHT_PAREN, "Expected a ')'!");
//    consume(SEMICOLON, "Expected a ';'!");

    return CreateTable(tableName, columnNames, columnTypes);
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

    if(peek().type==IDENTIFIER){
        tableName = peek().lexeme;
        advance();
    }
    else cerr<< "Expected table name";

    // consume VALUES
    consume(VALUES, "Expected Token Values");
    consume(LEFT_PAREN, "Expected Parenthesis");

    // consume values
    vector<string> values;
    while(peek().type!=RIGHT_PAREN){
        if(peek().type==IDENTIFIER || peek().type==STRING){
            values.push_back(peek().type==IDENTIFIER? peek().lexeme:peek().literal);
            advance();
        }
        else{
            cerr << "Expected values!";
            exit(1);
        }

        if(peek().type==COMMA){
            consume(COMMA, "");
        }
        else break;
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
    vector<Token> condition;

    // check for semicolon
    if (peek().type == SEMICOLON) consume(SEMICOLON, "Expected Token Semicolon!");

    else {
        // if there is a WHERE, consume and store the condition
        if (!isAtEnd()) {
            consume(WHERE, "Syntax Error!");
            while (peek().type != SEMICOLON) {
                if (isAtEnd()) cerr << "Syntax error!";
//                condition.push_back(peek());
//                advance();
                if (peek().type == IDENTIFIER) {
                    condition.push_back(peek());
                    advance();
                } else cerr << "Syntax error!";

                if (peek().type != SEMICOLON) {
                    vector<TokenType> operators = {BANG, BANG_EQUAL,
                                                   EQUAL, GREATER, GREATER_EQUAL,
                                                   LESS, LESS_EQUAL};
                    if (find(operators.begin(), operators.end(), peek().type) == operators.end()) {
                        cerr << "Syntax error!";
                    }
                    condition.push_back(peek());
                    advance();

                    if (peek().type == STRING || peek().type == IDENTIFIER) {
                        condition.push_back(peek());
                        advance();
                    } else cerr << "Syntax error!";
                }

                if (peek().type == AND || peek().type == OR) {
                    condition.push_back(peek());
                    advance();
                }
            }
        } else cerr << "Syntax error!";
    }
    return Select(tableName, columns, condition);
}

void Parser::consume(TokenType expected, string error) {
    if (isAtEnd() || tokens[current].type != expected) cerr << error;
    advance();
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