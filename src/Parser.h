#ifndef MINIRDBMS_PARSER_H
#define MINIRDBMS_PARSER_H

#include <bits/stdc++.h>
#include "Token.h"
#include "Stmt.cpp"

/*
SQL Grammar (We need this for the parser)
program -> command* EOF;
command -> (createTable | dropTable | select | insert | update | help) ";";
createTable -> "CREATE TABLE" table "(" attrDecl ")";
dropTable   -> "DROP TABLE" table;
select -> "SELECT" attrList "FROM" tableList ("WHERE" condList)?;
insert -> "INSERT INTO" table "VALUES" "(" attrList ")"
delete -> "DELETE FROM" table ("WHERE" condList)?;
update -> "UPDATE" table "SET" (attr = LITERAL)+ ("WHERE" condList)?;
help -> "HELP"
attrDecl -> (dataType attr)+;
attrList -> (attr)+;
condList -> (attr ("=" | ">" | ">=" | "<" | "<=" | "!=") LITERAL)+;
dataType -> "INT" | "FLOAT" | "CHAR";
attr  -> IDENTIFIER;
table -> IDENTIFIER
*/

class Parser {
 vector<Token> tokens;
 int current = 0;
public:
    Parser(vector<Token> tokensList);
    Stmt parse();
    Select parseSelect();
    CreateTable parseCreateTable();
    Token peek();
    Token advance();
    bool isAtEnd();
    void consume(TokenType expected, string error);
};


#endif //MINIRDBMS_PARSER_H
