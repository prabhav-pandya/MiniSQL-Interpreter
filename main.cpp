#include <bits/stdc++.h>
#include "SQLInterpreter.h"

using namespace std;

int main() {
    string query;
    SQLInterpreter interpreter = SQLInterpreter();
    for(;;){
        cout<<"> ";
        getline(cin, query);
        //cin>>query;
        interpreter.run(query);
    }
    return 0;
}
