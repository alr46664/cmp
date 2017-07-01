#ifndef UTILITY_H
#define UTILITY_H

// C
#include <cstdio>

// C++
#include <string>
#include <initializer_list>
#include <exception>

// defines of the AST types
#include "ast_types.h"

#include "error.h" // Error class

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

    // get filename (no ext)
    static std::string getFilename(std::string s){
        size_t found = s.rfind(".");
        if (found != std::string::npos){
            return s.substr(0, found);
        }
        throw Error("Not a file", s, 0, ERR_UNK);
    }

    // get file extension
    static std::string getExtension(std::string s){
        size_t found = s.rfind(".");
        if (found != std::string::npos){
            return s.substr(found);
        }
        throw Error("Not a file", s, 0, ERR_UNK);
    }
    // redirect input and output
    static void redirect_in_out(std::string in, std::string out, int (*f)(void) ){
        try {
            freopen(in.c_str(), "r", stdin);
            freopen(out.c_str(), "w", stdout);
            (*f)();
            fclose(stdin);
            fclose(stdout);
        } catch (Error& e){
            fclose(stdin);
            fclose(stdout);
            throw e;
        } catch (std::exception& e) {
            fclose(stdin);
            fclose(stdout);
            throw e;
        }
    }
};


#endif // UTILITY_H
