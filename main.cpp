#include <bits/stdc++.h>
#include "src/SQLInterpreter.h"

using namespace std;

int main() {
    string query;
    for(;;){
        SQLInterpreter interpreter = SQLInterpreter();
        cout<<"> ";
        getline(cin, query);
        interpreter.run(query);
    }
    return 0;
}
