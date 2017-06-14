%{
#include <assert.h>
#include <stdio.h>

static int Pop();
static int Top();
static void Push(int val);

%}

%token T_Int

%%
S : S E '\n' { printf("= %d\n", Top()); }
 |
 ;
E: E E '+' { Push(Pop() + Pop()); }
| E E '-' { int op2 = Pop(); Push(Pop() - op2); }
| E E '*' { Push(Pop() * Pop()); }
| E E '/' { int op2 = Pop(); Push(Pop() / op2); }
| T_Int { Push(yylval); }
;
%%
static int stack[100], count = 0;
static int Pop() {
    assert(count > 0);
    return stack[--count];
}
static int Top() {
    assert(count > 0);
    return stack[count-1];
}
static void Push(int val) {
    assert(count < sizeof(stack)/sizeof(*stack));
    stack[count++] = val;
}

int yyerror(char *s){
    printf("ERROR: %s\n", s);
    return 1;
}

int main() {
    return yyparse();
}

