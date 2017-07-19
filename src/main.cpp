
// custom includes
#include "main.h" // defines

using namespace std;

// create the AST parser
Parser p;

// rode analisador sintatico semantico
int run_semantical_sintatical(){
    // create the sintatical / semantical analiser
    Sintatical sin(p.getProgramAST());

    // parse the token stream and
    // run the sintatical / semantical analiser
    p.run();
    sin.run();

    // clear default functions / variables so that they
    // dont display in the final AST
    // remove print function from the AST
    Node* program = p.getProgramAST();
    Node* print = *program->begin();
    program->remove(print);
    delete print;

    // print the program AST tree
    cout << *p.getProgramAST() << endl;
    return 0;
}

// rode gerador de codigo
int run_codegen(){
    // create the code generator
    Codegen gen(p.getProgramAST());
    // run the code generator
    gen.run();
    // terminate file with endl
    cout << endl;
    return 0;
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

        // // rode analsador sintatico semantico
        // Utility::redirect_in_out(in_file, out_file + ".lex", yylex);
        // Utility::redirect_in_out(out_file + ".lex", out_file, run_semantical_sintatical);

        // // rode gerador de codigo
        Utility::redirect_in_out(in_file, out_file + ".lex", yylex);
        Utility::redirect_in_out(out_file + ".lex", out_file + ".syn", run_semantical_sintatical);
        Utility::redirect_in_out(out_file + ".syn", out_file, run_codegen);

        return 0;
    } catch (Error& e) {
        cerr << e.what() << endl;
        return e.getCode();
    } catch (exception& e) {
        cerr << e.what() << endl;
        return ERR_UNK;
    }
}

