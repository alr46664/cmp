
#ifndef CODEGEN_H
#define CODEGEN_H


// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string
#include <map> // map
#include <list> // list
#include <stack> // stack

// custom includes
#include "print_stack.h" // PrintStack class
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

    // stack of nodes
    std::list<Node*> operate;

    // stack of math operators and operands
    std::stack<Node*> math_op;
    std::stack<std::string> operands;

    // save the memory / symbol map
    std::map<std::string, MemContext> sym_map;

    // create a print stack
    PrintStack ps;

    // save last function name here
    std::string func_name;

    // PRIVATE FUNCTIONS
    void commit_print_stack();
    std::list<std::string>::iterator insert_print_stack(std::string s, int dir = 1);

    void push_to_insert(std::list<std::string>::iterator it);

    std::string get_dest_reg(Node *n, std::string *cmp_reg = NULL);

    std::string add(std::string dest, std::string src1, std::string src2);
    std::string sub(std::string dest, std::string src1, std::string src2);

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

    void process_op();
    // reserved words and structures of the lang
    void generate_program (Node *n);
    void generate_decfunc (Node *n);
    void generate_decvar (Node *n);
    void generate_assign (Node *n);
    void generate_funccall (Node *n);
    void generate_arglist (Node *n);
    void generate_paramlist (Node *n);
    void generate_block (Node *n);
    void generate_return (Node *n);
    void generate_if (Node *n);
    void generate_while (Node *n);
    void generate_break (Node *n);
    void generate_continue (Node *n);
    // tokens not idenfied by the above cases
    void generate_sym (Node *n);
    void generate_id (Node *n);
    void generate_dec (Node *n);
    // pseudo nodes used by codegen
    void generate_block_end (Node *n);
    void generate_end_statement(Node *n);
    void generate_end_arg(Node *n);

    void generate(Node *n);
public:

    //   MEMBER
    Codegen(Node* p){
        operate.push_back( p );
    };

    void run();
};

#endif // CODEGEN_H
