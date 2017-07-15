
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


// operations here

string Codegen::add(string dest, string src1, string src2){
    return string("  add ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::mul(string dest, string src1, string src2){
    return string("  mul ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::div(string dest, string src1, string src2){
    return string("  div ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::sub(string dest, string src1, string src2){
    return string("  sub ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

// comparison operations

string Codegen::lt(string dest, string src1, string src2){
    return string("  slt ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::lte(string dest, string src1, string src2){
    return string("  sle ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::gt(string dest, string src1, string src2){
    return string("  sgt ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::gte(string dest, string src1, string src2){
    return string("  sge ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::eq(string dest, string src1, string src2){
    return string("  seq ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::neq(string dest, string src1, string src2){
    return string("  sne ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

// jump to code
string Codegen::jump(string reg){
    return string("  j ") + reg+ "\n";
}

// save function registers into stack
string Codegen::function_save(){
    return save_reg("$ra")        +
           save_reg("$fp")        +
           copy_reg("$t1", "$sp") ;
}

// restore function registers into stack
string Codegen::function_restore(){
    return load_reg("$ra");
}

// execute call to function
string Codegen::function_call(string f){
    return string("  jal  ") + f + "\t\t# call function\n";
}

// return from a function call
string Codegen::function_return(){
    return load_reg("$v0", "0") +
        "  " + func_name + "_ret:" +
        copy_reg("$sp", "$fp") +
        load_reg("$fp") +
        jump("$ra") + "\n";
}

// move stack back
string Codegen::move_stack_back(){
    return string("  addiu $sp, $sp, 4\t\t# move stack back\n");
}

// load register with val (decimal)
string Codegen::load_reg(string reg, string val){
    return string("  li ") + reg + ", " + val + "\t\t# load fixed data\n";
}

// load register with stack data (if move_back, return stack to prev data)
string Codegen::load_reg(string reg){
    return string("  lw ") + reg + ", 4($sp)\t\t# load data\n" +
        move_stack_back();
}

// load a register with a data from $fp
string Codegen::load_reg_from_fp(string reg, string pos){
    return string("  lw ") + reg + ", -" + pos + "($fp)\t\t# load data\n";
}

// load a register with a data from $fp
string Codegen::load_reg_from_t0(string reg, string pos){
    return string("  lw ") + reg + ", " + pos + "($t0)\t\t# load data\n";
}

// save reg into stack
string Codegen::save_reg_to_t0(string reg, string pos){
    return string("  sw ") + reg + ", " + pos + "($t0)\n";
}

// save reg into stack
string Codegen::save_reg(string reg){
    return string("  sw ") + reg + ", 0($sp)\n" +
        "  addiu $sp, $sp, -4\t\t# save reg and move stack\n";
}

// copy reg src to reg dest
string Codegen::copy_reg(string dest, string src){
    return string("  move ") + dest + ", " + src + "\n";
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
        res += func_name + ":\n"      +
              copy_reg("$fp", "$t1")  ;
        // node 2 (paramlist) - function parameters here
        res += generate(*(it++));
        // node 3 (block) - the body of the function goes here
        res += generate(*(it++));
        // return function
        res += function_return();


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
                res += load_reg("$a1", "0");
            } else {
                res += generate(*(it++));
            }
            // save the result of the initialization into the global
            // "stack" / memory
            res += save_reg_to_t0("$a1", to_string(pos));
        } else {
            // initialize global var to 0 if no initialization statement
            // is found
            if (it == n->end()){
               res += load_reg("$a1", "0");
            } else {
                res += generate(*(it++));
            }
            // save the result of the initialization into the local "stack"
            res += save_reg("$a1");
        }


    } else if (n->getType() == AST_ASSIGN) {
        // generate_assign(n);


    } else if (n->getType() == AST_FUNCCALL){
            list<Node*>::iterator it = n->begin();
            // get the destination register for n
            string cmp_reg;
            string dest_reg = get_dest_reg(n, &cmp_reg);
            // funccall name
            Node* id = (*(it++));
            string f_call_name = string("_f_") + id->getType();
            // save registers that the function will change
            res += save_reg(cmp_reg) +
                   function_save()   ;
            // load function arguments
            res += generate(*(it++));
            // perform funccall
            res += function_call( f_call_name ) +
                   copy_reg(dest_reg, "$v0")    +
                   // restore registers the function has changed
                   function_restore() +
                   load_reg(cmp_reg);


    } else if (n->getType() == AST_ARGLIST){
        // get the destination register for n
        string dest_reg;
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            dest_reg = get_dest_reg(*it);
            res += generate(*it)      +
                   save_reg(dest_reg) ;
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
        res += copy_reg("$v0", "$a0")   +
               jump(func_name + "_ret") ;


    } else if (n->getType() == AST_IF){
        // generate_if(n);


    } else if (n->getType() == AST_WHILE){
        // generate_while(n);


    } else if (n->getType() == AST_BREAK){
        // generate_break(n);


    } else if (n->getType() == AST_CONTINUE){
        // generate_continue(n);


    } else if (n->getType() == AST_PROGRAM){
        // create the PROGRAM context
        sym_map[func_name = CG_FNAME_PROGRAM] = MemContext();
        // we are at the beginning of the program, create default structure
        res += ".data\n\n.text\n\n"            +
              string("_f_print:\n")            +
              copy_reg("$fp", "$t1")           +
              load_reg("$a0")                  +
              load_reg("$v0", "1")             +
              "  syscall\t\t# print integer\n" +
              load_reg("$v0", "11")            +
              load_reg("$a0", "0x0a")          +
              "  syscall\t\t# print newline\n" +
              function_return()                ;
        // weve reached the part of the program that declares the functions
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            Node *child = *it;
            if (child->getType() == AST_DECFUNC){
                res += generate(child);
            }
        }
        res += "main:\n" +
              copy_reg("$fp", "$sp")                         +
              load_reg("$t0", "0x10000000")                  ;
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
        res +=  function_save()                              +
              function_call("_f_main")                       +
              function_restore()                             +
              "  move $a0, $v0\n"                            +
              load_reg("$v0", "17")                          +
              "  syscall\t\t# call exit with errcode $v0\n"  ;


    // detect tokens not already specified above
    } else if (n->getToken() == T_SYM){
        // changed register, but not required by the operation
        string cmp_reg;
        // detect dest_reg of the operation
        string dest_reg = get_dest_reg(n, &cmp_reg);
        // protect changed registers
        res += save_reg(cmp_reg);
        // generate operands code
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            res += generate(*it);
        }
        // perform operation
        if (n->getType() == "+"){
            res += add(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "*"){
            res += mul(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "/"){
            res += div(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "<"){
            res += lt(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "<="){
            res += lte(dest_reg, "$a0", "$a1");
        } else if (n->getType() == ">"){
            res += gt(dest_reg, "$a0", "$a1");
        } else if (n->getType() == ">="){
            res += gte(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "=="){
            res += eq(dest_reg, "$a0", "$a1");
        } else if (n->getType() == "!="){
            res += neq(dest_reg, "$a0", "$a1");
        }
       // load protected registers back in
        res += load_reg(cmp_reg);
        // TODO add other operations


    } else if (n->getToken() == T_ID){
        int stack_pos;
        // get the destination register for n
        string dest_reg = get_dest_reg(n);
        // find the variable address and load it into a dest_reg register
        try {
            stack_pos = sym_map[func_name].get(n);
            // if we reach here, we have a local variable
            res += load_reg_from_fp(dest_reg, to_string(stack_pos));
        } catch(Error& e) {
            stack_pos = sym_map[CG_FNAME_PROGRAM].get(n);
            // if we reach here, we have a global variable
            res += load_reg_from_t0(dest_reg, to_string(stack_pos));
        }


    } else if (n->getToken() == T_DEC){
        // get the destination register for n
        string dest_reg = get_dest_reg(n);
        // process new operand
        res += load_reg(dest_reg, n->getType());
    }

    return res;
}

// PUBLIC FUNCTIONS

// run codegenerator
void Codegen::run(){
    cout << generate(program) << endl;
}
