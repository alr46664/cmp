
#include "semantical.h"

using namespace std;

// PRIVATE

// find the first context from bottom up the tree
Context* Semantical::findContext(Node* node){
    Context* c = NULL;
    while( node != NULL && (c = node->getContext()) == NULL){
        node = node->getParent();
    }
    return c;
}

// check if a context is already defined
Context* Semantical::findContext(string name, Node* node){
    Context* c = NULL;
    while( node != NULL && ((c = node->getContext()) == NULL || c->getName() != name) ){
        node = node->getParent();
    }
    if (c != NULL && c->getName() == name)
        return c;
    return NULL;
}

// verifies if the variable / function n is defined in some context
Defined* Semantical::find(string name, Node* node){
    Context* c = NULL;
    map<string, Defined>::iterator it;
    while( node != NULL && ((c = node->getContext()) == NULL || (it = c->find(name)) == c->end()) ){
        node = node->getParent();
    }
    if (c != NULL && it != c->end())
        return &(it->second);
    return NULL;
}

// PUBLIC

// add a new context
void Semantical::addContext(string name, Node* n){
    // avoid duplicate ifs and while blocks
    if (name == AST_IF || name == AST_WHILE || name == AST_BLOCK ){
        name += "_" + to_string(context_size);
    }
    // check if context is already defined
    if (findContext(name, n) != NULL)
        throw Error(string("The function (context) already exists (duplicate names not allowed)!"), n->getType(), n->getLine(), ERR_SEM);
    // create new context and update control variable
    n->createContext(name);
    context_size++;
}

// get the name of an already defined variable/function, if not found
// return "" (empty string)
Defined& Semantical::get(Node* n){
    Defined* d = find(n->getType(), n);
    if (d == NULL)
        throw Error(string("Could not find variable / function (not declared)!"), n->getType(), n->getLine(), ERR_SEM);
    return *d;
}

// set a new context varible/function (defined by t)
void Semantical::set(Node* n, string t, int s){
    Context* c = findContext(n);
    if ( c == NULL ){
        throw Error(string("Could not find a context to add the variable!"), n->getType(), n->getLine(), ERR_SEM);
    }
    if ( c->find(n->getType()) != c->end() ){
        // if we reached this point, our function failed becuse the
        // get didnt throw the error (which means we've found the function/variable
        // in other contexts already defined)
        throw Error(string("Could not set a new variable / function (duplicated names not allowed)!"), n->getType(), n->getLine(), ERR_SEM);
    }
    c->add(n->getType(), t, s);
}
