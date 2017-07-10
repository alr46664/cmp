#ifndef AST_TYPES_H
#define AST_TYPES_H

#define T_PROGRAM "PROGRAM"
#define T_ID "ID"
#define T_KEY "KEY"
#define T_DEC "DEC"
#define T_SYM "SYM"
#define T_ERR "ERROR"

#define AST_PROGRAM "program"
#define AST_DECFUNC "decfunc"
#define AST_DECVAR "decvar"
#define AST_ASSIGN "assign"
#define AST_FUNCCALL "funccall"
#define AST_ARGLIST "arglist"
#define AST_PARAMLIST "paramlist"
#define AST_PAREN "paren"
#define AST_BLOCK "block"
#define AST_RETURN "return"
#define AST_IF "if"
#define AST_ELSE "else"
#define AST_WHILE "while"
#define AST_BREAK "break"
#define AST_CONTINUE "continue"

// nodes that are only used by the codegenerator to create the .asm file
#define AST_ENDBLOCK "end_block"
#define AST_ENDSTATEMENT "end_statement"
#define AST_ENDARG "end_arg"

#endif // AST_TYPES_H
