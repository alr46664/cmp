// custom includes
#include "ast_types.h"
#include "error.h"
#include "semantical.h"
#include "utility.h"

using namespace std;

void Semantical::clean_up(){
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // remove paren from nodes (leave only its content)
        if (child->getType() == AST_PAREN){
            // get paren content and insert it in paren place
            list<Node*>::iterator it_pos = top->remove(it);
            if (child->size_children() == 1){
                Node *paren_child = *(child->begin());
                top->insert(it_pos, paren_child);
            }
            // free paren list and release its memory
            child->clear();
            delete child;
        } else if (child->getType() == AST_DECVAR){
            if (child->size_children() > 0){
                // if we have an assign inside a decvar, remove it and
                // add the assign's children to decvar
                Node *decvar_child = *(child->begin());
                if (decvar_child->getType() == AST_ASSIGN){
                    // remove assign from decvar
                    child->pop_front();
                    // pass all assign children to the decvar node
                    for (list<Node*>::iterator it_assign = decvar_child->begin(); it_assign != decvar_child->end(); it_assign++){
                        child->add(*it_assign);
                    }
                    // free decvar_child (assign token)
                    decvar_child->clear();
                    delete decvar_child;
                }
            }
        }
    }
}


void Semantical::verify_children(bool test_all, initializer_list<char*> token_test, initializer_list<char*> type_test, const string& msg){
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // if at least one condition (type_test) is provided,
        // check if its valid, if not, throw an error
        bool res = (test_all ?
            true:
            (type_test.begin() == type_test.end() && token_test.begin() == token_test.end())
            );
        for(initializer_list<char*>::iterator it_type = type_test.begin(); it_type != type_test.end(); it_type++){
            if (test_all){
                res &= (child->getType() == *it_type);
            } else {
                res |= (child->getType() == *it_type);
            }
        }
        for(initializer_list<char*>::iterator it_token = token_test.begin(); it_token != token_test.end(); it_token++){
            if (test_all){
                res &= (child->getToken() == *it_token);
            } else {
                res |= (child->getToken() == *it_token);
            }
        }
        if (!res) throw Error(msg, child->getType(), child->getLine(), ERR_SEM);
        // the children is fine, add it to the stack
        operate.push_back(child);
        // TODO remove debugging statement below
        // cout << *child << endl << endl;
    }
}

void Semantical::check_program(){
    verify_children(false, {}, {AST_DECFUNC, AST_DECVAR}, string("The root of the program can only have function declarations and variable declarations.") );
    // run a verification to see if the program has at least one main function
    bool res = false;
    int i = 0;
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        res |= (child->getType() == AST_DECFUNC && child->size_children() == 3 && (*child->begin())->getType() == "main");
        if (res) i++;
    }
    if (!res || i > 1) throw Error(string("The program needs exactly ONE main function to work!"), top->getType(), top->getLine(), ERR_SEM);
}

void Semantical::check_decfunc(){
    // this variable keep track of the position of the child
    int i = 0;
    // this variable keep track if the decfunc is right formed
    bool res = true;
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        switch(i++){
        case 0:
            if (child->getToken() != T_ID) res = false;
            break;
        case 1:
            if (child->getType() != AST_PARAMLIST) res = false;
            break;
        case 2:
            if (child->getType() != AST_BLOCK) res = false;
            break;
        default:
            res = false;
            break;
        }
        // cout << i << " - " << child->getToken() << " - " << child->getType() << endl;
    }
    if (!res) throw Error(string("Function declaration syntax incorrect!"), top->getType(), top->getLine(), ERR_SEM);
}

void Semantical::check_decvar(){
    // this variable keep track of the position of the child
    int i = 0;
    // this variable keep track if it is right formed
    bool res = true;
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        switch(i++){
        case 0:
            if (child->getToken() != T_ID) res = false;
            break;
        case 1:
            // TODO add expression check here
            if (!Utility::isExprResult(child->getType(), child->getToken())) res = false;
            break;
        default:
            res = false;
            break;
        }
    }
    if (!res) throw Error(string("Variable declaration syntax incorrect!"), top->getType(), top->getLine(), ERR_SEM);
}

void Semantical::check_assign(){
    // TODO
}

void Semantical::check_funccall(){
    // TODO
}

void Semantical::check_arglist(){
    // TODO
}

void Semantical::check_paramlist(){
    verify_children(false, {T_ID}, {}, string("The paramlist of a function declaration can only contain ID's.") );
}

void Semantical::check_block(){
    verify_children(false, {}, {AST_DECVAR, AST_ASSIGN, AST_FUNCCALL, AST_RETURN, AST_IF, AST_WHILE, AST_BREAK, AST_CONTINUE}, string("A block of code defined by {} can contain variable declarations, function calls, return, if and while statements, break and continue.") );
}

void Semantical::check_return(){
    // TODO
}

void Semantical::check_if(){
    // TODO
}

void Semantical::check_while(){
    // TODO
}

void Semantical::check_break(){
    // TODO
}

void Semantical::check_continue(){
    // TODO
}

void Semantical::run(){
    cout << "SEMANTICAL - BEGIN\n\n";
    while(!operate.empty()) {
        top = operate.front();
        // remove top from stack
        operate.pop_front();
        // DEBUGGING STACK
        cout << "PROCESSING: \n\t" << *top << "\n\n";
        // do some clean up inside the nodes
        clean_up();
        // check the children nodes
             if (top->getType() == AST_PROGRAM)    check_program();
        else if (top->getType() == AST_DECFUNC)    check_decfunc();
        else if (top->getType() == AST_DECVAR)     check_decvar();
        else if (top->getType() == AST_ASSIGN)     check_assign();
        else if (top->getType() == AST_FUNCCALL)   check_funccall();
        else if (top->getType() == AST_ARGLIST)    check_arglist();
        else if (top->getType() == AST_PARAMLIST)  check_paramlist();
        else if (top->getType() == AST_BLOCK)      check_block();
        else if (top->getType() == AST_RETURN)     check_return();
        else if (top->getType() == AST_IF)         check_if();
        else if (top->getType() == AST_WHILE)      check_while();
        else if (top->getType() == AST_BREAK)      check_break();
        else if (top->getType() == AST_CONTINUE)   check_continue();
    }
    // free up memory
    clear();
    cout << "\nSEMANTICAL - END\n\n";
}

// reduce memory usage
void Semantical::clear(){
    operate.clear();
}
