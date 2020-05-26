/* Declaraciones*/
%{
#include<stdio.h>
#include<string.h>
void yyerror(char *s);
extern int yylex();
%}

%union{
    int val;
}

%token NL
%token<val> NUMERO
/* %left %right %nonassoc */
%left MAS
%left MUL
%nonassoc LPAR RPAR
/*
exp = expresion = E
term = termino = T
factor = F
line = L
*/
%type<val> exp term factor line

%start line

%%
/* Esquema de traduccion*/
// $$ -> $1 $2
line : exp NL {
        /* L.val = E.val*/
        $$ = $1;
        printf("%d\n",$$);
    };

exp : exp MAS term{
        $$ = $1 + $3;
    }
    | term {$$=$1;};

term : term MUL factor{
        $$ = $1 * $3;
    }
    | factor {$$=$1;};

factor : LPAR exp RPAR {$$=$2;}
        | NUMERO {$$=$1;};

%%
/*codigo de usuario*/
void yyerror(char *s){
    printf("Error Sintactico: %s\n",s);
}