#include "ast_types.h"
#include "error.h"
#include "operate.h"

using namespace std;

Node* Operate::getProgramAST(){
    return program;
}

bool Operate::empty(){
    return to_operate.empty();
}

Node* Operate::top(){
    return to_operate.top();
}

void Operate::clear(){
    while (pop() != NULL) {}
}

Node* Operate::add(string token, string val, int line){
    // check if the parent node has the conditions specified by conditions
    Node *n = to_operate.top();
    // conditions satisfied, continue with the insertion
    to_operate.push(n->add(token, val, line));
    return to_operate.top();
}

void Operate::push(Node *n){
    to_operate.push(n);
}

Node* Operate::pop(){
    Node *n = NULL;
    if (!to_operate.empty()){
        n = to_operate.top();
        to_operate.pop();
    }
    return n;
}

// move o parent como primeiro filho do no especificado
Node* Operate::add_swap(Node *new_parent){
    // remova old_parent da arvore
    Node *old_parent = this->pop();
    list<Node*>::iterator it = this->top()->remove(old_parent);
    // adicione new_parent como pai de old_parent
    this->top()->insert(it, new_parent);
    new_parent->add(old_parent);
    // adicione new_parent ao stack
    to_operate.push(new_parent);
    return to_operate.top();
}
