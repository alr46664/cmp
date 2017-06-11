
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
([\(\{\[\]\}\)\,\;\=\+\-\*\/\<\>]) {printf("SYM \"%s\"\n", yytext);}
%%
// |"<="|">="|"=="|"!="|"&&"|"||"|"!"
#define MIN_ARGC 3
int main(int argc, char **argv) {
    // check number of arguments
    if (argc < MIN_ARGC){
        printf("\nERROR: (Arguments) You provided %d arguments, and the lexical scanner needs at least %d\n\n", argc, MIN_ARGC);
        return 1;
    }
    // redirect the stdin to the file specified
    if (freopen(argv[1], "r", stdin) == NULL){
        printf("\nERROR: (ReadIO) Could not open file \"%s\" for reading to perform the lexical analysis (scanning).\n\n", argv[1]);
        return 2;
    }
    // redirect the stdout to the file specified
    if (freopen(argv[2], "w", stdout) == NULL){
        printf("\nERROR: (WriteIO) Could not open file \"%s\" to writing the result of the lexical analysis (scanning).\n\n", argv[2]);
        fclose(stdin);
        return 3;
    }
    yylex();
    fclose(stdin);
    fclose(stdout);
    return 0;
}
