
#include "mem_context.h"

using namespace std;

// add a new variable/function to this context
void MemContext::add(Node *n){
    mem_map[n->getType()] = last_mem_pos;
    last_mem_pos += 4;
}

int MemContext::get(Node *n){
    map<string, int>::iterator it = mem_map.find(n->getType());
    if (it != mem_map.end())
        return it->second;
    throw Error(string("Could not retrieve memory map for variable \"") + n->getType() + "\" (variable is not defined)", "Variable Not Defined", n->getLine(), ERR_UNK);
}

