
digits [0-9]
letter [a-zA-Z]
alphanum [a-zA-Z0-9]
symbols [\(\{\[\]\}\)\,\;\=\+\-\*\/\<\>]
line [\n]
non_printable [\t ]
comments ("//".*)
invalid [^a-zA-Z0-9\n\t _\(\{\[\]\}\)\,\;\=\+\-\*\/\<\>\&\|\!]

%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    int lines = 1;
    int pos = 0;
%}
%%
{invalid} {
    printf("ERROR \"%s\" %d\n", yytext, lines);
    fprintf(stderr, "ERROR: (Line %d) Invalid character \"%s\" found.\n\n", lines, yytext);
    exit(1);}
{line} {
    /* count and ignore lines */
    pos = 0;
    lines++;}
({non_printable}+|{comments}) {/* ignore nonprintable chars and comments */}
(var|let|def|if|else|while|return|break|continue) {
    printf("KEY \"%s\" %d\n", yytext, lines);}
{letter}(_|{alphanum})* {
    printf("ID \"%s\" %d\n", yytext, lines);}
{digits}+ {
    printf("DEC \"%s\" %d\n", yytext, lines);}
({symbols}|"<="|">="|"=="|"!="|"&&"|"||"|"!") {
    printf("SYM \"%s\" %d\n", yytext, lines);}
%%
//
#define MIN_ARGC 2
int main(int argc, char **argv) {
    // esta funcao ira ler o stdin e printar no stdout
    // o resultado da tokenizacao (scaneamento) feito pelo
    // analisador lexico descrito acima
    return yylex();
}
