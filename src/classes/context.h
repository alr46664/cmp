#ifndef CONTEXT_H
#define CONTEXT_H

// C++ INCLUDES
#include <iostream> // cin, getline, endl
#include <string> // string
#include <list> // list
#include <map> // map
#include <initializer_list> // pass {} arrays to functions

// defines of the AST types
#include "ast_types.h"

// custom includes
#include "error.h" // Error class
#include "defined.h" // Defined class
#include "utility.h" // utility class

class Context {
private:
    // store the scope of the context
    std::string name;
    // store a list of defined variables / functions
    std::map<std::string, Defined> context;
public:

    Context (std::string name): name(name) {}

    std::string getName();

    void add(std::string n, std::string t, int s = 0);
    std::map<std::string, Defined>::iterator find(std::string n);

    std::map<std::string, Defined>::iterator begin();
    std::map<std::string, Defined>::iterator end();

    //    NON-MEMBER
    // (friend allows access to the
    // private / protected variables)
    friend std::ostream& operator<<(std::ostream &output, const Context &C){
        for(std::map<std::string, Defined>::const_iterator it = C.context.begin(); it != C.context.end(); it++){
            output << C.name << "\t\t" << it->second.getName() << "\t\t" << it->second.getType() << std::endl;
        }
        return output;
    }
};

#endif // CONTEXT_H
