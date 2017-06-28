#include "node.h"

using namespace std;


Node::Node(){
    token = type = "";
    line = 0;
}

Node::Node(string token, string type, int line): Node(){
    this->setToken(token);
    this->setType(type);
    this->setLine(line);
}

Node::~Node(){
    for (list<Node*>::iterator it = children.begin(); it != children.end(); it++){
        if (*it != NULL){
            delete *it;
        }
    }
}

list<Node*>::iterator Node::begin(){
    return children.begin();
}

list<Node*>::iterator Node::end(){
    return children.end();
}

list<Node*>::reverse_iterator Node::rbegin(){
    return children.rbegin();
}

list<Node*>::reverse_iterator Node::rend(){
    return children.rend();
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
string Node::getToken(){
    return this->token;
}

void Node::setLine(int l){
    this->line = l;
}
int Node::getLine(){
    return this->line;
}

Node* Node::add(){
    this->children.push_back(new Node());
    return this->children.back();
}

Node* Node::add(string token, string type, int line){
    Node *n = this->add();
    n->setType(type);
    n->setToken(token);
    n->setLine(line);
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

void Node::clear(){
    children.clear();
}

list<Node*>::iterator Node::insert(list<Node*>::iterator i, Node *n){
    return children.insert(i, n);
}

list<Node*>::iterator Node::remove(list<Node*>::iterator it){
    return children.erase(it);
}

list<Node*>::iterator Node::remove(Node *n){
    list<Node*>::iterator it = children.begin();
    while (it != children.end() && *it != n){
        it++;
    }
    if (it != children.end()){
        return this->remove(it);
    }
    return children.end();
}
