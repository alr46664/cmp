
// custom includes
#include "main.h" // defines

using namespace std;

// create the single parser that the whole program will use
Parser p;
// create the sintatical analiser
Sintatical sin(p.getProgramAST());

// parse the token stream into an AST
void run_ast_parser(){
    // create default functions
    // create print default function
    p.parse(T_KEY, "def", 0);
    p.parse(T_ID, "print", 0);
    p.parse(T_SYM, "(", 0);
    p.parse(T_ID, "a", 0);
    p.parse(T_SYM, ")", 0);
    p.parse(T_SYM, "{", 0);
    p.parse(T_SYM, "}", 0);

    // store the whole stdin / cin line
    int line;
    string token, val, input;
    // read until EOF
    while(getline(cin, input)){
        size_t space1 = input.find_first_of(' ');
        size_t space2 = input.find_last_of(' ');
        if (space1 != string::npos && space2 != string::npos){
            token = input.substr(0, space1);
            val = input.substr(space1+2, space2-space1-3);
            line = atoi(input.substr(space2, string::npos).c_str());
        }
        // process token and val
        p.parse(token, val, line);
    }
    // release unneeded memory from parser
    p.clear();
}

// rode analisador sintatico semantico
int run_semantical_sintatical(){
    // parse the token stream into an AST
    run_ast_parser();

    // run the sintatical / semantical analiser
    sin.run();

    // clear default functions / variables so that they
    // dont display in the final AST
    Node* program = p.getProgramAST();
    // remove print function from the AST
    Node* print = *program->begin();
    program->remove(print);
    delete print;

    // print the program AST tree
    cout << *p.getProgramAST() << endl;
    return 0;
}

// rode gerador de codigo
int run_codegen(){
    // TODO
}

int main(int argc, char **argv) {
    try {
        if (argc != 3 ){
            cerr << "ERROR: Needs 2 arguments, \"in_file\" and \"out_file\"\n\n";
            return ERR_UNK;
        }
        string in_file = string(argv[1]);
        string out_file = string(argv[2]);

        // // rode analisador lexico
        // Utility::redirect_in_out(in_file, out_file, yylex);

        // rode analsador sintatico semantico
        Utility::redirect_in_out(in_file, out_file + ".temp", yylex);
        Utility::redirect_in_out(out_file + ".temp", out_file, run_semantical_sintatical);

        // // rode gerador de codigo
        // Utility::redirect_in_out(in_file, out_file + ".temp", yylex);
        // Utility::redirect_in_out(out_file + ".temp", out_file + ".temp2", run_semantical_sintatical);
        // Utility::redirect_in_out(out_file + ".temp2", out_file, run_codegen);

        return 0;
    } catch (Error& e) {
        cerr << e.what() << endl;
        return e.getCode();
    } catch (exception& e) {
        cerr << e.what() << endl;
        return ERR_UNK;
    }
}

