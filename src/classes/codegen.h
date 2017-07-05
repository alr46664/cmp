
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
    // ROOT OF THE AST
    Node *program;
    // stack of nodes
    std::list<Node*> operate;

    // PRIVATE FUNCTIONS
    void generate(Node *n);
    void print_stack();
public:

    //   MEMBER
    Codegen(Node* p){
        operate.push_back( program = p );
    };

    void run();
};

#endif // CODEGEN_H
