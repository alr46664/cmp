
#include "error.h" // error class
#include "parser.h" // parser class

using namespace std;

bool Parser::isExpr(){
    if (operate.empty())
        throw Error("Could not process \"" + val + "\". We could not check if we are in an expression field because the Stack is EMPTY.", token, line, ERR_UNK);
    // pegue o tipo do no imediatamente superior
    string top_t = operate.top()->getType();
    return (top_t == AST_RETURN || top_t == AST_ASSIGN || top_t == AST_PAREN || top_t == AST_ARGLIST);
}

void Parser::parse_unary_op(){
    operate.add(T_SYM, val, line);
}

void Parser::parse_binary_op(){
    // iremos entrar pelo menos 1x no while
    Node *right = operate.pop();
    while (!isExpr()){
        // verificacao de erro
        if (operate.empty())
            throw Error("Could not process binary operator \"" + val + "\". A binary operator must be used inside a valid expression field and needs 2 operands. For some reason, there was an error (Stack is EMPTY). You most likely is not using the operator inside an expression.", token, line, ERR_UNK);
        string prev_op_type = operate.top()->getType();
        // se existir precedencia E a precedencia de val > precedencia de op,
        // entao adicionar novo nó (val) a direita
        if (precedence.find(prev_op_type) != precedence.end() && precedence[val] > precedence[prev_op_type]){
            break;
        }
        right = operate.pop();
    }
    // if de protecao (right nunca deve ser NULL, mas nao custa
    // se precaver :D )
    if (right == NULL)
        throw Error("Could not process binary operator \"" + val + "\". A binary operator must be used inside a valid expression field and needs 2 operands. For some reason, there was an error (Right operator is EMPTY).", token, line, ERR_UNK);
    operate.push(right);
    // adicionar novo no val
    operate.add_swap(new Node(T_SYM, val, line));
}

void Parser::parse_id(){
    operate.add(T_ID, val, line);
}

void Parser::parse_key(){
    // pegue o no imediatamente superior
    Node *top = operate.top();

    if (val == "def"){
        operate.add(T_KEY, AST_DECFUNC, line);
    } else if (val == "var" || val == "let"){
        operate.add(T_KEY, AST_DECVAR, line);
    } else if (val == "if"){
        operate.add(T_KEY, AST_IF, line);
    } else if (val == "else"){
        // retorne o ultimo if para o stack
        operate.push(top->top());
    } else if (val == "while"){
        operate.add(T_KEY, AST_WHILE, line);
    } else if (val == "return"){
        operate.add(T_KEY, AST_RETURN, line);
    } else if (val == "break"){
        operate.add(T_KEY, AST_BREAK, line);
    } else if (val == "continue"){
        operate.add(T_KEY, AST_CONTINUE, line);
    } else
        throw Error(string("KEY \"") + val + "\" not identified.", token, line, ERR_KEY);
}

void Parser::parse_dec(){
    // adicione dec
    operate.add(T_DEC, val, line);
}

void Parser::parse_sym(){
    if (val == "("){
        Node *top_id = operate.top();
        // verificacao se arglist ou paramlist
        if (top_id && top_id->getToken() == T_ID){
            // sabemos que estamos numa arglist ou paramlist
            operate.pop();
            // verificacao de erro
            if (operate.empty())
                throw Error("Could not process symbol \"" + val + "\". You are most likely trying to either declare or execute a function call, but some error happened while parsing it. Check if your syntax is right!", token, line, ERR_SYM);
            Node *prev_top_id = operate.top();
            if (prev_top_id->getType() == AST_DECFUNC){
                // sabemos que estamos numa declaracao de funcao (decfunc)
                // logo devemos adcionar o no paramlist
                operate.add(T_SYM, AST_PARAMLIST, line);
            } else {
                // sabemos que caso nao seja uma declaracao  de variavel,
                // temos uma chamada de funcao
                operate.push(top_id);
                operate.add_swap(new Node(T_KEY, AST_FUNCCALL, line));
                operate.add(T_SYM, AST_ARGLIST, line);
            }
        } else {
            // sabemos que estamos tratando de uma expressao
            operate.add(T_SYM, AST_PAREN, line);
        }
    } else if (val == ")"){
        // verificacao de erro
        if (operate.empty())
            throw Error("Could not process symbol \"" + val + "\". Each parenteses need to be matched in pairs (opening and closing). Check if your syntax is right!", token, line, ERR_SYM);
        // retorne para o no que ativou o parentese
        while(operate.top()->getType() != AST_PAREN && operate.top()->getType() != AST_ARGLIST && operate.top()->getType() != AST_DECFUNC){
            operate.pop();
            // verificacao de erro
            if (operate.empty())
                throw Error("Could not process symbol \"" + val + "\". Each parenteses need to be matched in pairs (opening and closing). Check if your syntax is right!", token, line, ERR_SYM);
        }
        // verifique se estamos em um arglist, se sim
        // retorne o stack para a function call
        if (operate.top()->getType() == AST_ARGLIST){
            while(operate.top()->getType() != AST_FUNCCALL) {
                operate.pop();
                // verificacao de erro
                if (operate.empty())
                    throw Error("Could not process symbol \"" + val + "\". You tried to perform a function call, but your syntax is incorrect.", token, line, ERR_SYM);
            }
        } else if (operate.top()->getType() == AST_PAREN) {
            // verifique se o paren pertence a um no de operador unario, if ou while,
            // se ele nao pertencer, mantenha paren no stack,
            // do contrario, devemos retornar para o no pai de paren
            Node *paren = operate.pop();
            // verificacao de erro
            if (paren == NULL || operate.empty())
                throw Error("Could not process symbol \"" + val + "\". Each parenteses need to be matched in pairs (opening and closing). Check if your syntax is right!", token, line, ERR_SYM);
            if (operate.top()->getType() != AST_IF && operate.top()->getType() != AST_WHILE && precedence_un.find(operate.top()->getType()) == precedence_un.end()){
                operate.push(paren);
            }
        }
    } else if (val == "{"){
        operate.add(T_SYM, AST_BLOCK, line);
    } else if (val == "}"){
        // verificacao de erro
        if (operate.empty())
            throw Error("Could not process symbol \"" + val + "\". Each block needs an open and close curly braces {}.", token, line, ERR_SYM);
        // retorne para fora do escopo do bloco
        while (operate.top()->getType() != AST_BLOCK) {
            operate.pop();
            // verificacao de erro
            if (operate.empty())
                throw Error("Could not process symbol \"" + val + "\". Each block needs an open and close curly braces {}.", token, line, ERR_SYM);
        }
        operate.pop();
        // verificacao de erro
        if (operate.empty())
            throw Error("Could not process symbol \"" + val + "\". Each block needs an open and close curly braces {}.", token, line, ERR_SYM);
        // saia da declaracao da funcao, while
        Node *n = operate.top();
        if (n->getType() == AST_DECFUNC || n->getType() == AST_WHILE || n->getType() == AST_IF)
            operate.pop();
    } else if (val == ","){
        // verificacao de erro
        if (operate.empty())
            throw Error("Could not process symbol \"" + val + "\". Commas can only be used in certain scenarios, please check your code syntax.", token, line, ERR_SYM);
        operate.pop();
    } else if (val == ";"){
        // verificacao de erro
        if (operate.empty())
            throw Error("Could not process symbol \"" + val + "\". Check your code syntax.", token, line, ERR_SYM);
        // retorne para o escope do bloco
        while (operate.top()->getType() != AST_BLOCK && operate.top()->getType() != AST_PROGRAM){
            operate.pop();
            // verificacao de erro
            if (operate.empty())
                throw Error("Could not process symbol \"" + val + "\". Check your code syntax.", token, line, ERR_SYM);
        }
    } else if (val == "="){
        // verificacao de erro
        if (operate.empty())
            throw Error("Could not process symbol \"" + val + "\". The assign operator needs a variable in its right side to work correctly.", token, line, ERR_SYM);
        operate.add_swap(new Node(T_SYM, AST_ASSIGN, line));
    } else if (val == "+" || val == "-" || val == "*" || val == "/" || val == "<" || val == ">" || val == "<=" || val == ">=" || val == "==" || val == "!=" || val == "&&" || val == "||"){
        // verificacao de erro
        if (operate.empty())
            throw Error("Could not process symbol \"" + val + "\". Binary operators require 2 operands.", token, line, ERR_SYM);
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
            operate.add(T_SYM, val, line);
        } else {
            // temos um operador binario com todos os operandos ja add,
            // logo devemos adicionar o ultimo operando desse operador para
            // que possamos tratar a precedencia na funcao abaixo
            operate.push((*top)[1]);
            parse_binary_op();
        }
    } else if (val == "!"){
        parse_unary_op();
    } else
        throw Error(string("SYM \"") + val + "\" not identified.", token, line, ERR_SYM);
}

void Parser::parse(string t, string v, int l){
    // define the member variables
    token = t;
    val = v;
    line = l;
    // // debugging print
    // cout << "\n\tPROGRAM (t: \"" << t << "\" - v: \"" << v << "\" - l: " << l <<
        // " - STACK: " << *operate.top()  << "): \n\n" << *getProgramAST() << "\n\n";
    // check for the token type
    if (token == T_ID) {
        parse_id();
    } else if (token == T_KEY) {
        parse_key();
    } else if (token == T_DEC) {
        parse_dec();
    } else if (token == T_SYM) {
        parse_sym();
    } else
        throw Error(string("Unidentified token \"") + val + "\" could not be parsed.", token, line, ERR_TOKEN_UNDEF);
}

Node* Parser::getProgramAST(){
    return program;
}

// reduce memory usage of the c++ compiler
void Parser::clear(){
    token = val = "";
    operate.clear();
}
