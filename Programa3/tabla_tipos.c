#include<stdio.h>
#include<stdlib.h>
#include "tabla_tipos.h"

 //Agrega al final de la tabla un nuevo tipo
void append_type(TYPTAB *tt, TYP *t){
    if(tt->head == NULL)
     {
       tt->head = tt->tail = t;
     }
   else
     {
      tt->tail->next = t;
      tt->tail = t;
     }
     tt->num = tt->num+1;
}

 // Deja vacia la tabla
void clear_type_tab(TYPTAB *tt){
   TYP *t;
    while(tt!=NULL){
        t = tt->head;
        tt->head=tt->head->next;
        t->next=NULL;
        finish_typ(t);
    }
}

 // Ejecuta un pop sobre la pila de tablas de tipos
TYPTAB *pop_tt(TSTACK *ttt){
    TYPTAB *tt,*aux;
    if(ttt->top == NULL) /* pila vacía */
        return NULL;

    if(ttt->top == ttt->tail) /* pila con un solo elemento */
    {
        tt = ttt->tail;
        ttt -> top = ttt->tail = NULL;
        return tt;
    }

    aux = ttt->tail;
    while(aux->next!=ttt->top)
      aux = aux->next;
    aux->next = NULL;
    tt = ttt->top;
    ttt->top = aux;
    return tt;
}

 // Ingresa una tabla a la pila de tablas de tipos
void push_tt(TSTACK *ttt, TYPTAB *tt){
    if(ttt->top == NULL)
    {
      ttt->top = ttt->tail = tt;
      return;
    }
    ttt->tail->next = tt;
    ttt->tail = tt;
}

 // Reserva memoria para la pila
TSTACK *init_type_tab_stack(){
    TSTACK *ttt;
    ttt = (TSTACK *)malloc(sizeof(TSTACK));
    ttt -> top = NULL;
    ttt -> tail = NULL;
    return ttt;
}

 // Reserva memoria para una tabla de tipos e inserta los tipos nativos
TYPTAB *init_type_tab(){
    TYPTAB *tt;
    tt = (TYPTAB *)malloc(sizeof(TYPTAB));
    
    TYP *t1 = init_type();
    t1->id=0;
    t1->nombre[0]="int";
    t1->tam=4;
    t1->tb.is_est=-1;
    t1->tb.tipo.est=tt;
    t1->tb.tipo.tipo=-1;
    append_type(tt,t1);

    TYP *t2 = init_type();
    t2->id=1;
    t2->nombre[0]="float";
    t2->tam=4;
    t2->tb.is_est=-1;
    t2->tb.tipo.est=tt;
    t2->tb.tipo.tipo=-1;
    append_type(tt,t2);
    t1->next=t2;
    t2->next=NULL;
    tt -> head = t1;
    tt -> tail = t2;
    tt -> num = 2;
    return tt;
}

// Reserva memoria para un tipo
TYP *init_type(){
   TYP *t;
   t = (TYP *)malloc(sizeof(TYP));
   t -> next = NULL;
   return t;
}


void finish_typ_tab_stack(SSTACK *ttt){// Libera la memoria para la pila
    TYPTAB *tt;
    while(ttt!=NULL){
        tt = pop_tt(ttt);
        finish_typ_tab(tt);
    }
    free(ttt);
}


void finish_typ_tab(TYPTAB *tt){ // Libera memoria para una tabla de tipos
    clear_type_tab(tt);
    free(tt);
}


void finish_typ(TYP *T){ // libera memoria para un tipo
   if(T!=NULL)
       free(T);
}


void print_tab(TYPTAB *tt){ // Imprime en pantalla la tabla de tipos
    TYP *t;
    int tip;
    printf("\n");
    if(tt->head == NULL)
    {
     printf("\n --- TABLA VACIA (NO HAY TIPOS). ---\n\n");
     return;
    }
    printf("\tTABLA DE TIPOS:\n");
    printf(" id        Nombre      Tamanio     Tipo Base\n");
    t = tt->head;
    while(t!=NULL)
    {
      //imprimirTipo(t);
      if(t->tb.is_est==1){
          t->tb.tipo.tipo;
      }
      else tip=t->tb.is_est;
      printf("%3d          %s         %d        %d",t->id,t->nombre,t->tam,tip);
      printf("\n");
      t = t->next;
    }
    printf("\n");
}