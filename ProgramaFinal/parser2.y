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
%}

%union{
    int tipo;
    char id[32];
    char cadena[100];
    char caracter[1];
    int num;
    list lista_tipos;
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
SSTACK *STS;
SYMTAB *newTS;
STS = init_sym_tab_stack();
newTS = init_sym_tab();
push_st(STS,newTS);

//Tabla de tipos
TSTACK *STT;
TYPTAB *newTT;
STT = init_type_tab_stack();
newTT = init_type_tab();
push_tt(STT,newTT);

dir = 0;
CODE *prog,*func;
func = init_code();
prog = func;
};


declaraciones : tipo lista_var SEMICOLON declaraciones {tipo = $$1.type}
            | tipo_registro lista_var SEMICOLON declaraciones {tipo = $$1.type}
            | ;


tipo_registro : STRUCT START declaraciones END 
{
STS.push(newTS())
STT.push(newTT())
SDir.push(dir)
dir = 0
SymTab = STS.pop()
SymTab.typeTab = STT.pop()
tam = getTam(SymTab)
dir = SDir.pop()
tipo_registro.type = STT.getTop().insert(“struct”, tam, SymTab)
};


tipo : base tipo_arreglo
{
baseGBL = base.base
tipo.type = tipo_arreglo.type
};


base : INT {base.base = STT.getTop().getType(“ent”)}
    | FLOAT {base.base = STT.getTop().getType(“real”)}
    | DOUBLE {base.base = STT.getTop().getType(“dreal”)}
    | CHAR {base.base = STT.getTop().getType(“car”)}
    | VOID {base.base = STT.getTop().getType(“sin”)};


tipo_arreglo : LSQBRACK NUM RSQBRACK tipo_arreglo
{
Si num.type = ent Entonces
     Si num.dir > 0 Entonces
            tipo_arreglo.type = STT.getTop().insert(’array’,num, tipo_arreglo1.tipo)
     Sino
            error(“El indice debe ser mayor a 0”)
     Fin Si
Sino
     error(“El indice debe de ser entero”)
Fin Si
}
| {tipo_arreglo.type = baseGBL};

lista_var : lista_var COMMA ID
{
Si no STS.getTop().existe(id) Entonces
    STS.getTop().insert(id, typeGBL, dir, ’var’, null, null )
            dir = dir + STT.getTop().getTam(typeGBL)
Sino
    error(“El id ya existe”)
Fin Si
}
| ID
{
Si no STS.getTop().existe(id) Entonces
    STS.getTop().insert(id, typeGBL, dir, ’var’, null, null )
    dir = dir + STT.getTop().getTam(typeGBL)
Sino
    error(“El id ya existe”)
Fin Si
};


funciones : DEF tipo ID LPAR argumentos RPAR START declaraciones sentencias END funciones
{
Si no STS.getGlobal().existe(id) Entonces
    STS.push(newTS())
    STT.push(newTT())
    SDir.push(dir)
    dir = 0
    listaRET = newListRet()
   Si cmpRet(lista retorno, tipo.type) Entonces
       L = newLabel()
       backpatch(sentencias.nextlist, L)
       genCode(label L)
       STS.pop()
       STT.pop()
   Sino
       error(“El valor no corresponde al tipo de la función”)
   Fin Si
Sino
   error(“El id ya fue declarado”)
Fin Si
}
| ;


argumentos : lista_arg
{
argumentos.lista = lista_arg.lista
argumentos.num = lista_arg.num
}
| VOID
{
argumentos.lista = NULO
argumentos.num = 0
};


lista_arg : lista_arg COMMA arg
{
lista_arg.lista = lista_arg.lista
lista_arg.lista.append(arg.type)
lista_arg.num = lista_arg1.num + 1
}
| arg
{
lista_arg.lista = newList()
lista_arg.lista.append(arg.type)
lista_arg.num = 1
};


arg : tipo_arg ID
{
Si no STS.getTop().existe(id) Entonces
      STS.getTop().append(id, tipo.type, dir, ’arg’, NULO, NULO)
      dir = dir + STT.getTop().getTam(tipo.type)
      arg.type = tipo.type
Sino
     error(“El identificador ya fue declarado”)
Fin Si
};


tipo_arg : base param_arr
{
baseGBL = base.base
tipo_arg.type = param_arr.type
};


param_arr : LSQBRACK RSQBRACK param_arr {param_arr.type = STT.getTop().insert(’array), 0, param_arr1.tipo)}
| {param arr.type = baseGBL};


sentencias : sentencias sentencia
{
L = newLabel()
backpatch(sentencias1.nextlist, L)
genCode(label L)
} | ;


sentencia : IF e_bool THEN sentencia END %prec SIT
{
L = newLabel()
backpatch(e bool.truelist, L)
sentencia.nextlist =combinar(e bool.falselist, Sentencia1.nextlist)
genCode(label L)
}

| IF e_bool THEN sentencia ELSE sentencia END
{
L1 = newLabel()
L2 = newLabel()
backpatch(e bool.truelist, L1)
backpatch(e bool.falselist, L2)
sentencia.nextlist = combinar(sentencia1.nextlist, sentencia2.nextlist)
genCode(label L1)
genCode(’goto’ sentencia1.nextlist[0])
genCode(label L2)
}

| WHILE e_bool DO sentencia END
{
L1 = newLabel()
L2 = newLabel()
backpatch(sentencia1.nextlist, L1)
backpatch(e bool.truelist, L2)
sentencia.nextlist = e bool.falselist
genCode(label L1)
genCode(label L2)
genCode(’goto’ sentencia1.nextlist[0])
}

| DO sentencia WHILE e_bool SEMICOLON
{
L = newLabel()
genCode(”label” L)
batckbatch(sentencia1.nextlist, L)
}

| SWITCH LPAR variable RPAR DO casos predeterminado END
{
prueba = combinar(casos.prueba,predeterminado.prueba)
backpatch(casos.nextlist, L2)
sustituir(”??”, variable.dir, prueba)
}

| variable ASSIGN expresion SEMICOLON
{
dir = reducir(expresion.dir, epresion.type, variable.type)
Si variable.code est = true Entonces
    genCode(variable.base’[’variable.des’]’ ’=’ dir)
Sino
    genCode(variable.dir ’=’ dir)
Fin Si
}

| PRINT expresion SEMICOLON
{gen(”write” expresion.dir)}

| SCAN variable SEMICOLON
{gen(”read” variable.dir)}

| RETURN SEMICOLON
{genCode(”return”)}

| RETURN expresion SEMICOLON
{
index = newIndex()
sentencia.nextlist = newIndexList(index)
genCode(”goto” index)
}

| BREAK SEMICOLON
{
index = newIndex()
sentencia.nextlist = newIndexList(index)
genCode(”goto” index)
}

| START sentencias END {sentencia.nextlist = sentencia1.nextlist};


casos : CASE NUM COLON sentencia casos
{
casos.nextlist =combinar(casos.nextlist, sentencia1.nextlist)
L = newLabel()
/*Indica el inicio del código para la sentencia*/
genCode(”label” L)
casos.prueba = casos1.prueba
casos.prueba.append(if ”??” ”==” num.dir ”goto” L )
}
| CASE NUM COLON sentencia
{
casos.prueba = newCode()
L = newLabel()
/*Indica el inicio del c´odigo para la sentencia*/
genCode(”label” L)
casos.prueba.append(if ”??” ”==” num.dir ”goto” L )
casos.nextlist = sentencia.nextlist
};


predeterminado : PRED COLON sentencia
{
predeterminado.prueba = newCode()
L = newLabel()
/*Indica el inicio del código para la sentencia*/
genCode(”label” L)
predeterminado.prueba.append(”goto” L )
}
| {pretederminado.prueba = NULO};


e_bool : e_bool OR e_bool %prec SIT
{
L = newLabel()
backpatch(e_bool1.falselist, L)
e_bool.truelist = combinar(e_bool1.truelist,e_bool2.truelist )
e_bool.falselist = e_bool2.falselist
genCode(label L)
}
| e_bool AND e_bool
{
L = newLabel()
backpatch(e_bool1.truelist, L)
e_bool.truelist = e_bool1.truelist
e_bool.falselist = combinar(e_bool1.falselist,e_bool2.falselist)
genCode(label L)
}

| NOT e_bool
{
e_bool.truelist = e_bool1.falselist
e_bool.falselist = e_bool.truelist
}

| relacional
{
e_bool.truelist = relacional op.truelist
e_bool.falselist = relacional op.falselist
}

| TRUE
{
index0 = newIndex()
e_bool.truelist = newIndexList(index0)
genCode(’goto’ index0)
}

| FALSE
{
index0 = newIndex()
e_bool.falselist = newIndexList(index0)
genCode(’goto’ index0)
};


relacional : relacional GREATER relacional
{
index0 = newIndex()
index1 = newIndex()
relacional.truelist = newIndexList(index0)
relacional.falselist = newIndexList(index1)
genCode(’if’ relacional1.dir > relacional2 ’goto’ index0)
genCode(’goto’ index1)
}

| relacional LESS relacional
{
index0 = newIndex()
index1 = newIndex()
relacional.truelist = newIndexList(index0)
relacional.falselist = newIndexList(index1)
genCode(’if’ relacional1.dir < relacional2 ’goto’ index0)
genCode(’goto’ index1)
}

| relacional LT relacional
{
index0 = newIndex()
index1 = newIndex()
relacional.truelist = newIndexList(index0)
relacional.falselist = newIndexList(index1)
genCode(’if’ relacional1.dir <= relacional2 ’goto’ index0)
genCode(’goto’ index1)
}

| relacional GT relacional
{
index0 = newIndex()
index1 = newIndex()
relacional.truelist = newIndexList(index0)
relacional.falselist = newIndexList(index1)
genCode(’if’ relacional1.dir >= relacional2 ’goto’ index0)
genCode(’goto’ index1)
}

| relacional NOTEQUAL relacional
{
index0 = newIndex()
index1 = newIndex()
relacional.truelist = newIndexList(index0)
relacional.falselist = newIndexList(index1)
genCode(’if’ relacional1.dir <> relacional2 ’goto’ index0)
genCode(’goto’ index1)
}

| relacional EQUAL relacional
{
index0 = newIndex()
index1 = newIndex()
relacional.truelist = newIndexList(index0)
relacional.falselist = newIndexList(index1)
genCode(’if’ relacional1.dir = relacional2 ’goto’ index0)
genCode(’goto’ index1)
}

| expresion
{
relacional.dir = expresion.dir
relacional.tipo = expresion.tipo
};


expresion : expresion PLUS expresion %prec SIT
{
expresion.type = max(expresion1.type, expresion2.type)
expresion.dir = newTemp()
dir1 = ampliar(expresion1.dir, expresion1.type,expresion.type)
dir2 = ampliar(expresion2.dir, expresion2.type,expresion.type)
getCode(expresion.dir ’=’ dir1 ’+’ dir2)
}

| expresion MINUS expresion
{
expresion.type = max(expresion1.type, expresion2.type)
expresion.dir = newTemp()
dir1 = ampliar(expresion1.dir, epxresion1.type,expresion.type)
dir2 = ampliar(expresion2.dir, epxresion2.type,expresion.type)
getCode(expresion.dir ’=’ dir1 ’-’ dir2)
}

| expresion MUL expresion
{
expresion.type = max(expresion1.type, expresion2.type)
expresion.dir = newTemp()
dir1 = ampliar(expresion1.dir, epxresion1.type,expresion.type)
dir2 = ampliar(expresion2.dir, epxresion2.type,expresion.type)
getCode(expresion.dir ’=’ dir1 ’*’ dir2)
}

| expresion DIV expresion
{
expresion.type = max(expresion1.type, expresion2.type)
expresion.dir = newTemp()
dir1 = ampliar(expresion1.dir, epxresion1.type,expresion.type)
dir2 = ampliar(expresion2.dir, epxresion2.type,expresion.type)
getCode(expresion.dir ’=’ dir1 ’/’ dir2)
}

| expresion MOD expresion
{
Si expresion1.type = entero and expresion2.type = entero Entonces
    expresion.type = max(expresion1.type,expresion2.type)
    expresion.dir = newTemp()
    dir1 = ampliar(expresion1.dir, epxresion1.type,expresion.type)
    dir2 = ampliar(expresion2.dir, epxresion2.type,expresion.type)
    getCode(expresion.dir ’=’ dir1 ’+’ dir2)
Sino
   error(“No se puede obtener el modulo si los operandos no son enteros”)
Fin Si
}

| LPAR expresion RPAR
{
expresion.type = expresion1.type
expresion.dir = expresion1.dir
}

| variable
{
expresion.type = variable.type
expresion.dir = variable.dir
}

| NUM
{
expresion.type = num.type
expresion.dir = num.dir
}

| STRING
{
expresion.type =’string’
Si TablaCadenas.existe(cadena) Entonces
     expresion.dir= TablaCadena.getIndexStr(cadena)
Sino
     expresion.dir=TablaCadena.insert(cadena)
Fin Si
}

| CHAR
{
expresion.type =’car’
Si TablaCadenas.existe(car) Entonces
     expresion.dir= TablaCadena.getIndexStr(car)
Sino
     expresion.dir=TablaCadena.insert(car)
Fin Si
};


variable : ID variable_comp
{
Si STS.getTop().existe(id) Entonces
     IDGBL = id
     Si variable_comp.code_est = true Entonces
          variable.dir=newTemp()
          variable.type = variable_comp.type
          genCode(variable.dir ’=’ id’[’ variable_comp.des’]’)
          variable.base = id.dir
          variable.code_est= true
          variable.des = variable_comp.des
Sino
         variable.dir = id)
         variable.type = STS.getTop().getType(id)
         variable.code_est= false
Sino
        error(“No existe la variable”)
Fin Si
};


variable_comp : dato_est_sim
{
variable_comp.type = dato_est_sim.type
variable_comp.des = dato_est_sim.des
variable_comp.code_est = dato_est_sim.code_est
}

| arreglo
{
variable_comp.type = arreglo.type
variable_comp.des = arreglo.dir
variable_comp.code_est = true
}

| LPAR parametros RPAR
{
Si STS.getGlobal().getVar(IDGBL)= ’func’ Entonces
     lista = STS.getGlobal().getListaArgs(IDGBL)
     num = STS.getGlobal().getNumArgs(ID)
     Si num = parametros.num Entonces
         Para cada i = 0 hasta i = num hacer
              Si lista[i] !=parametros.lista[i] entonces
                  Error(“Los parámetros pasados no coinciden con los parámetros de la función”)
Fin si
};


dato_est_sim : dato_est_sim POINT ID
{
Si dato_est_sim1.estructura = true Entonces
    Si dato_est_sim1.tabla.existe(id) Entonces
        dato_est_sim.des = dato_est_sim1.des + dato_est_sim.tabla1.getDir(id)
        typeTemp=dato_est_sim1.tabla.getType(id)
        estTemp = dato_est_sim1.tabla.tablaTipos.getName(typeTemp)
        Si estTemp = ’struct’ Entonces
            dato_est_sim.estructura= true
            dato_est_sim.tabla= dato_est_sim.tabla.tablaTipos.getTipoBase(typeTemp).tabla
        Sino
            dato_est_sim.estructura= false
            dato_est_sim.tabla= NULO
            dato_est_sim.type = dato_est_sim1.tabla.getType(id)
        FinSi
    dato est sim.code est=true
    Sino
           error(“No existe estructura con ese id”)
    FinSi
Sino
    error(“No existe la estructura”)
FinSi
}

| {
typeTemp = STS.getTop().getType(id)
Si STT.getTop().getName(typeTemp) =’struct’ Entonces
     dato_est_sim.estructura= true
     dato_est_sim.tabla= STT.getTop().getTipoBase(typeTemp).tabla
     dato_est_sim.des = 0
Sino
     dato_est_sim.estructura= false
     dato_est_sim.type = STT.getTop().getType(id)
Fin Si
dato_est_sim.code est=false
};


arreglo : LSQBRACK expresion RSQBRACK
{
arreglo.type = STS.getTop().getType(IDGBL)
Si STT.getTop().getName(arreglo.type) = ’array’ Entonces
    Si expresion.type = entero Entonces
        typeTemp = STT.getTop().getTypeBase(arreglo.type)
        tam = STT.getTop().getTam(typeTemp)
        arreglo.dir = newTemp()
        genCode(arreglo.dir’=’ expresion.dir ’*’ tam)
    Sino
        error(“La expresión no es de tipo entero”)
    Fin Si
Sino
    error(“No existe el arreglo”)
Fin Si
}

| arreglo LSQBRACK expresion RSQBRACK
{
arreglo.type = STS->head.getType(arreglo1.type)
Si STT.getTop().getName(arreglo.type) = ’array’ Entonces
    Si expresion.type = entero Entonces
        typeTemp = STT.getTop().getTypeBase(arreglo.type)
        tam = STT.getTop().getTam(typeTemp)
        dirTemp = newTemp()
        arreglo.dir = newTemp()
        genCode(dirTemp’=’ expresion.dir ’*’ tam)
        genCode(arreglo.dir’=’ arreglo1.dir ’+’ dirTemp)
    Sino
        error(“La expresión no es de tipo entero”)
    Fin Si
Sino
   error(“No existe el arreglo”)
Fin Si
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
