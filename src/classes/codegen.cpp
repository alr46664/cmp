
#include "codegen.h"

using namespace std;

// PRIVATE FUNCTIONS


// find the position of the node n relative to its parents
string Codegen::get_dest_reg(Node *n, string *cmp_reg){
    Node *parent = n->getParent();
    // find the node position in its parent and set the dest register accordingly
    int pos = parent->find(n);
    string dest_r = "$a0";
    string cmp_r = "$a1";
    string dest_reg = dest_r;
    if (pos != 0) {
        dest_reg = cmp_r;
        cmp_r = dest_r;
    }
    if (cmp_reg != NULL)
        *cmp_reg = cmp_r;
    return dest_reg;
}



// generate assembly code based on node n
string Codegen::generate(Node *n){
    string res = "";
    // detect reserved words and structures of the language
    if (n->getType() == AST_DECFUNC){
        list<Node*>::iterator it = n->begin();
        // node 1 (id) - name of the function
        func_name = "_f_" + (*(it++))->getType();
        // create the function context
        sym_map[func_name] = MemContext();
        // generate the asm
        res += func_name + ":\n" +
               Assembly::copy_reg("$fp", "$t1") ;
        // node 2 (paramlist) - function parameters here
        res += generate(*(it++));
        // node 3 (block) - the body of the function goes here
        res += generate(*(it++));
        // return function
        res += Assembly::function_return(func_name);


    } else if (n->getType() == AST_DECVAR) {
        list<Node*>::iterator it = n->begin();
        // get var name
        Node *n_id = *(it++);
        // add var to the symbol table
        sym_map[func_name].add(n_id);
        // add variable to the stack / global place
        if (n->getParent()->getType() == AST_PROGRAM){
            // get the position of the variable in global stack
            int pos = sym_map[func_name].get(n_id);
            // initialize global var to 0 if no initialization statement
            // is found
            if (it == n->end()){
                res += Assembly::load_reg("$a1", "0");
            } else {
                res += generate(*(it++));
            }
            // save the result of the initialization into the global
            // "stack" / memory
            res += Assembly::save_reg_to_t0("$a1", to_string(pos));
        } else {
            // initialize global var to 0 if no initialization statement
            // is found
            if (it == n->end()){
               res += Assembly::load_reg("$a1", "0");
            } else {
                res += generate(*(it++));
            }
            // save the result of the initialization into the local "stack"
            res += Assembly::save_reg("$a1");
        }


    } else if (n->getType() == AST_ASSIGN) {
        int stack_pos;
        list<Node*>::iterator it = n->begin();
        // node 1 (id) - name of the variable to assign
        Node *id = *(it++);
        // node 2 (expr) - run expr
        res += generate(*(it++));
        // find the variable address
        try {
            stack_pos = sym_map[func_name].get(id);
            // if we reach here, we have a local variable
            res += Assembly::save_reg_to_fp("$a1", to_string(stack_pos));
        } catch(Error& e) {
            stack_pos = sym_map[CG_FNAME_PROGRAM].get(id);
            // if we reach here, we have a global variable
            res += Assembly::save_reg_to_t0("$a1", to_string(stack_pos));
        }


    } else if (n->getType() == AST_FUNCCALL){
            list<Node*>::iterator it = n->begin();
            // get the destination register for n
            string cmp_reg;
            string dest_reg = get_dest_reg(n, &cmp_reg);
            // funccall name
            Node* id = (*(it++));
            string f_call_name = string("_f_") + id->getType();
            // save registers that the function will change
            res += Assembly::save_reg(cmp_reg) +
                   Assembly::function_save() ;
            // load function arguments
            res += generate(*(it++));
            // perform funccall
            res += Assembly::function_call( f_call_name ) +
                   Assembly::copy_reg(dest_reg, "$v0") +
                   // restore registers the function has changed
                   Assembly::function_restore() +
                   Assembly::load_reg(cmp_reg);


    } else if (n->getType() == AST_ARGLIST){
        // get the destination register for n
        string dest_reg;
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            dest_reg = get_dest_reg(*it);
            res += generate(*it) +
                   Assembly::save_reg(dest_reg) ;
        }


    } else if (n->getType() == AST_PARAMLIST){
        // add all variables defined inside paramlist to the symbol map
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            sym_map[func_name].add(*it);
        }


    } else if (n->getType() == AST_BLOCK){
        // parse block nodes
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            res += generate(*it);
        }


    } else if (n->getType() == AST_RETURN){
        // get the return result
        res += generate( *(n->begin()) );
        // generate func return
        res += Assembly::copy_reg("$v0", "$a0") +
               Assembly::jump(func_name + "_ret") ;


    } else if (n->getType() == AST_IF){
        int sz_child = n->size_children();
        list<Node*>::iterator it = n->begin();
        string jmp_label  = string("_jmp_")  + to_string(if_count);
        string if_label   = string("_if_")   + to_string(if_count);
        string else_label = string("_else_") + to_string(if_count);
        // node 1 (expr) - generate the expression asm
        res += generate(*(it++)) +
               Assembly::bnez(if_label, "$a0") ;
        if (sz_child > 2){
            res += Assembly::beqz(else_label, "$a0") ;
        }
        // no else, goto jump label
        res += Assembly::jump(jmp_label) ;
        // node 2 (if block) - generate block asm
        res += "  " + if_label + ": \n" +
               generate(*(it++)) +
               Assembly::jump(jmp_label) ;
        // node 3 (else block) - generate block asm
        if (sz_child > 2){
            res += "  " + else_label + ": \n" +
                   generate(*(it++));
        }
        // jump statement
        res += "  " + jmp_label + ": \n";
        if_count++;
        // TODO


    } else if (n->getType() == AST_WHILE){
        // TODO


    } else if (n->getType() == AST_BREAK){
        // TODO


    } else if (n->getType() == AST_CONTINUE){
        // TODO


    } else if (n->getType() == AST_PROGRAM){
        // create the PROGRAM context
        sym_map[func_name = CG_FNAME_PROGRAM] = MemContext();
        // we are at the beginning of the program, create default structure
        res += ".data\n\n.text\n\n"            +
              string("_f_print:\n")            +
              Assembly::copy_reg("$fp", "$t1") +
              Assembly::load_reg("$a0")        +
              Assembly::load_reg("$v0", "1")   +
              "  syscall\t\t# print integer\n" +
              Assembly::load_reg("$v0", "11")  +
              Assembly::load_reg("$a0", "0x0a")+
              "  syscall\t\t# print newline\n" +
              Assembly::function_return(func_name) ;
        // weve reached the part of the program that declares the functions
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            Node *child = *it;
            if (child->getType() == AST_DECFUNC){
                res += generate(child);
            }
        }
        res += "main:\n" +
               Assembly::copy_reg("$fp", "$sp") +
               Assembly::load_reg("$t0", "0x10000000") ;
        // global variables go here
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            Node *child = *it;
            if (child->getType() == AST_DECVAR){
                // correct the funcname stack issue here
                func_name = CG_FNAME_PROGRAM;
                // gen code
                res += generate(child);
            }
        }
        res += Assembly::function_save() +
               Assembly::function_call("_f_main") +
               Assembly::function_restore() +
               Assembly::copy_reg("$a0","$v0") +
               Assembly::load_reg("$v0", "17") +
               "  syscall\t\t# call exit with errcode $v0\n" ;


    // detect tokens not already specified above
    } else if (n->getToken() == T_SYM){
        // changed register, but not required by the operation
        string cmp_reg;
        // detect dest_reg of the operation
        string dest_reg = get_dest_reg(n, &cmp_reg);
        // protect changed registers
        res += Assembly::save_reg(cmp_reg);
        // generate operands code
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            res += generate(*it);
        }
        // perform operation
        // BINARY OPERATIONS
        if (n->getType() == "+"){
            res += Assembly::add(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "-" && n->size_children() > 1){
            res += Assembly::sub(dest_reg, "$a0", "$a1");
        } if (n->getType() == "*"){
            res += Assembly::mul(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "/"){
            res += Assembly::div(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "<"){
            res += Assembly::lt(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "<="){
            res += Assembly::lte(dest_reg, "$a0", "$a1");
        } else if (n->getType() == ">"){
            res += Assembly::gt(dest_reg, "$a0", "$a1");
        } else if (n->getType() == ">="){
            res += Assembly::gte(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "=="){
            res += Assembly::eq(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "!="){
            res += Assembly::neq(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "&&"){
            res += Assembly::a_and(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "||"){
            res += Assembly::a_or(dest_reg, "$a0", "$a1");
        // UNARY OPERATORS
        } else if (n->getType() == "!"){
            res += Assembly::a_not(dest_reg, "$a0");
        } else if (n->getType() == "-" && n->size_children() == 1){
            res += Assembly::neg(dest_reg, "$a0");
        }
        // load protected registers back in
        res += Assembly::load_reg(cmp_reg);


    } else if (n->getToken() == T_ID){
        int stack_pos;
        // get the destination register for n
        string dest_reg = get_dest_reg(n);
        // find the variable address and load it into a dest_reg register
        try {
            stack_pos = sym_map[func_name].get(n);
            // if we reach here, we have a local variable
            res += Assembly::load_reg_from_fp(dest_reg, to_string(stack_pos));
        } catch(Error& e) {
            stack_pos = sym_map[CG_FNAME_PROGRAM].get(n);
            // if we reach here, we have a global variable
            res += Assembly::load_reg_from_t0(dest_reg, to_string(stack_pos));
        }


    } else if (n->getToken() == T_DEC){
        // get the destination register for n
        string dest_reg = get_dest_reg(n);
        // process new operand
        res += Assembly::load_reg(dest_reg, n->getType());
    }

    return res;
}

// PUBLIC FUNCTIONS

// run codegenerator
void Codegen::run(){
    cout << generate(program) << endl;
}
