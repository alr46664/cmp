#ifndef SEMANTICAL_H
#define SEMANTICAL_H

// C++ INCLUDES
#include <iostream> // cin, getline
#include <map> // map
#include <string> // string
#include <stack> // stack
#include <list> // list
#include <initializer_list> // pass {} arrays to functions

// defines of the AST types
#include "ast_types.h"

// custom includes
#include "node.h" // Node class
#include "error.h" // Error class
#include "utility.h" // utility class

class Semantical {
private:
    // list of nodes from the AST
    std::list<Node*> operate;
    // the top node of the list
    Node* top;

    // private functions

    void clear();
public:

    Semantical (Node* p) {
        operate.push_back(top = p);
    };

    void run();
};

#endif // SEMANTICAL_H
