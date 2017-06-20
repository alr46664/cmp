#include "ast_types.h"
#include "error.h"
#include "operate.h"
#include "utility.h"

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

Node* Operate::add(string token, string val){
    Error e(string("Could not add \"") + val + "\" to the AST tree", 0, ERR_UNK);
    return this->add(token, val, {}, {}, e);
}

Node* Operate::add(string token, string val, initializer_list<char*> condToken, initializer_list<char*> condType, Error& e){
    // check if the parent node has the conditions specified by conditions
    Node *n = to_operate.top();
    if (!(Utility::check(n->getType(), condType) && Utility::check(n->getToken(), condToken))){
        e.print();
    }
    // conditions satisfied, continue with the insertion
    to_operate.push(n->add(token, val));
    return to_operate.top();
}

void Operate::push(Node *n){
    to_operate.push(n);
}

Node* Operate::pop(){
    Node *n = to_operate.top();
    to_operate.pop();
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
