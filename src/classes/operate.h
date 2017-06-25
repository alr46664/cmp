#ifndef OPERATE_H
#define OPERATE_H

#include <string>
#include <stack>
#include <initializer_list>

#include "ast_types.h"
#include "error.h"
#include "node.h"

class Operate {
private:
    // main AST node
    Node *program;

    // this stack will hold the Node that we need to operate
    // prior to adding it to the other nodes
    std::stack<Node*> to_operate;
public:
    //   MEMBER
    Operate(){
        to_operate.push(program = new Node(T_PROGRAM, AST_PROGRAM));
    }
    ~Operate(){
        delete program;
    }

    Node* getProgramAST();

    bool empty();
    Node* top();

    Node* add(std::string token, std::string val);

    Node* add_swap(Node *new_parent);

    void push(Node *n);
    Node* pop();
};

#endif // OPERATE_H
