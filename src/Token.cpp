#include "Token.h"

Token::Token(TokenType type, string lexeme, string literal) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
}

string Token::toString() {
    return getTokenType() + ": " + lexeme;
}

string Token::getTokenType(){
    vector<TokenType> keywords = {AND, OR, SELECT, CREATE, TABLE, FROM, WHERE, UPDATE, INSERT, IN, HAVING, AS, QUIT};
    if(find(keywords.begin(), keywords.end(), type)!=keywords.end()) return "KEYWORD";
    else if(type==IDENTIFIER) return "IDENTIFIER";
    else if(type==STRING) return "STRING LITERAL";
    return "OTHER";
}