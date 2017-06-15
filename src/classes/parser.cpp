
#include "parser.h" // parser class

using namespace std;

Node* Parser::getProgramAST(){
    return program;
}

void Parser::parse_id(){
    // TODO: TO REMOVE (debug)
    cout << "ID found" << endl;
    // crie variavel erro para caso um error ocorra
    Error e(string("ID \"") + val + "\" can only be used in declarations of variables and functions, as well as in assigns, expressions and function calls.", line, ERR_ID);
    // pegue o no imediatamente superior
    Node *top = operate.top();

    if (!operate.empty()){
        // operate.add(val, {AST_DECFUNC, AST_DECVAR, AST_RETURN}, e);
        operate.add(val);
    } else {
        operate.add(val);
    }
}

void Parser::parse_key(){
    // crie variavel erro para caso um error ocorra
    Error e(string("KEY \"") + val + "\" was not recognized. Valid values are var,let,def,if,else,while,return,break,continue.", line, ERR_KEY);
    // pegue o no imediatamente superior
    Node *top = operate.top();

    if (val == "def"){
        Error err_def(string("KEY \"") + val + "\" was can only be used within the \"program\" scope (root of the tree). In other word, you can't define a function inside a function.", line, ERR_KEY);
        operate.add(AST_DECFUNC, {AST_PROGRAM}, err_def);
        // operate.add(AST_DECFUNC);
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
    // adicione dec como child do top da arvore e nao atualize o stack
    operate.top()->add(val);
}

void Parser::parse_sym(){
    // crie variavel erro para caso um error ocorra
    Error e(string("SYM \"") + val + "\" was not recognized. Valid values are ( [ { } ] ) , ; = + - * / < > <= >= == != && || !", line, ERR_SYM);
    // pegue o no imediatamente superior
    Node *top = operate.top();

    if (val == "("){
        // verificacao se arglist ou paramlist
        operate.pop();
        Node *new_top = operate.top();
        cout << *new_top << endl;
        if (new_top->getType() == AST_DECFUNC){
            operate.add(AST_PARAMLIST);
        } else if (new_top->getType() == AST_FUNCCALL) {
            operate.add(AST_ARGLIST);
        } else {
            Error e1(string("SYM \"") + val + "\" needs a function declaration \"def\" or function call to be associated with.", line, ERR_SYM);
            e1.print();
        }
    } else if (val == ")"){
        // retorne para a id da funccall ou deffunc
        operate.pop();
        operate.pop();
    } else if (val == "["){
    } else if (val == "]"){
    } else if (val == "{"){
        operate.add(AST_BLOCK);
    } else if (val == "}"){
        // retorne para fora do escope do bloco
        while (operate.top()->getType() != AST_BLOCK) operate.pop();
        // saia da declaracao da funcao se estivermos dentro dela
        if (operate.pop()->getType() == AST_DECFUNC) operate.pop();
    } else if (val == ","){
        operate.pop();
    } else if (val == ";"){
        // retorne para o escope do bloco
        while (operate.top()->getType() != AST_BLOCK) operate.pop();
        // operate.pop();
    } else if (val == "="){
    } else if (val == "+"){
        // retorne ao no imediatamente superior
        Node *sum = new Node("+");
        operate.add_swap(sum);
    } else if (val == "-"){
    } else if (val == "*"){
    } else if (val == "/"){
    } else if (val == "<"){
    } else if (val == ">"){
    } else if (val == "<="){
    } else if (val == ">="){
    } else if (val == "=="){
    } else if (val == "!="){
    } else if (val == "&&"){
    } else if (val == "||"){
    } else if (val == "!"){
    }
}

void Parser::parse(string t, string v, int l){
    // define the member variables
    token = t;
    val = v;
    line = l;
    // TODO remove
    cout << "\n\tPROGRAM (t: \"" << t << "\" - v: \"" << v << "\" - l: " << l <<
        " - STACK: " << *operate.top()  << "): \n\n" << *this->getProgramAST() << "\n\n";
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
