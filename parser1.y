%{
#include <stdio.h>
int tipo;
extern int yylex();
void yyerror(char *s);
%}

%union{
    int tipo;
    char id[32];
    char cadena[100];
    char caracter[1];
    int num;
}

%token<id>  ID
%token<num> NUM
%token<cadena> STRING
%token<caracter> CHAR
%token  START END
%token  INT FLOAT DOUBLE CHAR VOID STRUCT
%token  COLON COMMA WHILE DO PRINT SCAN PRED
%token  RETURN SWITCH BREAK CASE DEF DEFAULT SEMICOLON POINT
%token  TRUE FALSE
%right  ASSIGN
%left   OR
%left   AND
%left   EQUAL NOTEQUAL
%left   GREATER LESS LT GT
%left   PLUS MINUS
%left   MUL DIV MOD
%left   NOT
%nonassoc   LPAR RPAR LSQBRACK RSQBRACK
%left IF THEN
%nonassoc   SIT 
%nonassoc   ELSE

%start programa
/*
Fecha: 25/05/2020
Autor: Martínez Martínez Brayan Eduardo,
Pachuca Cortés Santiago Emilio
Descripción: Gramática
*/
%%

programa : declaraciones funciones;

declaraciones : tipo lista_var SEMICOLON declaraciones
            | tipo_registro lista_var SEMICOLON declaraciones
            | ;

tipo_registro : STRUCT START declaraciones END;

tipo : base tipo_arreglo;

base : INT
    | FLOAT
    | DOUBLE
    | CHAR
    | VOID;

tipo_arreglo : LSQBRACK NUM RSQBRACK tipo_arreglo
            | ;

lista_var : lista_var COMMA ID
            | ID;      

funciones : DEF tipo ID LPAR argumentos RPAR START declaraciones sentencias END funciones
            | ;

argumentos : lista_arg
            | VOID;

lista_arg : lista_arg COMMA arg
            | arg;

arg : tipo_arg ID;

tipo_arg : base param_arr;

param_arr : LSQBRACK RSQBRACK param_arr
            | ;

sentencias : sentencias sentencia
            |sentencia;

sentencia : IF e_bool THEN sentencia END %prec SIT
            | IF e_bool THEN sentencia ELSE sentencia END
            | WHILE e_bool DO sentencia END
            | DO sentencia WHILE e_bool SEMICOLON
            | SWITCH LPAR variable RPAR DO casos predeterminado END
            | variable ASSIGN expresion SEMICOLON
            | PRINT expresion SEMICOLON
            | SCAN variable SEMICOLON
            | RETURN SEMICOLON
            | RETURN expresion SEMICOLON
            | BREAK SEMICOLON
            | START sentencias END;

casos : CASE NUM COLON sentencia casos
	| CASE NUM COLON sentencia;

predeterminado : PRED COLON sentencia
                | ;

e_bool : e_bool OR e_bool %prec SIT
        | e_bool AND e_bool
        | NOT e_bool
        | relacional
        | TRUE
        | FALSE;

relacional : relacional GREATER relacional
            | relacional LESS relacional
            | relacional LT relacional
            | relacional GT relacional
            | relacional NOTEQUAL relacional
            | relacional EQUAL relacional
            | expresion;

expresion : expresion PLUS expresion %prec SIT
            | expresion MINUS expresion
            | expresion MUL expresion
            | expresion DIV expresion
            | expresion MOD expresion
            | LPAR expresion RPAR
            | variable
            | NUM
            | STRING
            | CHAR;

variable : ID variable_comp

variable_comp : dato_est_sim
                | arreglo
                | LPAR parametros RPAR;

dato_est_sim : dato_est_sim POINT ID
            | ;

arreglo : LSQBRACK expresion RSQBRACK
        | arreglo LSQBRACK expresion RSQBRACK;

parametros : lista_param
            | ;

lista_param : lista_param COMMA expresion
            | expresion;

%%
void yyerror(char *s){
    printf("Error sintáctico: %s\n");
}
