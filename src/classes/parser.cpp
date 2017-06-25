
#include "utility.h" // utility class
#include "parser.h" // parser class

using namespace std;

bool Parser::isExpr(){
    // pegue o tipo do no imediatamente superior
    string top_t = operate.top()->getType();
    return (top_t == AST_RETURN || top_t == AST_ASSIGN || top_t == AST_PAREN || top_t == AST_ARGLIST);
}

void Parser::parse_unary_op(){
    operate.add(T_SYM, val);
}

void Parser::parse_binary_op(){
    // iremos entrar pelo menos 1x no while
    Node *right = NULL;
    while (!isExpr()){
        right = operate.pop();
        string prev_op_type = operate.top()->getType();
        // se existir precedencia E a precedencia de val > precedencia de op,
        // entao adicionar novo nó (val) a direita
        if (precedence.find(prev_op_type) != precedence.end() && precedence[val] > precedence[prev_op_type]){
            break;
        }
    }
    // if de protecao (right nunca deve ser NULL, mas nao custa
    // se precaver :D )
    if (right != NULL){
        operate.push(right);
    }
    // adicionar novo no val
    operate.add_swap(new Node(T_SYM, val));
}

void Parser::parse_id(){
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
        operate.add(T_KEY, AST_DECVAR);
    } else if (val == "if"){
        operate.add(T_KEY, AST_IF);
    } else if (val == "else"){
        // retorne o ultimo if para o stack
        operate.push(top->top());
    } else if (val == "while"){
        operate.add(T_KEY, AST_WHILE);
    } else if (val == "return"){
        operate.add(T_KEY, AST_RETURN);
    } else if (val == "break"){
        operate.add(T_KEY, AST_BREAK);
    } else if (val == "continue"){
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
    Error e(string("SYM \"") + val + "\" was not recognized. Valid values are ( { } ) , ; = + - * / < > <= >= == != && || !", line, ERR_SYM);

    if (val == "("){
        Node *top_id = operate.top();
        // verificacao se arglist ou paramlist
        if (top_id->getToken() == T_ID){
            // sabemos que estamos numa arglist ou paramlist
            operate.pop();
            Node *prev_top_id = operate.top();
            if (prev_top_id->getType() == AST_DECFUNC){
                // sabemos que estamos numa declaracao de funcao (decfunc)
                // logo devemos adcionar o no paramlist
                operate.add(T_SYM, AST_PARAMLIST);
            } else {
                // sabemos que caso nao seja uma declaracao  de variavel,
                // temos uma chamada de funcao
                operate.push(top_id);
                operate.add_swap(new Node(T_KEY, AST_FUNCCALL));
                operate.add(T_SYM, AST_ARGLIST);
            }
        } else {
            // sabemos que estamos tratando de uma expressao
            operate.add(T_SYM, AST_PAREN);
        }
    } else if (val == ")"){
        // retorne para o no que ativou o parentese
        while(operate.top()->getType() != AST_PAREN && operate.top()->getType() != AST_ARGLIST && operate.top()->getType() != AST_DECFUNC){
            operate.pop();
        }
        // verifique se estamos em um arglist, se sim
        // retorne o stack para a function call
        if (operate.top()->getType() == AST_ARGLIST){
            while(operate.top()->getType() != AST_FUNCCALL) {
                operate.pop();
            }
        } else if (operate.top()->getType() == AST_PAREN) {
            // verifique se o paren pertence a um no de operador unario, if ou while,
            // se ele nao pertencer, mantenha paren no stack,
            // do contrario, devemos retornar para o no pai de paren
            Node *paren = operate.pop();
            if (operate.top()->getType() != AST_IF && operate.top()->getType() != AST_WHILE && precedence_un.find(operate.top()->getType()) == precedence_un.end()){
                operate.push(paren);
            }
        }
    } else if (val == "{"){
        operate.add(T_SYM, AST_BLOCK);
    } else if (val == "}"){
        // retorne para fora do escopo do bloco
        while (operate.top()->getType() != AST_BLOCK) operate.pop();
        operate.pop();
        // saia da declaracao da funcao, while
        Node *n = operate.top();
        if (n->getType() == AST_DECFUNC || n->getType() == AST_WHILE || n->getType() == AST_IF)
            operate.pop();
    } else if (val == ","){
        operate.pop();
    } else if (val == ";"){
        // retorne para o escope do bloco
        while (operate.top()->getType() != AST_BLOCK && operate.top()->getType() != AST_PROGRAM)
            operate.pop();
    } else if (val == "="){
        operate.add_swap(new Node(T_SYM, AST_ASSIGN));
    } else if (val == "+" || val == "-" || val == "*" || val == "/" || val == "<" || val == ">" || val == "<=" || val == ">=" || val == "==" || val == "!=" || val == "&&" || val == "||"){
        Node *top = operate.top();
        // // verifique se estamos processando operadores unarios
        if (val == "-" && isExpr()){
            // o operador val é unario pois o que o antecede é um
            // operador de expressao)
            val = "u-";
            parse_unary_op();
        } else if (precedence.find(top->getType()) == precedence.end()){
            // sabemos que o que esta no topo do stack nao é um operador
            // binario (logo podemos tratar a precedencia na funcao abaixo)
            parse_binary_op();
        } else if (top->size_children() < 2) {
            // sabemos que temos um operador binario no topo do stack
            // e sabemos que ele nao possui 2 operandos, logo adicione
            // a direita o operando
            operate.add(T_SYM, val);
        } else {
            // temos um operador binario com todos os operandos ja add,
            // logo devemos adicionar o ultimo operando desse operador para
            // que possamos tratar a precedencia na funcao abaixo
            operate.push((*top)[1]);
            parse_binary_op();
        }
    } else if (val == "!"){
        parse_unary_op();
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
        " - STACK: " << *operate.top()  << "): \n\n" << *getProgramAST() << "\n\n";
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
