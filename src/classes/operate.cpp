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

Node* Operate::add(const char* val){
    return this->add(string(val));
}

Node* Operate::add(string val){
    Error e(string("Could not add \"") + val + "\" to the AST tree", 0, ERR_UNK);
    return this->add(val, {}, e);
}

Node* Operate::add(string val, initializer_list<char*> cond, Error& e){
    Node *n = to_operate.top();
    bool res = (cond.begin() == cond.end());
    for (initializer_list<char*>::iterator it = cond.begin(); it != cond.end(); it++){
        // cout << *it << endl;
        res |= (n->getType() == *it);
        if (res) break;
    }
    // cout << res << endl;
    if (!res){
        e.print();
    }
    to_operate.push(n->add(val));
    return to_operate.top();
}

Node* Operate::pop(){
    Node *n = to_operate.top();
    to_operate.pop();
    return n;
}

// move o parent como filho do no especificado
Node* Operate::add_swap(Node *new_parent){
    // remova old_parent da arvore
    Node *old_parent = this->pop();
    this->top()->pop();
    // adicione new_parent como pai de old_parent
    this->top()->add(new_parent);
    new_parent->add(old_parent);
    // adicione new_parent ao stack
    to_operate.push(new_parent);
    return to_operate.top();
}
