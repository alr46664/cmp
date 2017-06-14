#ifndef NODE_H
#define NODE_H


#include <iostream>
#include <string>
#include <list>

class Node {
private:
    std::string type;
    std::list<Node> children;
public:
    //   MEMBER
    Node();
    Node(std::string t);
    Node(std::string t, std::initializer_list<Node> nodes);

    void setType(std::string t);
    std::string getType(void);

    Node& add(Node &n);

    //    NON-MEMBER
    // (friend allows access to the
    // private / protected variables)
    friend std::ostream& operator<<(std::ostream &output, const Node &N){
        output << "[" << N.type;
        if (!N.children.empty()){
            for(std::list<Node>::const_iterator it = N.children.begin(); it != N.children.end(); it++){
                output << " " << *it;
            }
        }
        output << "]";
        return output;
    }
};


#endif // NODE_H
