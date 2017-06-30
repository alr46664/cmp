#include "defined.h"

using namespace std;

void Defined::setName(std::string n){
    name = n;
}

void Defined::setType(std::string t){
    type = t;
}

void Defined::setArgSize(int s){
    if (s >= 0)
        arg_size = s;
}

std::string Defined::getName() const{
    return name;
}

std::string Defined::getType() const{
    return type;
}

int Defined::getArgSize() const{
    return arg_size;
}
