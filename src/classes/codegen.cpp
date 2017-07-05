
#include "codegen.h"

using namespace std;

// PRIVATE FUNCTIONS

// print the to_print stack
void Codegen::commit_print_stack(){
    for (list<string>::iterator it = to_print.begin(); it != to_print.end() ; ++it){
        cout << *it;
    }
    to_print.clear();
    ins = to_print.end();
}

// insert string to the print stack and return its iterator
list<string>::iterator Codegen::insert_print_stack(string s){
    list<string>::iterator it = to_print.insert(ins, s);
    // point ins to the next iterator of the stack
    ins = it; ins++;
    return it;
}


string Codegen::function_call(string f){
    return save_reg("$ra") +
        save_reg("$fp") +
        copy_reg("$fp", "$sp") +
        // TODO: pass function variables here
        string("  jal  ") + f + "\t\t# call function\n"+
        load_reg("$ra");
}

string Codegen::function_return(){
    return load_reg("$v0", "0") +
        "  " + func_name + "_ret:" + load_reg("$fp") +
        "  j $ra\n\n";
}


string Codegen::move_stack_back(){
    return string("  addiu $sp, $sp, 4\t\t# move stack back\n");
}

string Codegen::load_reg(string reg, string val){
    return string("  li ") + reg + ", " + val + "\t\t# load fixed data\n";
}

// load register with stack data (if move_back, return stack to prev data)
string Codegen::load_reg(string reg, int move_back){
    return string("  lw ") + reg + ", 4($sp)\t\t# load data\n" +
        (move_back == 0 ? "" : move_stack_back());
}

string Codegen::save_reg(string reg){
    return string("  sw ") + reg + ", 0($sp)\n" +
        "  addiu $sp, $sp, -4\t\t# save reg and move stack\n";
}

string Codegen::copy_reg(string dest, string src){
    return string("  move ") + dest + ", " + src + "\n";
}


// generate assembly code based on node n
void Codegen::generate(Node *n){
    if (n->getType() == AST_PROGRAM){
        // iterator to the main: function of the print stack
        std::list<std::string>::iterator main_it;
        // we are at the beginning of the program, create default structure
        insert_print_stack(  ".data\n\n.text\n\n"               );
        func_name = string("_f_print");
        insert_print_stack(  func_name + ":\n"                  );
        insert_print_stack(  load_reg("$a0")                    );
        insert_print_stack(  load_reg("$v0", "1")               );
        insert_print_stack(  "  syscall\t\t# print integer\n"   );
        insert_print_stack(  load_reg("$v0", "11")              );
        insert_print_stack(  load_reg("$a0", "0x0a")            );
        insert_print_stack(  "  syscall\t\t# print newline\n"   );
        insert_print_stack(  function_return()                  );
        // weve reached the end of the program, create main function
        main_it = insert_print_stack(  "main:\n"                                      );
        insert_print_stack(            function_call("_f_main")                       );
        insert_print_stack(            "  move $a0, $v0\n"                            );
        insert_print_stack(            load_reg("$v0", "17")                          );
        insert_print_stack(            "  syscall\t\t# call exit with errcode $v0\n"  );
        // the rest of the data must go before the main: function
        ins = main_it;
    } else if (n->getType() == AST_DECFUNC){
        // function body must come before the return of the function
        std::list<std::string>::iterator func_ret;
        // name of the function
        func_name = "_f_" + (*n)[0]->getType();
        // generate the asm
        insert_print_stack(             func_name + ":\n"  );
        // TODO: read function args from stack (removing them from there)
        func_ret = insert_print_stack(  function_return()  );
        ins = func_ret;
    }
    // cout << *n << "\n\n";
}

// PUBLIC FUNCTIONS

// run codegenerator
void Codegen::run(){
    // build all stack based on the operation order
    list<Node*>::iterator act = operate.begin();
    list<Node*>::iterator ins = operate.end();
    while(act != ins) {
        Node *n = *act;
        // generate assembly based on n
        generate(n);
        // get all children of the n node and insert them one after another
        // preserving their relative order
        for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
            ins = operate.insert(ins, *it);
            ins++;
        }
        // remove already seen node, update act to the next node,
        // and make ins = act + 1 (next node)
        operate.erase(act++);
        ins = act;
        if (ins != operate.end())
            ins++;
    }
    // print the whole stack block into the file
    commit_print_stack();
}
