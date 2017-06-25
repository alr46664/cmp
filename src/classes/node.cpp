#include "node.h"

using namespace std;


Node::Node(){ /* dont do anything */}

Node::Node(string token, string type): Node(){
    this->setToken(token);
    this->setType(type);
}

Node::~Node(){
    for (list<Node*>::iterator it = children.begin(); it != children.end(); it++){
        if (*it != NULL){
            delete *it;
        }
    }
}

int Node::size_children(){
    return children.size();
}

void Node::setType(string t){
    this->type = t;
}

string Node::getType(){
    return this->type;
}

void Node::setToken(string t){
    this->token = t;
}
string Node::getToken(void){
    return this->token;
}

Node* Node::add(){
    this->children.push_back(new Node());
    return this->children.back();
}

Node* Node::add(string token, string type){
    Node *n = this->add();
    n->setType(type);
    n->setToken(token);
    return n;
}

Node* Node::add(Node* n){
    this->children.push_back(n);
    return this->children.back();
}

Node* Node::top(){
    return this->children.back();
}

Node* Node::bottom(){
    return this->children.front();
}

Node* Node::pop_front(){
    Node *n = NULL;
    if (!children.empty()){
        n = this->children.front();
        this->children.pop_front();
    }
    return n;
}

Node* Node::pop(){
    Node *n = NULL;
    if (!children.empty()){
        n = this->children.back();
        this->children.pop_back();
    }
    return n;
}

list<Node*>::iterator Node::insert(list<Node*>::iterator i, Node *n){
    return children.insert(i, n);
}

list<Node*>::iterator Node::remove(Node *n){
    list<Node*>::iterator it = children.begin();
    while (it != children.end() && *it != n){
        it++;
    }
    if (it != children.end()){
        return (children.erase(it));
    }
    return children.end();
}
