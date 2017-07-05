
#include "codegen.h"

using namespace std;

void Codegen::print_stack(){
    for (list<Node*>::iterator it = operate.begin(); it != operate.end(); it++){
        cout << *(*it) << "\n\n";
    }
}

// generate assembly code based on node n
void Codegen::generate(Node *n){
    cout << *n << "\n\n";
}

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
    // print all nodes from the operate stack
    // print_stack();
}
