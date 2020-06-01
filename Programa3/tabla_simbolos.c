#include<stdio.h>
#include "tabla_simbolos.h"

//Agrega al final de la tabla un nuevo simbolo
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

 // Deja vacia la tabla
void clear_sym_tab(SYMTAB *SS){
    SYM *S;
    while(SS!=NULL){
        S = SS->head;
        SS->head=SS->head->next;
        S->next=NULL;
        finish_typ(S);
    }
}

// Ejecuta un pop sobre la pila de tablas de simbolos
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

// Ingresa una tabla a la pila de tablas de simbolos
void push_st (SSTACK *SSS, SYMTAB *SS){
    if(SSS->top == NULL)
    {
      SSS->top = SSS->tail = SS;
      return;
    }
    SSS->tail->next = SS;
    SSS->tail = SS;
}

// Reserva memoria para la pila
SSTACK *init_sym_tab_stack(){
    SSTACK *SSS;
    SSS = (SSTACK *)malloc(sizeof(SSTACK));
    SSS -> top = NULL;
    SSS -> tail = NULL;
    return SSS;
}

 // Reserva memoria para una tabla de simbolos vacia
SYMTAB *init_sym_tab(){
    SYMTAB *SS;
    SS = (SYMTAB *)malloc(sizeof(SYMTAB));
    SS -> head = NULL;
    SS -> tail = NULL;
    SS -> num = 0;
    SS -> next = NULL;
    return SS;
}

 
SYM *init_sym(){ // Reserva memoria para un simbolo vacio
   SYM *S;
   S = (SYM *)malloc(sizeof(SYM));
   S -> next = NULL;
   return S;
}

void finish_sym_tab_stack(SSTACK *SSS){// Libera la memoria para la pila
    SYMTAB *SS;
    while(SSS!=NULL){
        SS = pop_tt(SSS);
        finish_typ_tab(SS);
    }
    free(SSS);
}


void finish_sym_tab(SYMTAB *SS){// Libera memoria para una tabla de simbolos
    clear_type_tab(SS);
    free(SS);
}


void finish_sym (SYM *S){ // libera memoria para un simbolo vacio
    if(S!=NULL)
       free(S);
}


void print_tab(SYMTAB *SS){// Imprime en pantalla la tabla de simbolos
    SYM *S;
    int sim;
    printf("\n");
    if(SS->head == NULL)
    {
     printf("\n --- TABLA VACIA (NO HAY SIMBOLOS). ---\n\n");
     return;
    }
    printf("\tTABLA DE SIMBOLOS:\n");
    printf(" Pos    id        Dir        Tipo        Var      Args     Num\n");
    S = SS->head;
    while(S!=NULL)
    {
      //imprimirSimbolo(S);
      printf(" %d     %s        %d        %d          %d        %d      ",S->num,S->id,S->dir,S->tipo,S->var,S->args->arg,S->num);
      printf("\n");
      S = S->next;
    }
    printf("\n");
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