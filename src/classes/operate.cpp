
#include "operate.h"

using namespace std;

bool Operate::empty(){
    return to_operate.empty();
}

Node Operate::top(){
    return to_operate.top();
}

void Operate::add(const char* val){
    this->add(string(val));
}

void Operate::add(string val){
    Node node(val);
    to_operate.push(node);
}

void Operate::add(Node& n, Error& e){
    if (!empty()){
        // faca n ser o pai de top, e top ser o primeiro ramo de n
        Node top = to_operate.top();
        to_operate.pop();
        n.add(top);
        to_operate.push(n);
    }
    e.print();
}

void Operate::add(string val, initializer_list<char*> cond, Error& e){
    Node node(val);
    if (!empty()){
        Node &n = to_operate.top();
        bool res = false;
        for (initializer_list<char*>::iterator it = cond.begin(); it != cond.end(); it++){
            res |= (n.getType() == *it);
            if (res) break;
        }
        if (res){
            n.add(val);
            return;
        }
    }
    e.print();
}

Node Operate::pop(){
    Node n = to_operate.top();
    to_operate.pop();
    return n;
}
