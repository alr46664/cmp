#include "node.h"

using namespace std;


Node::Node(){ /* dont do anything */}

Node::Node(string t): Node(){
    this->setType(t);
}


void Node::setType(string t){
    this->type = t;
}

string Node::getType(){
    return this->type;
}

Node& Node::add(){
    Node n;
    this->children.push_back(n);
    return this->children.back();
}

Node& Node::add(string t){
    Node &n = this->add();
    n.setType(t);
    return n;
}

Node& Node::add(Node& n){
    this->children.push_back(n);
    return this->children.back();
}
