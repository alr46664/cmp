#ifndef SEMANTICAL_H
#define SEMANTICAL_H

// C++ INCLUDES
#include <iostream> // cin, getline, endl
#include <string> // string
#include <map> // map
#include <list> // list
#include <initializer_list> // pass {} arrays to functions

// defines of the AST types
#include "ast_types.h"

// custom includes
#include "node.h" // Node class
#include "error.h" // Error class
#include "defined.h" // Defined class
#include "context.h" // Context class
#include "utility.h" // utility class

class Semantical {
private:
    // size of contexts list
    int context_size;

    // private functions
    Context* findContext(Node* node);
    Context* findContext(std::string name, Node* node);
    Defined* find(std::string name, Node* node);

public:

    // we need to pass the program node (global context)
    Semantical() {
        context_size = 0;
    }

    void addContext(std::string name, Node* n);

    Defined& get(Node* n);
    void set(Node* n, std::string t, int s = 0);

};

#endif // SEMANTICAL_H
