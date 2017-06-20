#ifndef PARSER_H
#define PARSER_H

// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string
#include <stack> // stack
#include <set> // set

// custom includes
#include "node.h" // Node class
#include "error.h" // Error class
#include "operate.h" // Operate class

// defines of the AST types
#include "ast_types.h"

#define P_INIT 0
#define P_PARENLIST 1

// THERE MUST BE ONLY ONE PARSER IN THE WHOLE PROGRAM
class Parser {
private:
    // MAIN PROGRAM DATA
    // AST
    Node *program;
    // class that controls the operate stack
    Operate operate;
    // this map will store the context (scope) of the identifiers
    // (functions and variables) that the user has declared
    std::set<std::string> ids;
    // armazene o estado anterior do parser
    int state;

    // PRIVATE DATA
    std::string token, val;
    int line;

    // PRIVATE FUNCTIONS
    bool isExpr();

    void parse_id();
    void parse_key();
    void parse_dec();
    void parse_sym();
public:

    //   MEMBER
    Parser(){
        program = operate.getProgramAST();
        state = P_INIT;
    };
    ~Parser(){
        program = NULL;
    }

    void parse(std::string t, std::string v, int l);
    Node* getProgramAST();
};

#endif // PARSER_H
