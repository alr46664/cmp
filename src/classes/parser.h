#ifndef PARSER_H
#define PARSER_H

// C++ INCLUDES
#include <iostream> // cin, getline
#include <map> // map
#include <string> // string
#include <stack> // stack

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
    // ROOT OF THE AST
    Node *program;
    // class that controls the operate stack
    Operate operate;
    // MAPA DE PRECEDENCIA
    std::map<std::string,int> precedence;
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
        // declaracao das precedencias aqui
        precedence["||"] = 0;
        precedence["&&"] = 1;
        precedence["=="] = precedence["!="] = 2;
        precedence["<"] = precedence["<="] = precedence[">="] = precedence[">"] = 3;
        precedence["-"] = precedence["+"] = 4;
        precedence["*"] = precedence["/"] = 5;
        precedence["!"] = 6;
        precedence["u-"] = 7;
    };
    ~Parser(){
        program = NULL;
    }

    void parse(std::string t, std::string v, int l);
    Node* getProgramAST();
};

#endif // PARSER_H
