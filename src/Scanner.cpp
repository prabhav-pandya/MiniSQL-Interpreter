#include "Scanner.h"

vector<Token> Scanner::scanTokens() {
    while(!isAtEnd()){
        start = current;
        scanToken();
    }
    return tokens;
}

Scanner::Scanner(string query) {
    // keywords hashmap
    keywords["create"] = CREATE;
    keywords["drop"] = DROP;
    keywords["table"] = TABLE;
    keywords["from"] = FROM;
    keywords["and"] = AND;
    keywords["or"] = OR;
    keywords["select"] = SELECT;
    keywords["where"] = WHERE;
    keywords["update"] = UPDATE;
    keywords["insert"] = INSERT;
    keywords["in"] = IN;
    keywords["having"] = HAVING;
    keywords["as"] = AS;
    keywords["quit"] = QUIT;
    keywords["into"] = INTO;
    keywords["values"] = VALUES;
    keywords["delete"] = DELETE;

    // data types
    keywords["int"] = TYPE;
    keywords["str"] = TYPE;
    keywords["char"] = TYPE;

    // help
    keywords["help"] = HELP;
    keywords["tables"] = TABLES;
    keywords["describe"] = DESCRIBE;

    this->query = query;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
        case '+': addToken(PLUS); break;
        case '/': addToken(SLASH); break;
        case '-': addToken(MINUS); break;
        case ',': addToken(COMMA); break;
        case '>':
            addToken(match('=')?GREATER_EQUAL:GREATER);
            break;
        case '<':
            addToken(match('=')?LESS_EQUAL:LESS);
            break;
        case '=':
            addToken(match('=')?EQUAL_EQUAL:EQUAL);
            break;
        case '!':
            addToken(match('=')?BANG_EQUAL:BANG);
            break;
        case ' ':
        case '\t':
        case '\r':
            break;
        case '"': value(); break;
        default:
            identifier();
            break;

    }
}

string toLower(string str){
    string res = "";
    for(char c: str){
        if(c==' ') continue;
        res += tolower(c);
    }
    return res;
}

void Scanner::identifier() {
    while((isalnum(peek()) || peek()=='_') && peek()!=';') advance();
    string text = query.substr(start, current-start);
    TokenType type = keywords.find(toLower(text))!=keywords.end()? keywords[toLower(text)]: IDENTIFIER;
    //if(type==IDENTIFIER) cout<<text<<endl;
    addToken(type, text);
}

void Scanner::value() {
    while(peek()!='"' && !isAtEnd()){
        advance();
    }

    advance();
    string value = query.substr(start+1, current-start-2);
    addToken(STRING, value);
}

bool Scanner::isAtEnd() {
    if(current>=query.size()) return true;
    return false;
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

void Scanner::addToken(TokenType type, string literal) {
    string text = query.substr(start, current-start);
    tokens.push_back(Token(type, text, literal));
}

char Scanner::advance() {
    return query[current++];
}

char Scanner::peek() {
    if(isAtEnd()) return '\0';
    return query[current];
}

bool Scanner::match(char expected) {
    if(isAtEnd()) return false;
    if(query[current]!= expected) return false;
    current++;
    return true;
}
