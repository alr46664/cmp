#ifndef NODE_H
#define NODE_H


#include <iostream>
#include <string>
#include <list>

class Node {
private:
    std::string token,type;
    std::list<Node*> children;
public:
    //   MEMBER
    Node();
    Node(std::string token, std::string type);
    ~Node();

    int size_children();

    void setType(std::string t);
    std::string getType(void);

    void setToken(std::string t);
    std::string getToken(void);

    Node* add();
    Node* add(std::string token, std::string type);
    Node* add(Node* n);

    Node* top();
    Node* bottom();
    Node* pop();
    Node* pop_front();

    std::list<Node*>::iterator insert(std::list<Node*>::iterator i, Node *n);
    std::list<Node*>::iterator remove(Node *n);

    Node* operator[](int i){
        if (i < children.size() && i > 0) {
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
