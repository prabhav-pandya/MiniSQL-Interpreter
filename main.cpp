#include <bits/stdc++.h>
#include "src/SQLInterpreter.h"

using namespace std;

int main()
{
    string query;
    for (;;)
    {
        try
        {
            SQLInterpreter interpreter = SQLInterpreter();
            getline(cin, query);
            interpreter.run(query);
        }
        catch (...)
        {
            cout << endl
                 << "Error caused";
            char descision = 'y';
            cout << endl
                 << "Do you want to continue? ";
            cin >> descision;
            if (descision == 'y' || descision == 'Y')
            {
            }
            else
            {
                break;
            }
        }
    }
    return 0;
}
