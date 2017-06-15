
#include "parser.h" // parser class

using namespace std;

Node& Parser::getProgramAST(){
    return program;
}

void Parser::parse_id(){
    // TODO: TO REMOVE (debug)
    cout << "ID found" << endl;
    // crie variavel erro para caso um error ocorra
    Error e(string("ID \"") + val + "\" can only be used in declarations of variables and functions, as well as in assigns, expressions and function calls.", line, ERR_ID);

    if (!operate.empty()){
        operate.add(val, {AST_DECFUNC, AST_DECVAR}, e);
    } else {
        operate.add(val);
    }
}

void Parser::parse_key(){
    // crie variavel erro para caso um error ocorra
    Error e(string("KEY \"") + val + "\" was not recognized. Valid values are var,let,def,if,else,while,return,break,continue.", line, ERR_KEY);
    if (val == "def"){
        operate.add(AST_DECFUNC);
    } else if (val == "var" || val == "let"){
        operate.add(AST_DECVAR);
    } else if (val == "if"){
        operate.add(AST_IF);
    } else if (val == "else"){
        operate.add(AST_ELSE);
    } else if (val == "while"){
        operate.add(AST_WHILE);
    } else if (val == "return"){
        operate.add(AST_RETURN);
    } else if (val == "break"){
        operate.add(AST_BREAK);
    } else if (val == "continue"){
        operate.add(AST_CONTINUE);
    } else {
        e.print();
    }
}

void Parser::parse_dec(){
    // crie variavel erro para caso um error ocorra
    Error e(string("DEC \"") + val + "\" was not recognized. Valid values are [0-9]+ (integer values only).", line, ERR_DEC);
}

void Parser::parse_sym(){
    // crie variavel erro para caso um error ocorra
    Error e(string("SYM \"") + val + "\" was not recognized. Valid values are ( [ { } ] ) , ; = + - * / < > <= >= == != && || !", line, ERR_SYM);
}

void Parser::parse(string t, string v, int l){
    // define the member variables
    token = t;
    val = v;
    line = l;
    // check for the token type
    if (token == "ID") {
        parse_id();
    } else if (token == "KEY") {
        parse_key();
    } else if (token == "DEC") {
        parse_dec();
    } else if (token == "SYM") {
        parse_sym();
    } else{
        Error e(string("Token \"") + token + "\" not recognized", line, ERR_TOKEN_UNDEF);
        e.print();
    }
}
