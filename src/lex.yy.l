
digits [0-9]
letter [a-zA-Z]
alphanum [a-zA-Z0-9]
symbols [\(\{\}\)\,\;\=\+\-\*\/\<\>]
line [\n]
non_printable [\t ]
comments ("//".*)
invalid [^a-zA-Z0-9\n\t _\(\{\}\)\,\;\=\+\-\*\/\<\>\&\|\!]

%{
    #include <iostream>
    #include <string>

    #include "classes/error.h"

    int lex_lines = 1;
%}
%%
{line} {
    /* count and ignore lines */
    lex_lines++;}
({non_printable}+|{comments}) {/* ignore nonprintable chars and comments */}
(var|let|def|if|else|while|return|break|continue) {
    std::cout << "KEY \"" << yytext << "\" " << std::to_string(lex_lines) << std::endl;}
{letter}(_|{alphanum})* {
    std::cout << "ID \"" << yytext << "\" " << std::to_string(lex_lines) << std::endl;}
{digits}+ {
    std::cout << "DEC \"" << yytext << "\" " << std::to_string(lex_lines) << std::endl;}
({symbols}|"<="|">="|"=="|"!="|"&&"|"||"|"!") {
    std::cout << "SYM \"" << yytext << "\" " << std::to_string(lex_lines) << std::endl;}
(.) {
    std::cout << "ERROR \"" << yytext << "\" " << std::to_string(lex_lines) << std::endl;
    throw Error(std::string("Invalid character \"") + yytext + "\" found.\n\n", "ERROR", lex_lines, ERR_LEX);
    }
%%
