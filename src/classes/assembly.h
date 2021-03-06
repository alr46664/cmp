#ifndef ASSEMBLY_H
#define ASSEMBLY_H

// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string

class Assembly {
private:

public:


// ----------------------- MATH OPERATIONS ----------------------------


static std::string add(std::string dest, std::string src1, std::string src2){
    return std::string("  add ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string mul(std::string dest, std::string src1, std::string src2){
    return std::string("  mul ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string div(std::string dest, std::string src1, std::string src2){
    return std::string("  div ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string sub(std::string dest, std::string src1, std::string src2){
    return std::string("  sub ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string neg(std::string dest, std::string src1){
    return std::string("  neg ") + dest+ ", " + src1 + "\n";
}


// ----------------------- COMPARISON OPERATIONS ----------------------------


static std::string lt(std::string dest, std::string src1, std::string src2){
    return std::string("  slt ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string lte(std::string dest, std::string src1, std::string src2){
    // used sge because it does return 1 in the proper cases of lte
    return std::string("  sge ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string gt(std::string dest, std::string src1, std::string src2){
    return std::string("  sgt ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string gte(std::string dest, std::string src1, std::string src2){
    // used sle because it does return 1 in the proper cases of gte
    return std::string("  sle ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string eq(std::string dest, std::string src1, std::string src2){
    return std::string("  seq ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string neq(std::string dest, std::string src1, std::string src2){
    return std::string("  sne ") + dest+ ", " + src1 + ", " + src2 + "\n";
}


// ----------------------- LOGICAL OPERATIONS ----------------------------

static std::string a_not(std::string dest, std::string src1){
    // todo: FIX improper behavior
    return Assembly::load_reg("$a1", "0") +
        Assembly::eq(dest, src1, "$a1");
    // return std::string("  not ") + dest+ ", " + src1 + "\n";
}

static std::string a_and(std::string dest, std::string src1, std::string src2){
    return std::string("  and ") + dest+ ", " + src1 + ", " + src2 + "\n";
}

static std::string a_or(std::string dest, std::string src1, std::string src2){
    return std::string("  or ") + dest+ ", " + src1 + ", " + src2 + "\n";
}


// ----------------------- BRANCH OPERATIONS ----------------------------


// jump to code
static std::string jump(std::string reg){
    return std::string("  j ") + reg + "\n";
}

// branch to code
static std::string branch(std::string label){
    return std::string("  b ") + label + "\n";
}

// branch not eq 0
static std::string bnez(std::string label, std::string src1){
    return std::string("  bnez ") + src1 + ", " + label + "\n";
}

// branch eq 0
static std::string beqz(std::string label, std::string src1){
    return std::string("  beqz ") + src1 + ", " + label + "\n";
}


// ----------------------- FUNCTION OPERATIONS ----------------------------


// save function registers into stack
static std::string function_save(){
    return Assembly::save_reg("$ra")        +
           Assembly::save_reg("$fp")        +
           Assembly::copy_reg("$t1", "$sp") ;
}

// restore function registers into stack
static std::string function_restore(){
    return Assembly::load_reg("$ra");
}

// execute call to function
static std::string function_call(std::string f){
    return std::string("  jal  ") + f + "\t\t# call function\n";
}

// return from a function call
static std::string function_return(std::string func_name){
    return Assembly::load_reg("$v0", "0") +
        "  " + func_name + "_ret:" +
        Assembly::copy_reg("$sp", "$fp") +
        Assembly::load_reg("$fp") +
        Assembly::jump("$ra") + "\n";
}


// ----------------------- MEMORY OPERATIONS ----------------------------


// move stack back
static std::string move_stack_back(){
    return std::string("  addiu $sp, $sp, 4\t\t# move stack back\n");
}

// load register with val (decimal)
static std::string load_reg(std::string reg, std::string val){
    return std::string("  li ") + reg + ", " + val + "\t\t# load fixed data\n";
}

// load register with stack data (if move_back, return stack to prev data)
static std::string load_reg(std::string reg){
    return std::string("  lw ") + reg + ", 4($sp)\t\t# load data\n" +
        Assembly::move_stack_back();
}

// load a register with a data from $fp
static std::string load_reg_from_fp(std::string reg, std::string pos){
    return std::string("  lw ") + reg + ", -" + pos + "($fp)\t\t# load data\n";
}

// load a register with a data from $fp
static std::string load_reg_from_t0(std::string reg, std::string pos){
    return std::string("  lw ") + reg + ", " + pos + "($t0)\t\t# load data\n";
}

// save reg into stack
static std::string save_reg_to_fp(std::string reg, std::string pos){
    return std::string("  sw ") + reg + ", " + pos + "($fp)\n";
}

// save reg into stack
static std::string save_reg_to_t0(std::string reg, std::string pos){
    return std::string("  sw ") + reg + ", " + pos + "($t0)\n";
}

// save reg into stack
static std::string save_reg(std::string reg){
    return std::string("  sw ") + reg + ", 0($sp)\n" +
        "  addiu $sp, $sp, -4\t\t# save reg and move stack\n";
}

// copy reg src to reg dest
static std::string copy_reg(std::string dest, std::string src){
    return std::string("  move ") + dest + ", " + src + "\n";
}

};

#endif // ASSEMBLY_H
