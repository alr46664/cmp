// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string

// custom includes
#include "syn.h" // defines
#include "classes/node.h" // Node class
#include "classes/parser.h" // Parser class


using namespace std;


// create the single parser that the whole program will use
Parser p;

// void parse_token(string &token, string &val, int line){
//             // TODO: TO REMOVE (debug)
//     cout << token << "- -" << val << "- -" << line << endl;
//     // return;
//     if (token == "ID")
//         // TODO: TO REMOVE (debug)
//         cout << "ID found" << endl;
//         // crie variavel erro para caso um error ocorra
//         Error e(string("ID \"") + val + "\" can only be used in declarations of variables and functions, as well as in assigns, expressions and function calls.", line, ERR_ID);

//         if (!operate.empty()){
//             operate.add(val, {"decfunc", "decvar"}, e);
//         } else {
//             operate.add(val);
//         }
//     else if (token == "KEY") {
//          // crie variavel erro para caso um error ocorra
//         Error e(string("KEY \"") + val + "\" was not recognized. Valid values are var,let,def,if,else,while,return,break,continue.", line, ERR_KEY);
//         if (val == "def"){
//             // TODO: TO REMOVE (debug)
//             cout << "KEY def found" << endl;
//             operate.add("decfunc");
//         } else if (val == "var" || val == "let"){
//             // TODO
//         } else if (val == "if"){
//             // TODO
//         } else if (val == "else"){
//             // TODO
//         } else if (val == "while"){
//             // TODO
//         } else if (val == "return"){
//             // TODO
//         } else if (val == "break"){
//             // TODO
//         } else if (val == "continue"){
//             // TODO
//         } else {
//             e.print();
//         }
//     } else if (token == "DEC") {

//     } else if (token == "SYM") {

//     } else{
//         Error e(string("Token \"") + token + "\" not recognized", line, ERR_TOKEN_UNDEF);
//         e.print();
//     }
// }

int main(int argc, char **argv) {
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
    cout << p.getProgramAST() << endl;
    return 0;
}
