#ifndef PRINT_STACK_H
#define PRINT_STACK_H

#include <iostream>
#include <string>
#include <list>
#include <map>

#include "error.h" // Error class
#include "utility.h" // utility class

// defines of the AST types
#include "ast_types.h"

class PrintStack {
private:
    // list of string to print
    std::list<std::string> to_print;
    // list of positions to insert data into
    std::list<std::list<std::string>::iterator> to_insert;

    // iterator to the main: function of the print stack
    std::list<std::string>::iterator main_pr;
    // iterator to the place where global register $t0 stores the mem_pos
    // of the global vars
    std::list<std::string>::iterator global_pr;
public:
    //   MEMBER
    PrintStack(){
        // clear to insert, and start it at the end of the print stack
        clear_to_insert();
        // set main and glboal var iterators to the end of the print stack
        // (this is temporary)
        setMain(      to_print.end() );
        setGlobalVar( to_print.end() );
    }

    void setMain(std::list<std::string>::iterator it);
    std::list<std::string>::iterator& getMain();

    void setGlobalVar(std::list<std::string>::iterator it);
    std::list<std::string>::iterator& getGlobalVar();

    std::list<std::string>::iterator push_to_print(std::string s, int dir = 1);
    void push_to_insert(std::list<std::string>::iterator it);
    void pop_to_insert();
    void clear_to_insert();

    //    NON-MEMBER
    // (friend allows access to the
    // private / protected variables)
    friend std::ostream& operator<<(std::ostream &output, const PrintStack &N){
        for (std::list<std::string>::const_iterator it = N.to_print.begin(); it != N.to_print.end() ; ++it){
            output << *it;
        }
        // N.to_print.clear();
        // N.clear_to_insert();
        return output;
    }
};

#endif // PRINT_STACK_H
