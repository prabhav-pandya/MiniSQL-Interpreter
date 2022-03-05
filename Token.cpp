#include "Token.h"

Token::Token(TokenType type, string lexeme, string literal) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
}

string Token::toString(TokenType type, string lexeme, string literal) {
    return type+" "+lexeme+" "+literal;
}