#ifndef OPERATE_H
#define OPERATE_H

#include <string>
#include <stack>
#include <initializer_list>

#include "error.h"
#include "node.h"

class Operate {
private:
    // this stack will hold the Node that we need to operate
    // prior to adding it to the other nodes
    std::stack<Node> to_operate;
public:
    //   MEMBER
    Operate(){};

    bool empty();
    Node top();

    void add(const char* val);
    void add(std::string val);
    void add(Node& n, Error& e);
    void add(std::string val, std::initializer_list<char*> cond, Error& e);

    Node pop();
};

#endif // OPERATE_H
