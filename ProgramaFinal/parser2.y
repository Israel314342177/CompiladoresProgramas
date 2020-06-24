%{
#include <stdio.h>
#include "backpatch.h"
#include "tabla_simbolos.h"
#include "tipos.h"
#include "estructuras.h"

int tipo;
int dir;
extern int yylex();
void yyerror(char *s);
SSTACK *STS;
STS = init_sym_tab_stack();
TSTACK *STT;
STT = init_type_tab_stack();
listD *Sdir;
Sdir = nuevaListaD();
%}

%union{
    int tipo;
    char id[32];
    char cadena[100];
    char caracter[1];
    int num;
    list *lista_tipos;
    dt *type_dir;
}

%token<id>  ID
%token<num> NUM
%token<cadena> STRING
%token<caracter> CHAR
%token  START END
%token  INT FLOAT DOUBLE VOID STRUCT
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


%type<lista_tipos> lista_arg argumentos parametros lista_param
%type<type_dir> variable expresion arreglo relacional tipo tipo_registro tipo_arreglo
%type<num> base

%start programa
/*
Fecha: 25/05/2020
Autor: Martínez Martínez Brayan Eduardo,
Pachuca Cortés Santiago Emilio
Descripción: Gramática
*/
%%

programa : declaraciones funciones  {
//Tabla de simbolos
SYMTAB *newTS;
newTS = init_sym_tab();
push_st(STS,newTS);

//Tabla de tipos
TYPTAB *newTT;
newTT = init_type_tab();
push_tt(STT,newTT);

dir = 0;
CODE *prog,*func;
func = init_code();
prog = func;
};


declaraciones : tipo lista_var SEMICOLON declaraciones {tipo = $1.type;}
            | tipo_registro lista_var SEMICOLON declaraciones {tipo = $1.type;}
            | ;


tipo_registro : STRUCT START declaraciones END 
{
//Tabla de simbolos
SYMTAB *newTS;
newTS = init_sym_tab();
push_st(STS,newTS);

//Tabla de tipos
TYPTAB *newTT;
newTT = init_type_tab();
push_tt(STT,newTT);

dt *dirTR;
dirTR = init_dt();
dirTR -> dir = dir;
push_dir(SDir,dirTR);
dir = 0;
SYMTAB *SymTab;
SymTab = pop_st(STS);
SymTab->tt = pop_tt(STT);
tam = sizeof(SymTab);
dir = pop_dir(SDir)->dir;
TYP *newTipo;
newTipo = init_sym();
newTipo-> tam = tam;
strcpy(newTipo->nombre,"struct");
newTipo->tb->is_est = 0;
newTipo->tb->tipo->SS = SymTab;
$$.type = append_type(STT->top,newTipo);
};


tipo : base tipo_arreglo
{
base = $1.base;
$$.type = $2.type;
};


base : INT {$$.base = 0;}
    | FLOAT {$$.base = 0;}
    | DOUBLE {$$.base = 0;}
    | CHAR {$$.base = 0;}
    | VOID {$$.base = -1;};


tipo_arreglo : LSQBRACK NUM RSQBRACK tipo_arreglo
{
if(sizeof($2)==sizeof(int)){
    if($2.dir > 0)
        $$.type = STT.getTop().insert(’array’,num, $1.tipo)
     else
        printf("El indice debe ser mayor a 0");
}
else
     printf("El indice debe de ser entero");
}
| {tipo_arreglo.type = baseGBL};


lista_var : lista_var COMMA ID
{
if(!STS.getTop().existe($3))
    STS.getTop().insert($3, typeGBL, dir, ’var’, null, null )
            dir = dir + STT.getTop().getTam(typeGBL)
else
    printf("El id ya existe");
}
| ID
{
if(!STS.getTop().existe($1))
    STS.getTop().insert($1, typeGBL, dir, ’var’, null, null )
    dir = dir + STT.getTop().getTam(typeGBL)
else
    printf("El id ya existe");
};


funciones : DEF tipo ID LPAR argumentos RPAR START declaraciones sentencias END funciones
{
if(!STS.getGlobal().existe($3)){
    STS.push(newTS())
    STT.push(newTT())
    SDir.push(dir)
    dir = 0;
    listaRET = newListRet()
   if(cmpRet(lista_RET,$2.type)){
       L = newLabel()
       backpatch(sentencias.nextlist, L)
       genCode(label L)
       STS.pop()
       STT.pop()
   }
   else
       printf("El valor no corresponde al tipo de la función");
}
else
   printf("El id ya fue declarado");
}
| ;


argumentos : lista_arg
{
$$.lista = $1.lista;
$$.num = $1.num;
}
| VOID
{
$$.lista = NULL;
$$.num = 0;
};


lista_arg : lista_arg COMMA arg
{
$$.lista = $1.lista;
$$.lista.append(arg.type);
$$.num = $11.num + 1;
}
| arg
{
$$.lista = newList()
$$.lista.append($3.type)
$$.num = 1
};


arg : tipo_arg ID
{
if(!STS.getTop().existe($2)){
    STS.getTop().append(id, $1.type, dir, ’arg’, NULO, NULO)
    dir = dir + STT.getTop().getTam($1.type)
    $$.type = $1.type
}
else
     printf("El identificador ya fue declarado");
};


tipo_arg : base param_arr
{
baseGBL = $1.base
$$.type = $2.type
};


param_arr : LSQBRACK RSQBRACK param_arr {$$.type = STT.getTop().insert(’array), 0, $1.tipo)}
| {$$.type = baseGBL};


sentencias : sentencias sentencia
{
L = newLabel()
backpatch($1.nextlist, L)
genCode(label L)
} | ;


sentencia : IF e_bool THEN sentencia END %prec SIT
{
L = newLabel()
backpatch($2.truelist, L)
$$.nextlist =combinar($2.falselist, $4.nextlist)
genCode(label L)
}

| IF e_bool THEN sentencia ELSE sentencia END
{
L1 = newLabel()
L2 = newLabel()
backpatch($2.truelist, L1)
backpatch($2.falselist, L2)
$$.nextlist = combinar($4.nextlist,$6.nextlist)
genCode(label L1)
genCode(’goto’ $4.nextlist[0])
genCode(label L2)
}

| WHILE e_bool DO sentencia END
{
L1 = newLabel()
L2 = newLabel()
backpatch($4.nextlist, L1)
backpatch($2.truelist, L2)
$$.nextlist = $2.falselist
genCode(label L1)
genCode(label L2)
genCode(’goto’ $4.nextlist[0])
}

| DO sentencia WHILE e_bool SEMICOLON
{
L = newLabel()
genCode(”label” L)
batckbatch($2.nextlist, L)
}

| SWITCH LPAR variable RPAR DO casos predeterminado END
{
prueba = combinar($6.prueba,$7.prueba)
backpatch($6.nextlist, L2)
sustituir(”??”, $3.dir, prueba)
}

| variable ASSIGN expresion SEMICOLON
{
dir = reducir($3.dir,$3.type,$1.type)
if($1.code_est = true)
    genCode($1.base'['$1.des']' '=' dir)
else
    genCode($1.dir ’=’ dir)
}

| PRINT expresion SEMICOLON
{gen(”write” $2.dir)}

| SCAN variable SEMICOLON
{gen(”read” $2.dir)}

| RETURN SEMICOLON
{genCode("return")}

| RETURN expresion SEMICOLON
{
index = newIndex()
$$.nextlist = newIndexList(index)
genCode("goto" index)
}

| BREAK SEMICOLON
{
index = newIndex()
$$.nextlist = newIndexList(index)
genCode(”goto” index)
}

| START sentencias END {$$.nextlist = $2.nextlist};


casos : CASE NUM COLON sentencia casos
{
$$.nextlist =combinar($5.nextlist, $4.nextlist)
L = newLabel()
/*Indica el inicio del código para la sentencia*/
genCode("label" L)
$$.prueba = $5.prueba
$$.prueba.append(if "??" "==" $2.dir "goto" L )
}
| CASE NUM COLON sentencia
{
$$.prueba = newCode()
L = newLabel()
/*Indica el inicio del c´odigo para la sentencia*/
genCode("label" L)
$$.prueba.append(if "??" "==" $2.dir "goto" L)
$$.nextlist = $4.nextlist
};


predeterminado : PRED COLON sentencia
{
$$.prueba = newCode()
L = newLabel()
/*Indica el inicio del código para la sentencia*/
genCode("label" L)
$$.prueba.append("goto" L)
}
| {$$.prueba = NULO};


e_bool : e_bool OR e_bool %prec SIT
{
L = newLabel()
backpatch($1.falselist, L)
$$.truelist = combinar($1.truelist,$2.truelist)
$$.falselist = $3.falselist
genCode(label L)
}

| e_bool AND e_bool
{
L = newLabel()
backpatch($1.truelist, L)
$$.truelist = $1.truelist
$$.falselist = combinar($1.falselist,$3.falselist)
genCode(label L)
}

| NOT e_bool
{
$$.truelist = $2.falselist
$$.falselist = $2.truelist
}

| relacional
{
$$.truelist = $1.truelist
$$.falselist = $1.falselist
}

| TRUE
{
index0 = newIndex()
$$.truelist = newIndexList(index0)
genCode('goto' index0)
}

| FALSE
{
index0 = newIndex()
$$.falselist = newIndexList(index0)
genCode('goto' index0)
};


relacional : relacional GREATER relacional
{
index0 = newIndex()
index1 = newIndex()
$$.truelist = newIndexList(index0)
$$.falselist = newIndexList(index1)
genCode('if' $1.dir > $3 'goto' index0)
genCode('goto' index1)
}

| relacional LESS relacional
{
index0 = newIndex()
index1 = newIndex()
$$.truelist = newIndexList(index0)
$$.falselist = newIndexList(index1)
genCode('if' $1.dir < $3 'goto' index0)
genCode('goto' index1)
}

| relacional LT relacional
{
index0 = newIndex()
index1 = newIndex()
$$.truelist = newIndexList(index0)
$$.falselist = newIndexList(index1)
genCode('if' $1.dir <= $3 'goto' index0)
genCode('goto' index1)
}

| relacional GT relacional
{
index0 = newIndex()
index1 = newIndex()
$$.truelist = newIndexList(index0)
$$.falselist = newIndexList(index1)
genCode('if' $1.dir >= $3 'goto' index0)
genCode('goto' index1)
}

| relacional NOTEQUAL relacional
{
index0 = newIndex()
index1 = newIndex()
$$.truelist = newIndexList(index0)
$$.falselist = newIndexList(index1)
genCode('if' $1.dir <> $3 'goto' index0)
genCode('goto' index1)
}

| relacional EQUAL relacional
{
index0 = newIndex()
index1 = newIndex()
$$.truelist = newIndexList(index0)
$$.falselist = newIndexList(index1)
genCode('if'$1.dir = $3 'goto' index0)
genCode('goto' index1)
}

| expresion
{
$$.dir = $1.dir;
$$.tipo = $1.tipo;
};


expresion : expresion PLUS expresion %prec SIT
{
$$.type = max($1.type, $3.type)
$$.dir = newTemp()
dir1 = ampliar($1.dir,$1.type,$3.type)
dir2 = ampliar($3.dir,$3.type,$1.type)
getCode($$.dir ’=’ dir1 ’+’ dir2)
}

| expresion MINUS expresion
{
$$.type = max($1.type,$3.type)
$$.dir = newTemp()
dir1 = ampliar($1.dir,$1.type,$3.type)
dir2 = ampliar($3.dir,$3.type,$1.type)
getCode($$.dir ’=’ dir1 ’-’ dir2)
}

| expresion MUL expresion
{
$$.type = max($1.type,$3.type)
$$.dir = newTemp()
dir1 = ampliar($1.dir, $1.type,$3.type)
dir2 = ampliar($3.dir, $3.type,$1.type)
getCode($$.dir ’=’ dir1 ’*’ dir2)
}

| expresion DIV expresion
{
$$.type = max($1.type,$3.type)
$$.dir = newTemp()
dir1 = ampliar($1.dir, $1.type,$3.type)
dir2 = ampliar($3.dir, $3.type,$1.type)
getCode($$.dir ’=’ dir1 ’/’ dir2)
}

| expresion MOD expresion
{
if($1.type = entero && $3.type = entero){
    $$.type = max($1.type,$3.type);
    $$.dir = newTemp();
    dir1 = ampliar($1.dir, $1.type,$3.type);
    dir2 = ampliar($3.dir, $3.type,$1.type);
    getCode($$.dir ’=’ dir1 ’+’ dir2);
}
else
   printf("No se puede obtener el modulo si los operandos no son enteros");
}

| LPAR expresion RPAR
{
$$.type = $2.type;
$$.dir = $2.dir;
}

| variable
{
$$.type = $1.type;
$$.dir = $1.dir;
}

| NUM
{
$$.type = $1.type;
$$.dir = $1.dir;
}

| STRING
{
$$.type ='string'
if(TablaCadenas.existe(cadena))
     $$.dir= TablaCadena.getIndexStr(cadena);
else
     $$.dir=TablaCadena.insert(cadena);
}

| CHAR
{
$$.type =’car’
if(TablaCadenas.existe(car))
     $$.dir= TablaCadena.getIndexStr(car)
else
     $$.dir=TablaCadena.insert(car)
};


variable : ID variable_comp
{
if(STS.getTop().existe($1)){
    IDGBL = $1
    if($2.code_est = true){
        $$.dir=newTemp()
        $$.type = $2.type
        genCode($$.dir ’=’ $1’[’ $2.des’]’)
        $$.base = $1.dir
        $$.code_est= true
        $$.des = $2.des
    }
    else{
        $$.dir = $1
        $$.type = STS.getTop().getType($1)
        $$.code_est= false
    }
}
else
    printf("No existe la variable");
};


variable_comp : dato_est_sim
{
$$.type = $1.type
$$.des = $1.des
$$.code_est = $1.code_est
}

| arreglo
{
$$.type = $1.type
$$.des = $1.dir
$$.code_est = true
}

| LPAR parametros RPAR
{
if(STS.getGlobal().getVar(IDGBL)= ’func’){
    lista = STS.getGlobal().getListaArgs(IDGBL)
     num = STS.getGlobal().getNumArgs(ID)
     if(num = $2.num){
         for(i=0;i=num;i++){
             if(lista[i]!=$2.lista[i])
                printf("Los parámetros pasados no coinciden con los parámetros de la función");
         }
     }
}
};


dato_est_sim : dato_est_sim POINT ID
{
if($1.estructura = true){
    if($1.tabla.existe(id)){
        $$.des = $1.des + $$.tabla1.getDir(id)
        typeTemp=$1.tabla.getType(id)
        estTemp = $1.tabla.tablaTipos.getName(typeTemp)
        if(estTemp = ’struct’){
            $$.estructura= true
            $$.tabla= $$.tabla.tablaTipos.getTipoBase(typeTemp).tabla
        }
        else{
            $$.estructura= false
            $$.tabla= NULO
            $$.type = $1.tabla.getType(id)
        }
        dato est sim.code est=true
    }
    else
        printf("No existe estructura con ese id");
}
else
    printf("No existe la estructura");
}

| {
typeTemp = STS.getTop().getType(id)
if(STT.getTop().getName(typeTemp) =’struct’){
    $$.estructura= true
    $$.tabla= STT.getTop().getTipoBase(typeTemp).tabla
    $$.des = 0
}
else{
    $$.estructura= false
    $$.type = STT.getTop().getType(id)
}
$$.code_est=false
};


arreglo : LSQBRACK expresion RSQBRACK
{
$$.type = STS.getTop().getType(IDGBL)
if(STT.getTop().getName($$.type) = ’array’){
    if($2.type = entero){
        typeTemp = STT.getTop().getTypeBase($$.type)
        tam = STT.getTop().getTam(typeTemp)
        $$.dir = newTemp()
        genCode($$.dir’=’ $2.dir ’*’ tam)
    }
    else
        printf("La expresión no es de tipo entero");
}
else
    printf("No existe el arreglo");
}

| arreglo LSQBRACK expresion RSQBRACK
{
$$.type = STS->head.getType($1.type);
if(STT->top().getName(arreglo.type) = ’array’){
    if($3.type = entero){
        typeTemp = STT.getTop().getTypeBase($1.type)
        tam = STT.getTop().getTam(typeTemp)
        dirTemp = newTemp()
        $$.dir = newTemp()
        genCode(dirTemp’=’ $3.dir ’*’ tam)
        genCode($$.dir’=’ $1.dir ’+’ dirTemp)
    }
    else
        printf("La expresión no es de tipo entero");
}
else
   printf("No existe el arreglo");
};


parametros : lista_param
{
$$ = $1;
$$.num = $$1.num;
}

|{
$$ = NULL;
$$.num = 0
};


lista_param : lista_param COMMA expresion
{
$$ = $1;
append_tipo($$.lista_tipos,init_tipo($1.type));
$$.num = $$.num + 1;
}
| expresion
{
$$ = nuevaLista();
append_tipo($$,init_tipo($1.type));
$$.num = 1;
};


%%
void yyerror(char *s){
    printf("Error sintáctico: %s\n");
}
