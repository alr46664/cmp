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
#include "utility.h" // utility class

// defines of the AST types
#include "ast_types.h"

// THERE MUST BE ONLY ONE PARSER IN THE WHOLE PROGRAM
class Parser {
private:
    // MAIN PROGRAM DATA
    // ROOT OF THE AST
    Node *program;
    // class that controls the operate stack
    Operate operate;
    // MAPA DE PRECEDENCIA
        // operadores binarios
    std::map<std::string,int> precedence;
        // operadores unarios
    std::map<std::string,int> precedence_un;

    // PRIVATE DATA
    std::string token, val;
    int line;

    // PRIVATE FUNCTIONS

    void parse_unary_op();
    void parse_binary_op();

    void parse_id();
    void parse_key();
    void parse_dec();
    void parse_sym();

    void clear();
public:

    //   MEMBER
    Parser(){
        program = operate.getProgramAST();
        // declaracao das precedencias aqui
            // op binarios
        precedence["||"] = 0;
        precedence["&&"] = 1;
        precedence["=="] = precedence["!="] = 2;
        precedence["<"] = precedence["<="] = precedence[">="] = precedence[">"] = 3;
        precedence["-"] = precedence["+"] = 4;
        precedence["*"] = precedence["/"] = 5;
            // op unarios
        precedence_un["!"] = 6;
        precedence_un["u-"] = 7;
    };

    void parse(std::string t, std::string v, int l);
    void run();
    Node* getProgramAST();
};

#endif // PARSER_H
