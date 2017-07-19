// custom includes
#include "ast_types.h"
#include "error.h"
#include "sintatical.h"
#include "utility.h"

using namespace std;

void Sintatical::clean_up(){
    // while something has changed, then continue the cleanup
    bool clean_continue = true;
    while (clean_continue){
        // lets reset the clean continue var
        clean_continue = false;
        for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
            Node *child = *it;
            // remove paren from nodes (leave only its content)
            if (child->getType() == AST_PAREN){
                clean_continue = true;
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
                        clean_continue = true;
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
            } else if (child->getType() == "u-"){
                clean_continue = true;
                child->setType("-");
            }
        }
    }
}


void Sintatical::verify_children(bool test_all, initializer_list<char*> token_test, initializer_list<char*> type_test, const string& msg){
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
        if (!res) throw Error(msg, child->getType(), child->getLine(), ERR_SIN);
        // the children is fine, add it to the stack
        operate.push_back(child);
    }
}

void Sintatical::check_program(){
    verify_children(false, {}, {AST_DECFUNC, AST_DECVAR}, string("The root of the program can only have function declarations and variable declarations.") );
    // run a verification to see if the program has at least one main function
    int i = 0;
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        if (child->getType() == AST_DECFUNC && child->size_children() > 0 && (*child->begin())->getType() == "main")
            i++;
    }
    if (i != 1) throw Error(string("The program needs exactly ONE main function to work!"), top->getType(), top->getLine(), ERR_SIN);
}

void Sintatical::check_decfunc(){
    Node *decfunc = top;
    Node *decfunc_id, *decfunc_block;
    int paramlist_sz = 0;
    // this variable keep track of the position of the child
    int i = 0;
    // this variable keep track if the decfunc is right formed
    bool res = (top->size_children() == 3);
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        switch(i++){
        case 0:
            if (child->getToken() != T_ID) {
                res = false;
            } else {
                decfunc_id = child;
            }
            break;
        case 1:
            if (child->getType() != AST_PARAMLIST) {
                res = false;
            } else {
                paramlist_sz = child->size_children();
            }
            break;
        case 2:
            if (child->getType() != AST_BLOCK) {
                res = false;
            } else {
                decfunc_block = child;
            }
            break;
        default:
            res = false;
            break;
        }
        // cout << i << " - " << child->getToken() << " - " << child->getType() << endl;
    }
    if (!res) throw Error(string("Function declaration syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
    // add a new defined function inside the previous context,
    // and create this new function scope as well
    sem.set(decfunc_id, DEFINED_FUNCTION, paramlist_sz);
    sem.addContext(decfunc_id->getType(), decfunc);
}

void Sintatical::check_decvar(){
    Node* decvar = top;
    // this variable keep track of the position of the child
    int i = 0;
    // this variable keep track if it is right formed
    bool res = (top->size_children() > 0);
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        switch(i++){
        case 0:
            if (child->getToken() != T_ID) {
                res = false;
            } else {
                // add those ids to the semantical analisis
                sem.set(child, DEFINED_VARIABLE);
            }
            break;
        case 1:
            if (!Utility::isExprResult(child->getType(), child->getToken())) res = false;
            break;
        default:
            res = false;
            break;
        }
    }
    if (!res) throw Error(string("Variable declaration syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
}

void Sintatical::check_assign(){
    // this variable keep track of the position of the child
    int i = 0;
    // this variable keep track if it is right formed
    bool res = (top->size_children() == 2);
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        switch(i++){
        case 0:
            if (child->getToken() != T_ID) {
                res = false;
            } else {
                // perform the semantical verification
                Defined& v = sem.get(child);
                if (v.getType() != DEFINED_VARIABLE)
                    throw Error(string("The id you're trying to perform an assign is not a variable (its a function)!"), top->getType(), top->getLine(), ERR_SEM);
            }
            break;
        case 1:
            if ( !Utility::isExprResult(child->getType(), child->getToken()) ) res = false;
            break;
        default:
            res = false;
            break;
        }
    }
    if (!res) throw Error(string("Assign (\"=\") syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
}

void Sintatical::check_funccall(){
    Defined* d;
    // this variable keep track of the position of the child
    int i = 0;
    // this variable keep track if it is right formed
    bool res = (top->size_children() == 2);
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        switch(i++){
        case 0:
            if (child->getToken() != T_ID){
                res = false;
            } else {
                d = &sem.get(child);
                if (d->getType() != DEFINED_FUNCTION)
                    throw Error(string("The id specified is not a function id (its probably a variable). Check your function declarations!"), top->getType(), top->getLine(), ERR_SIN);
            }
            break;
        case 1:
            if (child->getType() != AST_ARGLIST) {
                res = false;
            } else {
                if (d->getArgSize() != child->size_children())
                    throw Error(string("The function you are trying to call requires a certain # of arguments. Check if that number is correct!"), top->getType(), top->getLine(), ERR_SIN);
            }
            break;
        default:
            res = false;
            break;
        }
    }
    if (!res) throw Error(string("Function call syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
}

void Sintatical::check_arglist(){
    // this variable keep track if it is right formed
    bool res = true;
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        if ( !Utility::isExprResult(child->getType(), child->getToken()) )
            res = false;
    }
    if (!res) throw Error(string("Arglist requires an expression! Syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
}

void Sintatical::check_paramlist(){
    Node *paramlist = top;
    verify_children(false, {T_ID}, {}, string("The paramlist of a function declaration can only contain ID's.") );
    // add those ids to the semantical analisis
    for(list<Node*>::iterator it = paramlist->begin(); it != paramlist->end(); it++){
        Node *child = *it;
        sem.set(child, DEFINED_VARIABLE);
    }
}

void Sintatical::check_block(){
    Node *block = top;
    verify_children(false, {}, {AST_DECVAR, AST_ASSIGN, AST_FUNCCALL, AST_RETURN, AST_IF, AST_WHILE, AST_BREAK, AST_CONTINUE}, string("A block of code defined by {} can contain variable declarations, function calls, return, if and while statements, break and continue.") );
    if (block->getParent()->getType() != AST_DECFUNC){
        sem.addContext(string("block_") + to_string(block_count++), block);
    }
}

void Sintatical::check_return(){
    if ( top->size_children() != 1 || !Utility::isExprResult( (*top->begin())->getType(), (*top->begin())->getToken() ) )
        throw Error(string("Return syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
    Node *child = (*top->begin());
    operate.push_back(child);
}

void Sintatical::check_if(){
    Node* n_if = top;
    // this variable keep track of the position of the child
    int i = 0;
    // this variable keep track if it is right formed
    bool res = (top->size_children() == 2 || top->size_children() == 3);
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        switch(i++){
        case 0:
            if (!Utility::isExprResult(child->getType(), child->getToken())) res = false;
            break;
        case 1:
        case 2:
            if (child->getType() != AST_BLOCK) {
                res = false;
            }
            break;
        default:
            res = false;
            break;
        }
    }
    if (!res) throw Error(string("If [else] syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
}

void Sintatical::check_while(){
    Node* n_while = top;
    // this variable keep track of the position of the child
    int i = 0;
    // this variable keep track if it is right formed
    bool res = (top->size_children() == 2);
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // put the child inside the stack
        operate.push_back(child);
        switch(i++){
        case 0:
            if (!Utility::isExprResult(child->getType(), child->getToken())) res = false;
            break;
        case 1:
            if (child->getType() != AST_BLOCK) {
                res = false;
            }
            break;
        default:
            res = false;
            break;
        }
    }
    if (!res) throw Error(string("While syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
}

void Sintatical::check_break(){
    Node* n = top;
    // sintatical analisis
    bool res = (top->size_children() == 0);
    if (!res) throw Error(string("Break syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
    // semantical analisis - is inside loop?
    while (n != NULL && n->getType() != AST_WHILE){
        n = n->getParent();
    }
    if (n == NULL)
        throw Error(string("Break statement can only be used inside a while loop!"), top->getType(), top->getLine(), ERR_SEM);
}

void Sintatical::check_continue(){
    Node* n = top;
    // sintatical analisis
    bool res = (top->size_children() == 0);
    if (!res) throw Error(string("Continue syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
    // semantical analisis - is inside loop?
    while (n != NULL && n->getType() != AST_WHILE){
        n = n->getParent();
    }
    if (n == NULL)
        throw Error(string("Continue statement can only be used inside a while loop!"), top->getType(), top->getLine(), ERR_SEM);
}

void Sintatical::check_expr(){
    // lets check for the unary and binary operators
    bool res = ( (Utility::isBinaryOp(top->getType()) && top->size_children() == 2) ||
        ( Utility::isUnaryOp(top->getType()) && top->size_children() == 1 ) );
    for(list<Node*>::iterator it = top->begin(); it != top->end(); it++){
        Node *child = *it;
        // cout << "\n Top: " << *top << " - Child: " << *child << "\n\n";
        // put the child inside the stack
        operate.push_back(child);
        if ( !Utility::isExprResult( child->getType(), child->getToken() ) ) res = false;
    }
    if (!res) throw Error(string("Expression syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
    res = false;
}

void Sintatical::check_id(){
    Node* parent = top->getParent();
    // sintatical analisis
    bool res = (top->size_children() == 0);
    if (!res) throw Error(string("ID syntax incorrect!"), top->getType(), top->getLine(), ERR_SIN);
    // do semantical analisis
    Defined& d = sem.get(top);
    if (parent != NULL && parent->getType() != AST_FUNCCALL && parent->getType() != AST_DECFUNC){
        // we know this id is not a function, so it must be a variable!
        if (d.getType() != DEFINED_VARIABLE)
            throw Error(string("The id you're trying to use is not a variable. So it's a function and requires the () to be called."), top->getType(), top->getLine(), ERR_SEM);
    }
    // change the name of the defined variable/function
    // (WE SHOULD NOT ENABLE THIS - THE EXAMPLE OUTPUT SUGGESTS WE KEEP
    // THE MAIN VARIABLE NAMES AS THEY WERE DEFINED BY THE PROGRAMMER!)
    // top->setType(d.getName());
}

void Sintatical::run(){
    // cout << "Sintatical - BEGIN\n\n";
    while(!operate.empty()) {
        top = operate.front();
        // remove top from stack
        operate.pop_front();
        // do some clean up inside the nodes
        clean_up();
        // DEBUGGING STACK
        // Node* p = top->getParent();
        // if (p != NULL)
        //     cout << "PARENT: " << *p << "\n";
        // cout <<     "TOP:    " << *top << "\n\n";
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
        else if ( Utility::isBinaryOp(top->getType()) ||
            Utility::isUnaryOp(top->getType()) )   check_expr();
        else if (top->getToken() == T_ID)          check_id();
        else if (top->getToken() != T_DEC)
            throw Error(string("Node of type \"") + top->getToken()  + "\" not identified!", top->getType(), top->getLine(), ERR_SIN);
    }
    // free up memory
    clear();
    // cout << "\nSintatical - END\n\n";
}

// reduce memory usage
void Sintatical::clear(){
    operate.clear();
}
