#ifndef MEM_MemContext_H
#define MEM_MemContext_H


// C++ INCLUDES
#include <iostream> // cin, getline, endl
#include <string> // string
#include <list> // list
#include <map> // map
#include <initializer_list> // pass {} arrays to functions

// defines of the AST types
#include "ast_types.h"

// custom includes
#include "node.h" // node class
#include "error.h" // Error class
#include "utility.h" // utility class

class MemContext {
private:
    std::list<std::string> names;
    std::list<int> stack_pos;
    int last_mem_pos;
public:

    MemContext () {
        last_mem_pos = 0;
    }

    void add(Node *n);
    void remove(Node *n);
    int get(Node *n);

    //    NON-MEMBER
    // (friend allows access to the
    // private / protected variables)
    friend std::ostream& operator<<(std::ostream &output, const MemContext &MC){
        std::list<std::string>::const_iterator it_s = MC.names.begin();
        for(std::list<int>::const_iterator it = MC.stack_pos.begin(); it != MC.stack_pos.end(); it++){
            output << *(it_s++) << " => " << *it << "\n";
        }
        output << "\n";
        return output;
    }

};

#endif // MEM_MemContext_H
