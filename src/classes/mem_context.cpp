
#include "mem_context.h"

using namespace std;

// add a new variable/function to this context
void MemContext::add(Node *n){
    names.push_back(n->getType());
    stack_pos.push_back(last_mem_pos);
    last_mem_pos += 4;
}

void MemContext::remove(Node *n){
    list<string>::reverse_iterator it = names.rbegin();
    list<int>::reverse_iterator it_pos = stack_pos.rbegin();
    for (; it != names.rend(); it++){
        if (*it == n->getType())
            break;
        it_pos++;
    }
    if (it != names.rend()){
        names.erase( --(it.base()) );
        stack_pos.erase( --(it_pos.base()) );
        return;
    }
    throw Error(string("Could not remove variable \"") + n->getType() + "\" from memory map (variable is not defined)", "Variable Not Defined", n->getLine(), ERR_UNK);
}

int MemContext::get(Node *n){
    list<string>::reverse_iterator it = names.rbegin();
    list<int>::reverse_iterator it_pos = stack_pos.rbegin();
    for (; it != names.rend(); it++){
        if (*it == n->getType())
            break;
        it_pos++;
    }
    if (it != names.rend())
        return *it_pos;
    throw Error(string("Could not retrieve memory map for variable \"") + n->getType() + "\" (variable is not defined)", "Variable Not Defined", n->getLine(), ERR_UNK);
}

