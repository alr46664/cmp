#include "node.h"

using namespace std;


Node::Node(){ /* dont do anything */}

Node::Node(string t): Node(){
    this->setType(t);
}

Node::Node(string t, initializer_list<Node> nodes): Node(t) {
    for (initializer_list<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it){
        Node temp = *it;
        this->add(temp);
    }
}


void Node::setType(string t){
    this->type = t;
}

string Node::getType(){
    return this->type;
}

Node& Node::add(Node &n){
    this->children.push_back(n);
    return *this;
}


