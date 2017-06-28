#ifndef SEMANTICAL_H
#define SEMANTICAL_H

// C++ INCLUDES
#include <iostream> // cin, getline
#include <map> // map
#include <string> // string
#include <stack> // stack
#include <list> // list
#include <initializer_list> // pass {} arrays to functions

// custom includes
#include "node.h" // Node class
#include "error.h" // Error class
#include "utility.h" // utility class

// defines of the AST types
#include "ast_types.h"

class Semantical {
private:
    // list of nodes from the AST
    std::list<Node*> operate;
    // the top node of the list
    Node* top;

    // private functions

    void clean_up();
    void verify_children(bool test_all, std::initializer_list<char*> token_test,
        std::initializer_list<char*> type_test, const std::string& msg);

    void check_program();
    void check_decfunc();
    void check_decvar();
    void check_assign();
    void check_funccall();
    void check_arglist();
    void check_paramlist();
    void check_block();
    void check_return();
    void check_if();
    void check_while();
    void check_break();
    void check_continue();

    void clear();
public:

    Semantical(Node* p) {
        operate.push_back(top = p);
    };

    void run();
};

#endif // SEMANTICAL_H
