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

programa: {
		label_c = 0;
		indice = 0;
		temp = 0;
		stackDir = crearStackDir();
		dir = 0;
		id_tipo = 5;
		StackTT = crearTypeStack();
		StackTS = crearSymStack();
		tt_global = crearTypeTab();
		ts_global = crearSymTab();
		insertarTypeTab(StackTT,tt_global);
		insertarSymTab(StackTS,ts_global);
		StackCad = crearStackCad();
	} declaraciones  funciones {
		printPilaSym(StackTS);
		printStack(StackTT);
		print_code(&CODE);
	};

declaraciones:
  tipo {tipo_g = $1.tipo;} lista_var  declaraciones
| tipo_registro {tipo_g = $1.tipo;} lista_var declaraciones
| /*epsilon*/ {};

tipo_registro:
  REGISTRO INICIO declaraciones FIN {
	typetab *tt = crearTypeTab();
	symtab *ts = crearSymTab();
	addStackDir(&stackDir,dir);
	dir = 0;
	insertarTypeTab(StackTT,tt);
	insertarSymTab(StackTS,ts);
	dir = popStackDir(&stackDir);
	typetab *tt1 = sacarTypeTab(StackTT);
	symtab *ts1 = sacarSymTab(StackTS);
	dir = popStackDir(&stackDir);
	$$.tipo = insertarTipo(getTop(StackTT),crearTipo("registro",0,-1,-1,true,ts1));
	id_tipo++;
	};
tipo:
base {base = $1;} tipo_arreglo {$$ = $3;};

base:
  	ENT {$$.tipo = 1; $$.dim = 4;}
	| REAL {$$.tipo = 2; $$.dim = 8;}
	| DREAL {$$.tipo = 3; $$.dim = 16;}
	| CAR {$$.tipo = 4; $$.dim = 2;}
	| SIN {$$.tipo = 0; $$.dim = 0;}
	;

tipo_arreglo:
  LCOR NUM RCOR tipo_arreglo{
	if($2.tipo == 1 && $2.ival >= 1){
		$$.tipo = insertarTipo(getTop(StackTT),crearTipo("array",$4.dim,$4.tipo,$2.ival, false,NULL));
	}
	yyerror("El indice tiene que ser entero y mayor que cero");
	} 
| {
		$$ = base;
	}
	;

lista_var:
  lista_var COMA ID {
	if(buscar(getTopSym(StackTS),$3) == -1){
		
		symbol *sym = crearSymbol($3, tipo_g, dir, "var");
		insertar(getTopSym(StackTS),sym);
		dir = dir + getTam(getTop(StackTT),tipo_g);
	}else{
		yyerror("el identificador ya fue declarado");
	}
}
| ID {
		if(buscar(getTopSym(StackTS),$1) == -1){
			symbol *sym = crearSymbol($1, tipo_g, dir, "var");
			insertar(getTopSym(StackTS),sym);
			dir = dir + getTam(getTop(StackTT),tipo_g);
		}else{
			yyerror("el identificador ya fue declarado");
		}

	};

funciones:
  FUNC tipo ID {
	if(buscar(ts_global,$3) != 1 ){
		symbol *sym = crearSymbol($3, tipo_g, -1, "func");
		insertar(ts_global,sym);
		addStackDir(&stackDir,dir);
		FuncType = $2.tipo;
		dir = 0;
	} else{
		yyerror("el identificador ya fue declarado");
	}
} LPAR argumentos{addListParam(getTopSym(StackTS),$6.lista,$3);} RPAR INICIO  declaraciones {FuncReturn = false;} sentencias FIN funciones {
	{
		insertarSymTab(StackTS,ts_global);
		insertarTypeTab(StackTT,tt_global);
		dir = popStackDir(&stackDir);
		agregar_cuadrupla(&CODE,"label","","",$3);
		label *L = newLabel();
		backpatch(L,$12.lnext);
		char lchar[50];
		label_to_char(lchar,*L);
		agregar_cuadrupla(&CODE,"label","","",lchar);
		sacarSymTab(StackTS);
		sacarTypeTab(StackTT);
		dir = popStackDir(&stackDir);
		if($2.tipo != 0 && FuncReturn == false){
			yyerror("La funcion no tiene valor de retorno");
		}
	}
}
	| /*epsilon*/ {};

argumentos:
  lista_arg { $$.lista = $1.lista; }
| SIN { $$.lista = NULL; };

lista_arg:
  lista_arg arg {
		$$.lista = $1.lista;
		add_tipo($$.lista,$2.tipo);
	}
| arg {
		$$.lista = crearLP();
		add_tipo($$.lista,$1.tipo);
	};

arg:
  tipo_arg ID {
		if(buscar(getTopSym(StackTS),$2) == -1){
			symbol *sym = crearSymbol($2, base.tipo, dir, "var");
			insertar(getTopSym(StackTS),sym);
			dir = dir + getTam(getTop(StackTT),base.tipo);
		}else{
			yyerror("el identificador ya fue declarado");
		}
	};

tipo_arg:
  base param_arr {
	base.tipo = $1.tipo;
	$$.tipo = $2.tipo;
};

param_arr:
  LCOR RCOR param_arr {
		$$.tipo = insertarTipo(getTop(StackTT),crearTipoArray(6,"array",getTipoBase(getTop(StackTT),$3.tipo),$3.dim,-1));
	}
| /*epsilon*/ {
		$$.tipo = base.tipo;
		$$.dim = base.dim;
	};

sentencias:
  sentencias sentencia {
	label *L = newLabel();
	if($1.lnext != NULL){
		backpatch(L,$1.lnext);
	}
	$$.lnext = $2.lnext;
	}
| sentencia {$$.lnext = $1.lnext;};

sentencia:
	SI e_bool ENTONCES  sentencias  FIN {
		label *L = newLabel();
		backpatch(L, $2.ltrue);
		$$.lnext = merge($2.lfalse,$4.lnext);
	} %prec SIX
	| SI e_bool  sentencias  SINO  sentencias  FIN {
		label *L = newLabel();
		label *L1 = newLabel();
		backpatch(L, $2.ltrue);
		backpatch(L1, $2.lfalse);
		$$.lnext = merge($3.lnext,$5.lnext);
	}
	| MIENTRAS  e_bool HACER  sentencias  FIN {
		label *L = newLabel();
		label *L1 = newLabel();
		backpatch(L, $4.lnext);
		backpatch(L1, $2.ltrue);
		$$.lnext = $2.lfalse;
		char lchar[50];
		label_to_char(lchar,*L);
		agregar_cuadrupla(&CODE,"goto","","",lchar);
	}
	| HACER  sentencia  MIENTRAS_QUE e_bool {
		label *L = newLabel();
		label *L1 = newLabel();
		backpatch(L, $4.ltrue);
		backpatch(L1, $2.lnext);
		$$.lnext = $4.lfalse;
		char lchar[50];
		label_to_char(lchar,*L);
		agregar_cuadrupla(&CODE,"goto","","",lchar);
	}
	| ID ASIG expresion {
		if(buscar(getTopSym(StackTS),$1)!=-1){
			int t = getTipo(getTopSym(StackTS),$1);
			int d = getDir(getTopSym(StackTS),$1);
			char di[50];
			sprintf(di,"%d",$3.dir);
			char *alfa = reducir(di,$3.tipo,t);
			char res[100];
			sprintf(res,"%s %d","Id",d);
			if(alfa != NULL){
				agregar_cuadrupla(&CODE,"=",alfa,"",res); 
			}else{
				agregar_cuadrupla(&CODE,"=","0","",res);
			}
			$$.lnext = NULL;
		}else{
			yyerror("Variable no declarada");
		}
	}
	| variable ASIG expresion {
		char d[25];
		sprintf(d,"%d",$3.dir);
		char *alfa = reducir(d,$3.tipo,$1.tipo);
		char b[25];
		sprintf(b,"%c",$1.base[$1.dir]);
		agregar_cuadrupla(&CODE,"=",alfa,"",b);
		$$.lnext = NULL;
	}
	| ESCRIBIR expresion {
		char d[100];
		sprintf(d,"%d",$2.dir);
		agregar_cuadrupla(&CODE,"print",d,"","");
		$$.lnext = NULL;
	}
	| LEER variable {
		char d[100];
		sprintf(d,"%d",$2.dir);
		agregar_cuadrupla(&CODE,"scan",d,"",d);
		$$.lnext = NULL;
	}
	| DEVOLVER {
		if ( FuncType == 0 ){
			agregar_cuadrupla(&CODE,"return","","","");
		}else{
			yyerror("La funcion no puede retonar valores");
		}
		$$.lnext = NULL;
	}
	| DEVOLVER expresion {
		if ( FuncType != 0 ){
			char d[10];
			sprintf(d,"%d",$2.dir);
			char *alfa = reducir(d,$2.tipo,FuncType);
			sprintf(d,"%d",$2.dir);
			agregar_cuadrupla(&CODE,"return",d,"","");
			FuncReturn = true;
		}else{
			yyerror("La funcion debe retonar valor no sin");
		}
		$$.lnext = NULL;
	}
	| TERMINAR {
		char *I = newIndex();
		agregar_cuadrupla(&CODE,"goto","","",I);
		$$.lnext = newLabel();
	};

e_bool:
 e_bool OO e_bool { 
	label *L = newLabel();
	backpatch(L, $1.lfalse);
	$$.ltrue = merge($1.ltrue,$3.ltrue);
	$$.lfalse = $3.lfalse;
	char lchar[50];
	label_to_char(lchar,*L);
	agregar_cuadrupla(&CODE,"label","","",lchar);
	}
	|e_bool YY e_bool {
		label *L = newLabel();
		backpatch(L, $1.ltrue);
		$$.ltrue = merge($1.lfalse,$3.lfalse);
		$$.ltrue = $3.ltrue;
		char lchar[50];
		label_to_char(lchar,*L);
		agregar_cuadrupla(&CODE,"label","","",lchar);
	}
	| NOT e_bool {
		$$.ltrue = $2.lfalse;
		$$.lfalse = $2.ltrue;
	}
	| relacional {
		$$.ltrue = $1.ltrue;
		$$.lfalse = $1.lfalse;
	}
	|VERDADERO {
		char* I = newIndex();
		$$.ltrue = NULL;
		$$.ltrue = create_list(atoi(I));
		agregar_cuadrupla(&CODE,"goto","","",I);
		$$.lfalse = NULL;
	}
	| FALSO {
		char* I = newIndex();
		$$.ltrue = NULL;
		$$.lfalse = create_list(atoi(I));
		agregar_cuadrupla(&CODE,"goto","","",I);
	}
	;

relacional:
	relacional GRT relacional { $$ = relacional($1,$3,$2); }
	| relacional SMT relacional { $$ = relacional($1,$3,$2); }
	| relacional GREQ relacional { $$ = relacional($1,$3,$2); }
	| relacional SMEQ relacional { $$ = relacional($1,$3,$2); }
	| relacional DIF relacional { $$ = relacional($1,$3,$2); }
	| relacional EQEQ relacional { $$ = relacional($1,$3,$2); }
	| expresion {
		$$.tipo = $1.tipo;
		$$.dir = $1.dir;
	};

expresion:
  	expresion MAS expresion {$$ = operacion($1,$3,$2);}
	| expresion MENOS expresion {$$ = operacion($1,$3,$2);}
	| expresion PROD expresion {$$ = operacion($1,$3,$2);}
	| expresion DIV expresion {$$ = operacion($1,$3,$2);}
	| expresion MOD expresion {$$ = operacion($1,$3,$2);}
	| LPAR expresion RPAR {$$.dir = $2.dir;$$.tipo=$2.tipo;}
	| variable {
		$$.dir = atoi(newTemp());
		$$.tipo = $1.tipo;
		char d[50];
		sprintf(d,"%d",$$.dir);
		char b[50];
		sprintf(b,"%c",$1.base[$1.dir]);
		agregar_cuadrupla(&CODE, "*",b,"",d);
	}
	| NUM {
		$$.tipo = $1.tipo;
		if($1.tipo == 1)
			$$.dir = $1.ival;
		if($1.tipo == 2)
			$$.dir = $1.fval;
		if($1.tipo == 3)
			$$.dir = $1.dval;
	}
	| CADENA {
		$$.tipo = 7;
		//$$.dir = addStackCad(StackCad,$1);
	}
	| CARACTER {
		$$.tipo = 4;
		//$$.dir = addStackCad(StackCad,$1);
	}
	| ID LPAR parametros RPAR {
		if(buscar(ts_global,$1) != -1){
			if(strcmp(getTipoVar(ts_global,$1), "func")==0){
				listParam *lista = getListParam(ts_global,$1);
				if(getNumListParam(lista) != getNumListParam($3.lista)){
					yyerror("El numero de argumentos no coincide");
				}
				param *p,*pl;
				p = $3.lista->root;
				pl = lista->root;
				for(int i=0; i<getNumListParam($3.lista);i++){
					if(p->tipo != pl->tipo){
						yyerror("El tipo de los parametros no coincide");
					}p = p->next;
					pl = pl->next;
				}
				$$.dir = atoi(newTemp());
				$$.tipo = getTipo(ts_global,$1);
				char *d;
				sprintf(d,"%d",$$.dir);
				agregar_cuadrupla(&CODE,"=","call",$1,d);
			}
		}else{
			yyerror("El identificador no ha sido declarado");
		}
	}
	;

variable:
	ID {
		if(buscar(getTopSym(StackTS),$1)!=-1){
			$$.dir = getDir(getTopSym(StackTS),$1);
			$$.tipo = getTipo(getTopSym(StackTS),$1);
			strcpy($$.base,"");
		}else{
			yyerror("variable ya definida");
		}
	}
	| arreglo {
		$$.dir = $1.dir;
		strcpy($$.base,$1.base);
		$$.tipo = $1.tipo;
	}
	| ID PT ID {
		if(buscar(ts_global,$1)!=-1){
			int t = getTipo(ts_global,$1);
			char *t1 = getNombre(tt_global,t);
			if (strcmp(t1,"registro") == 0 ){
				tipoBase *tb = getTipoBase(tt_global,t);
			}
		}
	};

arreglo:
	ID LCOR expresion RCOR {
		if(buscar(getTopSym(StackTS),$1) != -1){
			int t = getTipo(getTopSym(StackTS),$1); 
			if (strcmp(getNombre(getTop(StackTT),t),"array") == 0){
				if ($3.tipo == 1){
					strcpy($$.base,$1);
					$$.tipo = getTipoBase(getTop(StackTT),t)->t.type;
					$$.tam = getTam(getTop(StackTT),$$.tipo);
					$$.dir = atoi(newTemp());
					char tm[10];
					intToChar(tm,$$.tam);
					char dr[10];
					intToChar(dr,$$.dir);
					char dre[10];
					intToChar(dre,$3.dir);
					agregar_cuadrupla(&CODE,"*",dre,tm,dr);
				} else{
					yyerror("La expresion para un indice debe ser entero");
				}
			}else{
				yyerror("El identificador debe ser un arreglo");
			}
		
			}else{
				yyerror("El Identificador no ha sido declarado");
			}
	} 
	| arreglo LCOR expresion RCOR  {
		if(strcmp(getNombre(getTop(StackTT),$1.tipo),"array") == 0 ){
			if ($3.tipo == 1){
				strcpy($$.base,$1.base);
				$$.tipo = getTipoBase(getTop(StackTT),$1.tipo)->t.type;
				$$.tam = getTam(getTop(StackTT),$$.tipo);
				int temp_1 = atoi(newTemp());
				$$.dir = atoi(newTemp());
				char t[10];
				sprintf(t,"%d",temp_1);
				char tm[10];
				sprintf(tm,"%d",$$.tam);
				char d2[10];
				sprintf(d2,"%d",$1.dir);
				agregar_cuadrupla(&CODE,"*",d2,tm,t);
				char d[10];
				sprintf(d,"%d",$$.dir);
				sprintf(d2,"%d",$1.dir);
				agregar_cuadrupla(&CODE,"+",d2,t,d);
			}else{
				yyerror("La expresion para un indice debe ser entero");
			}
		}else{
			yyerror("El arreglo no tiene tantas dimentsiones");
		}
	}
	| {};

parametros:
  lista_param {
		$$.lista = $1.lista;
	}
	| { $$.lista = NULL;
	}
	;

lista_param:
  lista_param COMA expresion {
		$$.lista = $1.lista;
		add_tipo($$.lista,$3.tipo);
		char *d;
		sprintf(d,"%d",$3.dir);
		agregar_cuadrupla(&CODE,"param",d,"","");
	}
	| expresion {
		$$.lista = crearLP();
		add_tipo($$.lista,$1.tipo);
		char *d;
		sprintf(d,"%d",$1.dir);
		agregar_cuadrupla(&CODE,"param",d,"","");
	}
	;


%%
void yyerror(char *s){
    printf("Error sintáctico: %s\n");
}
