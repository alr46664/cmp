#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <initializer_list>

// defines of the AST types
#include "ast_types.h"

class Utility {
public:
    static bool isUnaryOp(std::string op){
        return (op == "!" || op == "-");
    }

    static bool isBinaryOp(std::string op){
        return (op == "+" || op == "-" || op == "*" || op == "/" ||
                op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" ||
                op == "!=" || op == "&&" || op == "||");
    }

    static bool isExpr(std::string op){
        return (op == AST_RETURN || op == AST_ASSIGN || op == AST_PAREN || op == AST_ARGLIST);
    }

    static bool isExprResult(std::string op, std::string tok = T_SYM){
        return (tok == T_ID || tok == T_DEC ||
            op == AST_FUNCCALL ||  op == AST_PAREN ||
            Utility::isUnaryOp(op) || Utility::isBinaryOp(op));
    }
};


#endif // UTILITY_H
