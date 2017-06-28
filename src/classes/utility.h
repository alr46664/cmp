#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
#include <initializer_list>

class Utility {
public:
    static bool isExpr(std::string op){
        return (op == AST_RETURN || op == AST_ASSIGN || op == AST_PAREN || op == AST_ARGLIST);
    }

    static bool isExprResult(std::string op, std::string tok = T_SYM){
        return (tok == T_ID || tok == T_DEC ||
            op == "!" || op == "+" || op == "-" || op == "*" || op == "/" ||
            op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" ||
            op == "!=" || op == "&&" || op == "||");
    }
};


#endif // UTILITY_H
