#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <list>

#include "context.h"

class Node {
private:
    std::string token, type;
    int line;

    Node* parent;
    std::list<Node*> children;

    Context* context;
public:
    //   MEMBER
    Node();
    Node(std::string token, std::string type, int line);
    ~Node();

    void createContext(std::string c);
    Context* getContext();

    int size_children();

    void setParent(Node* p);
    Node* getParent();

    void setType(std::string t);
    std::string getType();

    void setToken(std::string t);
    std::string getToken();

    void setLine(int l);
    int getLine();

    Node* add();
    Node* add(std::string token, std::string type, int line);
    Node* add(Node* n);

    Node* top();
    Node* bottom();
    Node* pop();
    Node* pop_front();
    void clear();

    std::list<Node*>::iterator insert(std::list<Node*>::iterator i, Node *n);
    std::list<Node*>::iterator remove(std::list<Node*>::iterator it);
    std::list<Node*>::iterator remove(Node *n);

    std::list<Node*>::iterator begin();
    std::list<Node*>::iterator end();
    std::list<Node*>::reverse_iterator rbegin();
    std::list<Node*>::reverse_iterator rend();

    Node* operator[](int i){
        if (i < children.size() && i >= 0) {
            std::list<Node*>::iterator it = children.begin();
            for (int j = 0; j < i; j++){it++;}
            return *it;
        }
        return NULL;
    }

    //    NON-MEMBER
    // (friend allows access to the
    // private / protected variables)
    friend std::ostream& operator<<(std::ostream &output, const Node &N){
        output << "[" << N.type;
        for(std::list<Node*>::const_iterator it = N.children.begin(); it != N.children.end(); it++){
            output << " " << **it;
        }
        output << "]";
        return output;
    }
};


#endif // NODE_H
