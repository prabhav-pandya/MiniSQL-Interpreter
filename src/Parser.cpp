#include "Parser.h"

Parser::Parser(vector<Token> tokensList) {
    for(auto token: tokensList){
        tokens.push_back(token);
    }
}

Stmt Parser::parse() {
    if(peek().type==SELECT){

    }
}

Select Parser::parseSelect(){
    // consume SELECT
    consume(SELECT,"");

    // consume and store column names or *
    bool toConsume = true;
    vector<string> columns;
    string tableName;

    if(peek().type==STAR){
        columns.push_back("*");
        advance();
    }
    else{
        while(toConsume){
            columns.push_back(peek().lexeme);
            advance();
            toConsume=false;
            if(peek().type==COMMA){
                consume(COMMA,"");
                toConsume = true;
            }
        }
    }

    // consume keyword FROM
    consume(FROM, "Syntax error");

    // consume table name
    if(peek().type==IDENTIFIER) tableName = peek().lexeme;
    else cerr << "Table name not specified";
    advance();
    vector<Token> condition;

    // check for semicolon
    if(peek().type==SEMICOLON) consume(SEMICOLON,"");

    else{
        // if there is a WHERE, consume and store the condition
        if(!isAtEnd()){
            consume(WHERE, "Syntax Error!");
            while(peek().type!=SEMICOLON){
                if(isAtEnd()) cerr << "Syntax error!";
//                condition.push_back(peek());
//                advance();
                  if(peek().type==IDENTIFIER){
                      condition.push_back(peek());
                      advance();
                  }
                  else cerr << "Syntax error!";

                  if(peek().type!=SEMICOLON){
                      vector<TokenType> operators = {BANG, BANG_EQUAL,
                                                     EQUAL,GREATER, GREATER_EQUAL,
                                                     LESS, LESS_EQUAL};
                      if(find(operators.begin(), operators.end(), peek().type)==operators.end()) {
                          cerr << "Syntax error!";
                      }
                      condition.push_back(peek());
                      advance();

                      if(peek().type==STRING || peek().type==IDENTIFIER){
                          condition.push_back(peek());
                          advance();
                      }
                      else cerr << "Syntax error!";
                  }

                  if(peek().type==AND || peek().type==OR){
                      condition.push_back(peek());
                      advance();
                  }
            }
        } else cerr << "Syntax error!";
    }
    return Select(tableName, columns, condition);
}

void Parser::consume(TokenType expected, string error) {
    if(isAtEnd() || tokens[current].type!=expected) cerr << error;
    advance();
}

Token Parser::peek() {
    if(isAtEnd()) return Token(NULL_TOKEN, "","");
    return tokens[current];
}

Token Parser::advance() {
    if(isAtEnd()) return Token(NULL_TOKEN, "","");
    current++;
    return tokens[current];
}

bool Parser::isAtEnd() {
    return current>=tokens.size();
}