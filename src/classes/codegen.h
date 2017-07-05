
#ifndef CODEGEN_H
#define CODEGEN_H


// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string
#include <map> // map
#include <list> // list

// custom includes
#include "node.h" // Node class
#include "error.h" // Error class
#include "utility.h" // utility class

// defines of the AST types
#include "ast_types.h"

// THERE MUST BE ONLY ONE Codegen IN THE WHOLE PROGRAM
class Codegen {
private:
    // MAIN PROGRAM DATA

    // stack of nodes
    std::list<Node*> operate;

    // list of string to print
    std::list<std::string> to_print;
    // position to insert new string into the stack
    std::list<std::string>::iterator ins;
    // save last function name here
    std::string func_name;

    // PRIVATE FUNCTIONS
    void commit_print_stack();
    std::list<std::string>::iterator insert_print_stack(std::string s);

    std::string function_call(std::string f);
    std::string function_return();

    std::string move_stack_back();

    std::string save_reg(std::string reg);
    std::string load_reg(std::string reg, std::string val);
    std::string load_reg(std::string reg, int move_back = 1);
    std::string copy_reg(std::string dest, std::string src);

    void generate(Node *n);
public:

    //   MEMBER
    Codegen(Node* p){
        operate.push_back( p );
        ins = to_print.end();
    };

    void run();
};

#endif // CODEGEN_H
