
#include "utility.h" // utility class
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

    // FACA AS VERIFICACOES ANTES DE ADICIONAR A ID
    operate.add(T_ID, val, {T_KEY, T_SYM}, {AST_DECFUNC, AST_DECVAR, AST_PARAMLIST, AST_ARGLIST, AST_RETURN, AST_ASSIGN, AST_PAREN, AST_SUM, AST_SUB, AST_MUL, AST_DIV, AST_LT, AST_GT, AST_GTE, AST_LTE, AST_EQ, AST_NEQ, AST_AND, AST_OR, AST_NOT}, e);
}

void Parser::parse_key(){
    // crie variavel erro para caso um error ocorra
    Error e(string("KEY \"") + val + "\" was not recognized. Valid values are var,let,def,if,else,while,return,break,continue.", line, ERR_KEY);
    // pegue o no imediatamente superior
    Node *top = operate.top();

    if (val == "def"){
        Error err_def(string("KEY \"") + val + "\" can only be used within the \"program\" scope (root of the tree). In other word, you can't define a function inside a function.", line, ERR_KEY);
        operate.add(T_KEY, AST_DECFUNC, {}, {AST_PROGRAM}, err_def);
        // operate.add(AST_DECFUNC);
    } else if (val == "var" || val == "let"){
        Error err_var(string("KEY \"") + val + "\" can only be used within the \"block\" scope (root of the tree). In other word, you must define a variable inside a block (of a function, while loop, if statement) or as a global variable (outside all functions).", line, ERR_KEY);
        operate.add(T_KEY, AST_DECVAR, {T_PROGRAM, T_KEY}, {AST_PROGRAM, AST_BLOCK}, err_var);
    } else if (val == "if"){
        // TODO restrict usage
        operate.add(T_KEY, AST_IF);
    } else if (val == "else"){
        // TODO restrict usage
        operate.add(T_KEY, AST_ELSE);
    } else if (val == "while"){
        // TODO restrict usage
        operate.add(T_KEY, AST_WHILE);
    } else if (val == "return"){
        // TODO restrict usage
        operate.add(T_KEY, AST_RETURN);
    } else if (val == "break"){
        // TODO restrict usage
        operate.add(T_KEY, AST_BREAK);
    } else if (val == "continue"){
        // TODO restrict usage
        operate.add(T_KEY, AST_CONTINUE);
    } else {
        e.print();
    }
}

void Parser::parse_dec(){
    // crie variavel erro para caso um error ocorra
    Error e(string("DEC \"") + val + "\" was not recognized. Valid values are [0-9]+ (integer values only).", line, ERR_DEC);
    // adicione dec
    operate.add(T_DEC, val);
}

void Parser::parse_sym(){
    // crie variavel erro para caso um error ocorra
    Error e(string("SYM \"") + val + "\" was not recognized. Valid values are ( [ { } ] ) , ; = + - * / < > <= >= == != && || !", line, ERR_SYM);
    // pegue o no imediatamente superior
    Node *top = operate.top();

    if (val == "("){
        Node *top = operate.top();
        // verificacao se arglist ou paramlist
        if (Utility::check(top->getToken(), {T_ID})){
            this->state = P_PARENLIST;
            // sabemos que estamos numa arglist ou paramlist
            operate.pop();
            Node *new_top = operate.top();
            cout << *new_top << endl;
            if (new_top->getType() == AST_DECFUNC){
                operate.add(T_SYM, AST_PARAMLIST);
            } else if (new_top->getType() == AST_FUNCCALL) {
                operate.add(T_SYM, AST_ARGLIST);
            } else {
                Error e1(string("SYM \"") + val + "\" needs a function declaration \"def\" or function call to be associated with.", line, ERR_SYM);
                e1.print();
            }
        } else {
            // sabemos que estamos tratando de uma expressao
            operate.add(T_SYM, AST_PAREN);
        }
    } else if (val == ")"){
        if (state == P_PARENLIST){
            // retorne para a id da funccall ou deffunc
            state = P_INIT;
            operate.pop();
            operate.pop();
        } else {
            // retorne para o no imediatamente superior ao ast_paren
            while(operate.top()->getType() != AST_PAREN) operate.pop();
            operate.pop();
        }
    } else if (val == "["){
    } else if (val == "]"){
    } else if (val == "{"){
        operate.add(T_SYM, AST_BLOCK);
    } else if (val == "}"){
        // retorne para fora do escope do bloco
        while (operate.top()->getType() != AST_BLOCK) operate.pop();
        Node *n = operate.pop();
        // saia da declaracao da funcao ou while
        if (n->getType() == AST_DECFUNC || n->getType() == AST_WHILE)
            operate.pop();
    } else if (val == ","){
        operate.pop();
    } else if (val == ";"){
        // retorne para o escope do bloco
        while (operate.top()->getType() != AST_BLOCK) operate.pop();
        // operate.pop();
    } else if (val == "="){
    } else if (val == "+"){
        // retorne ao no imediatamente superior
        operate.add_swap(new Node(T_SYM, AST_SUM));
    } else if (val == "-"){
        operate.add_swap(new Node(T_SYM, AST_SUB));
    } else if (val == "*"){
        operate.add_swap(new Node(T_SYM, AST_MUL));
    } else if (val == "/"){
        operate.add_swap(new Node(T_SYM, AST_DIV));
    } else if (val == "<"){
        operate.add_swap(new Node(T_SYM, AST_LT));
    } else if (val == ">"){
        operate.add_swap(new Node(T_SYM, AST_GT));
    } else if (val == "<="){
        operate.add_swap(new Node(T_SYM, AST_LTE));
    } else if (val == ">="){
        operate.add_swap(new Node(T_SYM, AST_GTE));
    } else if (val == "=="){
        operate.add_swap(new Node(T_SYM, AST_EQ));
    } else if (val == "!="){
        operate.add_swap(new Node(T_SYM, AST_NEQ));
    } else if (val == "&&"){
        operate.add_swap(new Node(T_SYM, AST_AND));
    } else if (val == "||"){
        operate.add_swap(new Node(T_SYM, AST_OR));
    } else if (val == "!"){
        operate.add_swap(new Node(T_SYM, AST_NOT));
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
    if (token == T_ID) {
        parse_id();
    } else if (token == T_KEY) {
        parse_key();
    } else if (token == T_DEC) {
        parse_dec();
    } else if (token == T_SYM) {
        parse_sym();
    } else{
        Error e(string("Token \"") + token + "\" not recognized", line, ERR_TOKEN_UNDEF);
        e.print();
    }
}
