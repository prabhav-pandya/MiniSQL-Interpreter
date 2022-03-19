#include <bits/stdc++.h>
#include "src/SQLInterpreter.h"

using namespace std;

/*
 * Check constraint while INSERT
 * Add constraint while CREATE TABLE
 * */

int main() {
    string query;
    for (;;) {
        SQLInterpreter interpreter = SQLInterpreter();
        cout << "> ";
        getline(cin, query);
        interpreter.run(query);
    }
    return 0;
}
