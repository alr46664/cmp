#include <stdio.h>
#include <stdlib.h>

int yylex(void);

int main(int argc, char **argv) {
    while(yylex() != 0) {}
    return 0;
}
