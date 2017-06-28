
#include "semantical.h"

using namespace std;

void Semantical::run(){
    cout << "Semantical - BEGIN\n\n";
    while(!operate.empty()) {
        top = operate.front();
        // remove top from stack
        operate.pop_front();
        // DEBUGGING STACK
        cout << "PROCESSING: \n\t" << *top << "\n\n";
        // check the children nodes
        //      if (top->getType() == AST_PROGRAM)    check_program();
        // else if (top->getType() == AST_DECFUNC)    check_decfunc();
        // else if (top->getType() == AST_DECVAR)     check_decvar();
        // else if (top->getType() == AST_ASSIGN)     check_assign();
        // else if (top->getType() == AST_FUNCCALL)   check_funccall();
        // else if (top->getType() == AST_ARGLIST)    check_arglist();
        // else if (top->getType() == AST_PARAMLIST)  check_paramlist();
        // else if (top->getType() == AST_BLOCK)      check_block();
        // else if (top->getType() == AST_RETURN)     check_return();
        // else if (top->getType() == AST_IF)         check_if();
        // else if (top->getType() == AST_WHILE)      check_while();
        // else if (top->getType() == AST_BREAK)      check_break();
        // else if (top->getType() == AST_CONTINUE)   check_continue();
        // else if ( Utility::isBinaryOp(top->getType()) ||
        //     Utility::isUnaryOp(top->getType()) )   check_expr();
        // else if (top->getToken() == T_ID)          check_id();
        // else if (top->getToken() != T_DEC)
        //     throw Error(string("Node of type \"") + top->getToken()  + "\" not identified!", top->getType(), top->getLine(), ERR_SIN);
    }
    // free up memory
    clear();
    cout << "\nSemantical - END\n\n";
}

// reduce memory usage
void Semantical::clear(){
    operate.clear();
}
