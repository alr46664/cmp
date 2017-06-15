#include "node.h"

using namespace std;


Node::Node(){ /* dont do anything */}

Node::Node(string t): Node(){
    this->setType(t);
}

Node::~Node(){
    for (list<Node*>::iterator it = children.begin(); it != children.end(); it++){
        if (*it != NULL){
            delete *it;
        }
    }
}

void Node::setType(string t){
    this->type = t;
}

string Node::getType(){
    return this->type;
}

Node* Node::add(){
    this->children.push_back(new Node());
    return this->children.back();
}

Node* Node::add(string t){
    Node *n = this->add();
    n->setType(t);
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
    Node *n = this->children.front();
    this->children.pop_front();
    return n;
}

Node* Node::pop(){
    Node *n = this->children.back();
    this->children.pop_back();
    return n;
}
