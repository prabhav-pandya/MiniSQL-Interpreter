#include <bits/stdc++.h>
#include "SQLInterpreter.h"

using namespace std;

int main() {
    string query;
    for(;;){
        SQLInterpreter interpreter = SQLInterpreter();
        cout<<"> ";
        getline(cin, query);
        //cin>>query;
        interpreter.run(query);
    }
    return 0;
}
