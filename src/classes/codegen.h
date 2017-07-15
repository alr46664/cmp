
#ifndef CODEGEN_H
#define CODEGEN_H


// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string
#include <map> // map
#include <list> // list
#include <stack> // stack

// custom includes
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

    // create a print stack
    // PrintStack ps;

    // save last function name here
    std::string func_name;

    // PRIVATE FUNCTIONS

    std::string get_dest_reg(Node *n, std::string *cmp_reg = NULL);

    std::string add(std::string dest, std::string src1, std::string src2);
    std::string sub(std::string dest, std::string src1, std::string src2);
    std::string mul(std::string dest, std::string src1, std::string src2);
    std::string div(std::string dest, std::string src1, std::string src2);

    std::string lt(std::string dest, std::string src1, std::string src2);
    std::string lte(std::string dest, std::string src1, std::string src2);
    std::string gt(std::string dest, std::string src1, std::string src2);
    std::string gte(std::string dest, std::string src1, std::string src2);
    std::string eq(std::string dest, std::string src1, std::string src2);
    std::string neq(std::string dest, std::string src1, std::string src2);

    std::string jump(std::string reg);
    std::string function_save();
    std::string function_restore();
    std::string function_call(std::string f);
    std::string function_return();

    std::string move_stack_back();

    std::string save_reg(std::string reg);
    std::string load_reg(std::string reg, std::string val);
    std::string load_reg(std::string reg);

    std::string load_reg_from_fp(std::string reg, std::string pos);

    std::string save_reg_to_t0(std::string reg, std::string pos);
    std::string load_reg_from_t0(std::string reg, std::string pos);

    std::string copy_reg(std::string dest, std::string src);

    std::string generate(Node *n);
public:

    //   MEMBER
    Codegen(Node* p){
        program = p;
    };

    void run();
};

#endif // CODEGEN_H
