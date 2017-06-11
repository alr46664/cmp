
digits [0-9]
letter [a-zA-Z]
alphanum [a-zA-Z0-9]
non_printable [\n\t ]

%{

%}
%%
{non_printable} {printf("");}
(var|function|if|else|while|return|break|continue) {printf("KEY \"%s\"\n", yytext);}
{letter}(_|{alphanum})* {printf("ID \"%s\"\n", yytext);}
{digits}+ {printf("DEC \"%s\"\n", yytext);}
([\(\{\[\]\}\)\,\;\=\+\-\*\/\<\>]|"<="|">="|"=="|"!="|"&&"|"||"|"!") {printf("SYM \"%s\"\n", yytext);}
%%
//
#define MIN_ARGC 2
int main(int argc, char **argv) {
    // esta funcao ira ler o stdin e printar no stdout
    // o resultado da tokenizacao (scaneamento) feito pelo
    // analisador lexico descrito acima
    yylex();
    return 0;
}
