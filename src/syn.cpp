// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string

// custom includes
#include "syn.h" // defines
#include "classes/node.h" // Node class
#include "classes/parser.h" // Parser class


using namespace std;

int main(int argc, char **argv) {
    // create the single parser that the whole program will use
    Parser p;
    // store the whole stdin / cin line
    int line;
    string token, val;
    string input;
    // read until EOF
    while(getline(cin, input)){
        size_t space1 = input.find_first_of(' ');
        size_t space2 = input.find_last_of(' ');
        if (space1 != string::npos && space2 != string::npos){
            token = input.substr(0, space1);
            val = input.substr(space1+2, space2-space1-3);
            line = atoi(input.substr(space2, string::npos).c_str());
        }
        // process token and val
        p.parse(token, val, line);
    }
    // print the program AST tree
    cout << *p.getProgramAST() << endl;
    return 0;
}
