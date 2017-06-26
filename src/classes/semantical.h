#ifndef SEMANTICAL_H
#define SEMANTICAL_H

// C++ INCLUDES
#include <iostream> // cin, getline
#include <map> // map
#include <string> // string
#include <stack> // stack

// custom includes
#include "node.h" // Node class
#include "error.h" // Error class

// defines of the AST types
#include "ast_types.h"

class Semantical {
private:
    // root of the AST
    Node* program;
public:
    Semantical(Node* p): program(p) {};

    void run();
};

#endif // SEMANTICAL_H
