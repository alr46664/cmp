
#include "utility.h" // utility class
#include "parser.h" // parser class

using namespace std;

bool Parser::isExpr(){
    // pegue o tipo do no imediatamente superior
    string top_t = operate.top()->getType();
    return (top_t != AST_RETURN && top_t != AST_ASSIGN && top_t != AST_PAREN && top_t != AST_ARGLIST);
}

void Parser::parse_id(){
    // TODO: TO REMOVE (debug)
    cout << "ID found" << endl;
    // crie variavel erro para caso um error ocorra
    Error e(string("ID \"") + val + "\" can only be used in declarations of variables and functions, as well as in assigns, expressions and function calls.", line, ERR_ID);
    // pegue o no imediatamente superior
    Node *top = operate.top();

    operate.add(T_ID, val);
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
        // retorne para fora do escopo do bloco
        while (operate.top()->getType() != AST_BLOCK) operate.pop();
        Node *n = operate.pop();
        // saia da declaracao da funcao, while ou if
        if (n->getType() == AST_DECFUNC || n->getType() == AST_WHILE || n->getType() == AST_IF)
            operate.pop();
    } else if (val == ","){
        operate.pop();
    } else if (val == ";"){
        // retorne para o escope do bloco
        while (operate.top()->getType() != AST_BLOCK && operate.top()->getType() != AST_PROGRAM)
            operate.pop();
        // operate.pop();
    } else if (val == "="){
        operate.add_swap(new Node(T_SYM, AST_ASSIGN));
    } else if (val == "+" || val == "*" || val == "/" || val == "<" || val == ">" || val == "<=" || val == ">=" || val == "==" || val == "!=" || val == "&&" || val == "||"){
        // TODO: fazer precedencia funcionar
        if (operate.top()->getToken() != T_SYM){
            Node *prev_top = operate.pop();
            // 2 casos para entrar no if:
            //      1) primeiro operador da expressao
            //      2) operador com maior precedencia
            if (operate.top()->getToken() != T_SYM || operate.top()->getType() == AST_ASSIGN || precedence[val] > precedence[operate.top()->getType()]){
                operate.push(prev_top);
            }
            operate.add_swap(new Node(T_SYM, val));
        } else if (operate.top()->size_children() < 2) {
            operate.add(T_SYM, val);
        } else {
            operate.add_swap(new Node(T_SYM, val));
        }
    } else if (val == "-"){
        operate.add_swap(new Node(T_SYM, "-"));
    } else if (val == "!"){
        operate.add(T_SYM, "!");
    } else {
        e.print();
    }
}

Node* Parser::getProgramAST(){
    return program;
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
