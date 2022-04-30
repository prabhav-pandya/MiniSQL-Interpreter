#include <bits/stdc++.h>
#include "src/SQLInterpreter.h"

using namespace std;

/*
 * Add support for multiple tables in SELECT command
 * Add syntax error for invalid WHERE commands
 *
 * */


int main() {
    string query;
    for (;;) {
        SQLInterpreter interpreter = SQLInterpreter();
        getline(cin, query);
        interpreter.run(query);
    }
    return 0;
}
