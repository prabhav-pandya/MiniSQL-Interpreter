//
// Created by prabhav on 05/03/22.
//

#include "SQLInterpreter.h"

void SQLInterpreter::run(string query){
    Scanner scanner(query);
    vector<Token> tokens = scanner.scanTokens();
};