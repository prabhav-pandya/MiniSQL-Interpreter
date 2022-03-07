//
// Created by prabhav on 05/03/22.
//

#include "SQLInterpreter.h"
#include "Scanner.h"

SQLInterpreter::SQLInterpreter() {

}

void SQLInterpreter::run(string query){
    Scanner scanner(query);
    vector<Token> tokens = scanner.scanTokens();
    for(auto token: tokens){
        cout<<token.toString()<<endl;
    }
};