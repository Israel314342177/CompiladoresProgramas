#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "tabla_simbolos.h"

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Agrega al final de la tabla de simbolos un nuevo simbolo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
solucionar el problema de como guardaba los simbolos, ya que estos no se estaban ingresando correctamente
y mucha informacion se perdia, se cambio a que tail guarde al nuevo simbolo en vez de head
*/
void append_sym(SYMTAB *SS, SYM *S){
    if(SS->head == NULL)
     {
       SS->head = SS->tail = S;
     }
   else
     {
      SS->tail->next = S;
      SS->tail = S;
     }
     SS->num = SS->num+1;
}

 /*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Deja vacia la tabla simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
solucionar el problema en la condicion del while, pues antes se verificaba que SS fuera distinto de NULL,
ahora se verifica que el head de SS sea distinto de NULL, ademas de apuntar a NULL al tail de SS y reiniciar
en 0 el contador de elementos de la tabla
*/
void clear_sym_tab(SYMTAB *SS){
    SYM *S;
    while(SS->head!=NULL){
        S = SS->head;
        SS->head=SS->head->next;
        S->next=NULL;
        finish_sym(S);
    }
    SS->tail=NULL;
    SS->num=0;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Ejecuta un pop sobre la pila de tablas de simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
solucionar el problema con el orden en el que se reasignaban los punteros de next y top, en el segundo if
se agrego el return para que no causara problemas con las siguientes instrucciones
*/
SYMTAB *pop_st(SSTACK *SSS){
    SYMTAB *SS,*aux;
    if(SSS->top == NULL) /* pila vacía */
        return NULL;

    if(SSS->top == SSS->tail) /* pila con un solo elemento */
    {
        SS = SSS->tail;
        SSS -> top = SSS->tail = NULL;
        return SS;
    }

    aux = SSS->tail;
    while(aux->next!=SSS->top)
      aux = aux->next;
    aux->next = NULL;
    SS = SSS->top;
    SSS->top = aux;
    return SS;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Ingresa una tabla de simbolos a la pila de tablas de simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
solucionar el problema con el orden en el que se reasignaban los punteros de top->next y top, ya que el push no
se estaba realizando correctamente y los punteros apuntaban a otra direccion a la esperada
*/
void push_st (SSTACK *SSS, SYMTAB *SS){
    if(SSS->top == NULL)
    {
      SSS->top = SSS->tail = SS;
      return;
    }
    SSS->top->next = SS;
    SSS->top = SS;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para la pila de simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
se agregaron las asignaciones para que el top y el tail de la pila apunten a null
*/
SSTACK *init_sym_tab_stack(){
    SSTACK *SSS;
    SSS = (SSTACK *)malloc(sizeof(SSTACK));
    SSS -> top = NULL;
    SSS -> tail = NULL;
    return SSS;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para una tabla de simbolos vacia
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se cambio la forma de asignar los nombres a los simbolos mediante un strcpy y se agregaron
 las asignaciones para que el tanto el head, tail, la tabla de tipos asociada y el simbolo siguiente
 sean NULL, ademas el contador de simbolos se inicializa en 0
*/
SYMTAB *init_sym_tab(){
    SYMTAB *SS;
    SS = (SYMTAB *)malloc(sizeof(SYMTAB));
    SS -> head = NULL;
    SS -> tail = NULL;
    SS -> num = 0;
    SS -> tt = NULL;
    SS -> next = NULL;
    return SS;
}

 /*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para un simbolo vacio
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se asigno a NULL el puntero del siguiente del simbolo creado
*/
SYM *init_sym(){
   SYM *S;
   S = (SYM *)malloc(sizeof(SYM));
   S -> next = NULL;
   return S;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera la memoria para la pila de simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se cambio la condicion del while, antes era SSS!=NULL pero causaba problemas, 
 se cambio a que el top de la pila fuera distinto de NULL para que terminara
*/
void finish_sym_tab_stack(SSTACK *SSS){
    SYMTAB *SS;
    while(SSS->top!=NULL){
        SS = pop_st(SSS);
        finish_sym_tab(SS);
    }
    free(SSS);
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera memoria para una tabla de simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se quito el codigo innecesario para que se reultilizara la funcion clear_sym_tab
*/
void finish_sym_tab(SYMTAB *SS){
    clear_sym_tab(SS);
    free(SS);
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: libera memoria para un simbolo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se agrego la condicion de que S no fuera nulo para poder liberar la memoria
*/
void finish_sym (SYM *S){
    if(S!=NULL)
       free(S);
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Imprime en pantalla la tabla de simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se agrego un contador para poder imprimir la posicion de los simbolos
*/
void print_Stab(SYMTAB *SS){
    int cont=0;
    SYM *S;
    int sim;
    printf("\n");
    if(SS->head == NULL)
    {
     printf("\n --- TABLA VACIA (NO HAY SIMBOLOS). ---\n\n");
     return;
    }
    printf("\tTABLA DE SIMBOLOS:\n");
    printf(" Pos      id        Dir        Tipo        Var      Args     Num\n");
    S = SS->head;
    while(S!=NULL)
    {
      //imprimirSimbolo(S);
      printf(" %2d%8s        %3d         %3d          %d        %2d      %2d",cont,S->id,S->dir,S->tipo,S->var,0,0);
      printf("\n");
      S = S->next;
      cont++;
    }
    printf("\n");
}


 /*
 Fecha: 02/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Busca un simbolo en la pila de tablas de simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 03/06/2020
 Modificacion:
 se quitaron instrucciones innecesarias para asignar la tabla en la que se empezara
 la busqueda, tambien se agregaron las asignaciones a NULL cuando no se encuentra un simbolo
*/
SYM *search_sym_tab_stack(SSTACK *SSS,char *id){
    SYMTAB *auxSymTab;
    SYM *auxSym;
    if(SSS->top == NULL) /* pila vacía */
        return NULL;

    auxSymTab = SSS->tail;
    while(auxSymTab!=NULL){
        auxSym = search_sym_tab(auxSymTab,id);
        if(auxSym!=NULL)
            return auxSym;
        auxSymTab = auxSymTab->next;
    }
    return NULL;
}


 /*
 Fecha: 02/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Busca un simbolo en una tabla de simbolos
 Modifico: Pachuca Cortes Santiago Emilio el 03/06/2020
 Modificacion:
 se quitaron instrucciones innecesarias para asignar la tabla en la que se empezara
 la busqueda, tambien se agregaron las asignaciones a NULL cuando no se encuentra un simbolo
*/
SYM *search_sym_tab(SYMTAB *SS,char *id){
    SYM *auxSym;
    if(SS->head == NULL) /* tabla vacía */
        return NULL;

    auxSym = SS->head;
    while(auxSym!=NULL)
      if(strcmp(id,auxSym->id))
            return auxSym;
      auxSym = auxSym->next;
    return NULL;
}
/*struct sym{
    char id [33]; // identificador
    int dir; // direccion para la variable
    int tipo; // tipo como indice a la tabla de tipos
    int var; // tipo de variable
    ARG *args; // Lista de argumentos
    int num; // numero de argumentos
    SYM *next; // apuntador al siguiente simbolo
};*/
