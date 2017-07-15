
#ifndef CODEGEN_H
#define CODEGEN_H


// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string
#include <map> // map
#include <list> // list
#include <stack> // stack

// custom includes
#include "assembly.h" // Assembly class
#include "mem_context.h" // MemContext class
#include "node.h" // Node class
#include "error.h" // Error class
#include "utility.h" // utility class

// defines of the AST types
#include "ast_types.h"

#define CG_FNAME_PROGRAM "program"

// THERE MUST BE ONLY ONE Codegen IN THE WHOLE PROGRAM
class Codegen {
private:
    // MAIN PROGRAM DATA
    Node* program;

    // save the memory / symbol map
    std::map<std::string, MemContext> sym_map;

    // save last function name here
    std::string func_name;
    // save counts
    int if_count, while_count;

    // PRIVATE FUNCTIONS
    std::string get_dest_reg(Node *n, std::string *cmp_reg = NULL);

    std::string generate(Node *n);
public:

    //   MEMBER
    Codegen(Node* p){
        if_count = while_count = 0;
        program = p;
    };

    void run();
};

#endif // CODEGEN_H
