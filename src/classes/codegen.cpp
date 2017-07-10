
#include "codegen.h"

using namespace std;

// PRIVATE FUNCTIONS


// find the position of the node n relative to its parents
string Codegen::get_dest_reg(Node *n){
    Node *parent = n->getParent();
    // find the node position in its parent and set the dest register accordingly
    int pos = parent->find(n);
    string dest_reg = "$a0";
    if (pos != 0) {
        dest_reg = "$a1";
    }
    // // TODO: remote debugging statements
    // cout << "\tPRT: " << *parent << "\n";
    // cout << "\tPROCESSING: " << *n << "\n";
    // cout << "\tPOS: " << pos << "\n\n";
    // // END DEBUG
    return dest_reg;
}


// operations here

string Codegen::add(string dest, string src1, string src2){
    return string("  add ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

string Codegen::sub(string dest, string src1, string src2){
    return string("  sub ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

// jump to code
string Codegen::jump(string reg){
    return string("  j ") + reg+ "\n";
}

// save function registers into stack
string Codegen::function_save(){
    return save_reg("$ra") +
        save_reg("$fp") +
        copy_reg("$fp", "$sp");
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



void Codegen::generate_program(Node *n){
    // create the PROGRAM context
    sym_map[func_name = CG_FNAME_PROGRAM] = MemContext();
    // we are at the beginning of the program, create default structure
    ps.push_to_print(  ".data\n\n.text\n\n"               );
    ps.push_to_print(  "_f_print:\n"                  );
    ps.push_to_print(  load_reg("$a0")                    );
    ps.push_to_print(  load_reg("$v0", "1")               );
    ps.push_to_print(  "  syscall\t\t# print integer\n"   );
    ps.push_to_print(  load_reg("$v0", "11")              );
    ps.push_to_print(  load_reg("$a0", "0x0a")            );
    ps.push_to_print(  "  syscall\t\t# print newline\n"   );
    ps.push_to_print(  function_return()                  );
    // weve reached the end of the program, create main function and
    // save it into the PrintStack
    ps.setMain(  ps.push_to_print(  "main:\n"                                    )  );
    // continue with main creation
    ps.push_to_print(            copy_reg("$fp", "$sp")                         );
    ps.push_to_print(            load_reg("$t0", "0x10000000")                  );
    // save this position to the print stack (global variables go here)
    ps.setGlobalVar( ps.push_to_print(  function_save()                              ) );
    ps.push_to_print(            function_call("_f_main")                       );
    ps.push_to_print(            function_restore()                             );
    ps.push_to_print(            "  move $a0, $v0\n"                            );
    ps.push_to_print(            load_reg("$v0", "17")                          );
    ps.push_to_print(            "  syscall\t\t# call exit with errcode $v0\n"  );
}

void Codegen::generate_decfunc(Node *n){
    // create the function before the main function
    ps.clear_to_insert();
    ps.push_to_insert( ps.getMain() );
    // name of the function
    func_name = "_f_" + (*n)[0]->getType();
    // create the function context
    sym_map[func_name] = MemContext();
    // generate the asm
    ps.push_to_print(  func_name + ":\n"  );
    ps.push_to_print(  function_return() , 0 );
}

void Codegen::generate_decvar(Node *n){
    // get var name
    Node *n_id = *n->begin();
    // add var to the symbol table
    sym_map[func_name].add(n_id);
    // add a endstatement to the decvar
    n->add(T_SYM, AST_ENDSTATEMENT, n->getLine());
    // add variable to the stack / global place
    if (n->getParent()->getType() == AST_PROGRAM){
        // set the global variables initialization point
        ps.clear_to_insert();
        ps.push_to_insert( ps.getGlobalVar() );
        // get the position of the variable in global stack
        int pos = sym_map[func_name].get(n_id);
        // initialize global var
        ps.push_to_print( load_reg("$a1", "0") );
        ps.push_to_insert( ps.push_to_print( save_reg_to_t0("$a1", to_string(pos))  ) );
    } else {
        // we know were inside a function, so we must save
        // the var in the local stack
        ps.push_to_print( load_reg("$a1", "0")  );
        ps.push_to_insert( ps.push_to_print( save_reg("$a1") ) );
    }
    // TODO: initialize the variable
}

void Codegen::generate_assign(Node *n){
    // TODO
}

void Codegen::generate_funccall(Node *n){
    Node* arglist = (*n)[1];
    list<string>::iterator f_call;
    // get the destination register for n
    string dest_reg = get_dest_reg(n);
    string f_call_name = string("_f_") + (*n)[0]->getType();
    // do function stuff
    ps.push_to_print(  copy_reg(dest_reg, "$v0"), 0              );
    ps.push_to_print(  function_restore(), 0                     );
    f_call = ps.push_to_print(  function_call( f_call_name ), 0  );
    ps.push_to_print(  function_save(), 0                        );
    // fix position of the insert stack to allow variables
    // to know where to go inside the funccall
    for (int i = 0; i < arglist->size_children(); ++i){
        ps.push_to_insert( f_call );
    }
}

void Codegen::generate_arglist(Node *n){
    // this forces the arguments of the function call to return
    // to the next insert position that they should fit in
    for (list<Node*>::reverse_iterator it = n->rbegin(); it != n->rend(); ++it){
        Node *arg = *it;
        arg->add(T_SYM, AST_ENDSTATEMENT, n->getLine() );
        arg->add(T_SYM, AST_ENDARG, n->getLine() );
        arg->add(T_SYM, AST_ENDSTATEMENT, n->getLine() );
    }
}

void Codegen::generate_paramlist(Node *n){
    // add all variables defined inside paramlist to the symbol map
    for (list<Node*>::iterator it = n->begin(); it != n->end(); ++it){
        sym_map[func_name].add(*it);
    }
}

void Codegen::generate_block(Node *n){
    // mark the end of statements
    std::list<Node*>::iterator it = n->begin();
    for (++it; it != n->end(); it++){
        // mark the end of a statment
        n->insert(it, new Node(T_SYM, AST_ENDSTATEMENT, n->getLine() ));
    }
    n->add(T_SYM, AST_ENDSTATEMENT, n->getLine() );
    // mark the end of the block with a end_block node
    n->add( T_SYM, AST_ENDBLOCK, n->getLine() );
}

void Codegen::generate_return(Node *n){
    // do return stuff
    ps.push_to_print( jump(func_name + "_ret") , 0 );
    ps.push_to_print( copy_reg("$v0", "$a0")   , 0 );
}

void Codegen::generate_if(Node *n){
    // TODO
}

void Codegen::generate_while(Node *n){
    // TODO
}

void Codegen::generate_break(Node *n){
    // TODO
}

void Codegen::generate_continue(Node *n){
    // TODO
}

// PSEUDO NODES

void Codegen::generate_block_end(Node *n){
    // if our parent is a decfunction, then swtich the
    // func_name to program
    Node *block_parent = n->getParent()->getParent();
    if (block_parent->getType() == AST_DECFUNC){
        func_name = CG_FNAME_PROGRAM;
    }
    // TODO: detect end if, end else, end while
}

void Codegen::generate_end_statement(Node *n){
    // set the next place to insert text
    ps.pop_to_insert();
}

void Codegen::generate_end_arg(Node *n){
    // we need to save the generated funccall arg into the stack
    // (to do so, we need to know which reg to save)
    Node *arg = n->getParent();
    Node *arglist = arg->getParent();

    // // TODO remove debug
    // cout << *arglist << "\n";
    // cout << *arg << "\n\n";

    string dest_reg = "$a1";
    if (arglist->find(arg) == 0){
        // in this case, we know reg must be $a0 by definition, otherwise
        // its $a1
        dest_reg = "$a0";
    }
    ps.push_to_print( save_reg(dest_reg), 0 );
}

// nodes not specified above are being treated below

void Codegen::generate_sym(Node *n){
    // get the destination register for n
    Node *parent = n->getParent();
    string dest_reg = get_dest_reg(n);
    // // if we are in one of the following cases, we should only use register
    // // 0 ($a0)
    // if (parent->getType() == AST_DECVAR){
    //     dest_reg = "$a0";
    // }
    if (n->getType() == "+"){
        ps.push_to_print( add(dest_reg, "$a0", "$a1") , 0 );
    }
}

void Codegen::generate_id(Node *n){
    // get the destination register for n
    Node *parent = n->getParent();
    string dest_reg = get_dest_reg(n);
    // test if we are trying to use a variable (not function, not declaration, not anything else!)
    if (parent->getType() != AST_DECFUNC && parent->getType() != AST_PARAMLIST &&
        parent->getType() != AST_FUNCCALL){
        // we are in a DECVAR, ARGLIST or a general EXPRESSION
        int pos = parent->find(n);
        if (parent->getType() != AST_DECVAR || (parent->getType() == AST_DECVAR && pos == 1)){
            // we know were trying to get a variable from the stack for a VARIABLE INITIALIZATION,
            // ARGLIST of a funccall, or expression
            try {
                int stack_pos = sym_map[func_name].get(n);
                // if we reach here, we have a local variable
                ps.push_to_print( load_reg_from_fp(dest_reg, to_string(stack_pos)), 0 );
            } catch(Error& e) {
                int stack_pos = sym_map[CG_FNAME_PROGRAM].get(n);
                // if we reach here, we have a global variable
                ps.push_to_print( load_reg_from_t0(dest_reg, to_string(stack_pos)), 0 );
            }
        }
    }
}

void Codegen::generate_dec(Node *n){
    // get the destination register for n
    string dest_reg = get_dest_reg(n);
    ps.push_to_print( load_reg(dest_reg, n->getType()), 0 );
}


// generate assembly code based on node n
void Codegen::generate(Node *n){
    // detect reserved words and structures of the language
    if (n->getType() == AST_DECFUNC){
        generate_decfunc(n);
    } else if (n->getType() == AST_DECVAR) {
        generate_decvar(n);
    } else if (n->getType() == AST_ASSIGN) {
        generate_assign(n);
    } else if (n->getType() == AST_FUNCCALL){
        generate_funccall(n);
    } else if (n->getType() == AST_ARGLIST){
        generate_arglist(n);
    } else if (n->getType() == AST_PARAMLIST){
        generate_paramlist(n);
    } else if (n->getType() == AST_BLOCK){
        generate_block(n);
    } else if (n->getType() == AST_RETURN){
        generate_return(n);
    } else if (n->getType() == AST_IF){
        generate_if(n);
    } else if (n->getType() == AST_WHILE){
        generate_while(n);
    } else if (n->getType() == AST_BREAK){
        generate_break(n);
    } else if (n->getType() == AST_CONTINUE){
        generate_continue(n);
    } else if (n->getType() == AST_PROGRAM){
        generate_program(n);
    // pseudo nodes used only by codegenerator only for .asm file creation
    } else if (n->getType() == AST_ENDBLOCK){
        generate_block_end(n);
    } else if (n->getType() == AST_ENDSTATEMENT){
        generate_end_statement(n);
    } else if (n->getType() == AST_ENDARG){
        generate_end_arg(n);
    // detect tokens not already specified above
    } else if (n->getToken() == T_SYM){
        generate_sym(n);
    } else if (n->getToken() == T_ID){
        generate_id(n);
    } else if (n->getToken() == T_DEC){
        generate_dec(n);
    }
}

// PUBLIC FUNCTIONS

// run codegenerator
void Codegen::run(){
    // build all stack based on the operation order
    list<Node*>::iterator act = operate.begin();
    list<Node*>::iterator ins = operate.end();
    while(act != ins) {
        Node *n = *act;
        // // TODO: remove debug stat below
        // cout << *n << "\n\n";

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
    cout << ps << endl;
    // TODO: remove debug statements below
    // cout << "\n\n";
    // for (std::map<std::string, MemContext>::iterator it = sym_map.begin(); it != sym_map.end(); ++it){
    //     cout << "--------------   Memory Context: " << it->first << "   -----------------\n\n" << it->second << "\n";
    // }
}
