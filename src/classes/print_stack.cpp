
#include "print_stack.h"

using namespace std;

void PrintStack::setMain(list<string>::iterator it){
    main_pr = it;
}
list<string>::iterator& PrintStack::getMain(){
    return main_pr;
}

void PrintStack::setGlobalVar(list<string>::iterator it){
    global_pr = it;
}
list<string>::iterator& PrintStack::getGlobalVar(){
    return global_pr;
}

// insert string to the print stack and return its iterator
// if dir == 0, insert backwards, otherwise insert forward
list<string>::iterator PrintStack::push_to_print(string s, int dir){
    // save actual pos
    list<string>::iterator& ins = to_insert.back();
    list<string>::iterator it = to_print.insert( ins, s );
    // point ins to the next iterator of the stack
    ins = it;
    if (dir != 0){
        ins++;
    }
    return it;
}

// define the next position to insert text into
void PrintStack::push_to_insert(list<string>::iterator it){
    // this is to keep track of where we were
    to_insert.push_back(it);
    // this is going to keep track of where we want to be right now
    to_insert.push_back(it);
}

// pop an element from the to_insert
void PrintStack::pop_to_insert(){
    if (to_insert.empty())
        throw Error("Insert stack is empty! Could not pop it!", "Code Generator",0 , ERR_UNK);
    to_insert.pop_back();
}

// clear the to_insert stack
void PrintStack::clear_to_insert(){
    to_insert.clear();
    push_to_insert( to_print.end() );
}
