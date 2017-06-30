
#include "context.h"

using namespace std;

// add a new variable/function to this context
void Context::add(string n, string t, int s){
    context[n] = Defined(name + "_" + n, t, s);
}

// gets the context name
string Context::getName(){
    return name;
}

map<string, Defined>::iterator Context::find(string n){
    return context.find(n);
}

map<string, Defined>::iterator Context::begin(){
    return context.begin();
}

map<string, Defined>::iterator Context::end(){
    return context.end();
}
