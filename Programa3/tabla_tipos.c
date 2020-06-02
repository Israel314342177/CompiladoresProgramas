#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_tipos.h"

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Agrega al final de la tabla de tipos un nuevo tipo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
solucionar el problema de como guardaba los tipos, ya que estos no se estaban ingresando correctamente
y mucha informacion se perdia, se cambio a que tail guardaria al nuevo tipo en vez de head
*/
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
     t->id=tt->num;
     tt->num = tt->num+1;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Deja vacia la tabla de tipos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
solucionar el problema en la condicion del while, pues antes se verificaba que tt fuera distinto de NULL,
ahora se verifica que el head de tt sea distinto de NULL, ademas de apuntar a NULL al tail de tt y reiniciar
en 0 el contador de elementos de la tabla
*/
void clear_type_tab(TYPTAB *tt){
   TYP *t;
    while(tt->head!=NULL){
        t = tt->head;
        tt->head=tt->head->next;
        t->next=NULL;
        finish_typ(t);
    }
    tt->tail=NULL;
    tt->num=0;
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Ejecuta un pop sobre la pila de tablas de tipos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
solucionar el problema con el orden en el que se reasignaban los punteros de next y top, en el segundo if
se agrego el return para que no causara problemas con las siguientes instrucciones
*/
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
    tt = aux->next;
    aux->next = NULL;
    ttt->top = aux;
    return tt;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Ingresa una tabla de tipos a la pila de tablas de tipos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
solucionar el problema con el orden en el que se reasignaban los punteros de top->next y top, ya que el push no
se estaba realizando correctamente y los punteros apuntaban a otra direccion a la esperada
*/
void push_tt(TSTACK *ttt, TYPTAB *tt){
    if(ttt->top == NULL)
    {
      ttt->top = ttt->tail = tt;
      return;
    }
    ttt->top->next = tt;
    ttt->top = tt;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para la pila de tipos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
se agregaron las asignaciones para que el top y el tail de la pila apunten a null
*/
TSTACK *init_type_tab_stack(){
    TSTACK *ttt;
    ttt = (TSTACK *)malloc(sizeof(TSTACK));
    ttt -> top = NULL;
    ttt -> tail = NULL;
    return ttt;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para una tabla de tipos e inserta los tipos nativos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se cambio la forma de asignar los nombres a los tipos mediante un strcpy y se agregaron
 las asignaciones para que el 
*/
TYPTAB *init_type_tab(){
    TYPTAB *tt;
    tt = (TYPTAB *)malloc(sizeof(TYPTAB));
    tt->num=0;
    
    TYP *t1 = init_type();
    //t1->id=0;
    strcpy(t1->nombre,"int");
    t1->tam=4;
    t1->tb.is_est=-1;
    t1->tb.tipo.SS=NULL;
    t1->tb.tipo.tipo=-1;
    append_type(tt,t1);

    TYP *t2 = init_type();
    strcpy(t2->nombre,"float");
    //t2->id=1;
    t2->tam=4;
    t2->tb.is_est=-1;
    t2->tb.tipo.SS=NULL;
    t2->tb.tipo.tipo=-1;
    append_type(tt,t2);
    t1->next=t2;
    t2->next=NULL;
    tt -> head = t1;
    tt -> tail = t2;
    return tt;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para un tipo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se asigno a NULL el puntero del siguiente del tipo creado
*/
TYP *init_type(){
   TYP *t;
   t = (TYP *)malloc(sizeof(TYP));
   t -> next = NULL;
   return t;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera la memoria para la pila
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se cambio la condicion del while, antes era tail!=NULL pero causaba problemas, 
 se cambio a que el tail de la pila fuera distinto de NULL para que terminara
*/
void finish_typ_tab_stack(TSTACK *ttt){
    TYPTAB *tt;
    while(ttt->tail!=NULL){
        tt = pop_tt(ttt);
        finish_typ_tab(tt);
    }
    free(ttt);
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera memoria para una tabla de tipos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se quito el codigo innecesario para que se reultilizara la funcion clear_type_tab
*/
void finish_typ_tab(TYPTAB *tt){
    clear_type_tab(tt);
    free(tt);
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: libera memoria para un tipo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se agrego la condicion de que T no fuera nulo para poder liberar la memoria
*/
void finish_typ(TYP *T){
   if(T!=NULL)
       free(T);
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Imprime en pantalla la tabla de tipos
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se agrego un condicional para elegir que valor se debia mostrar en el tipo base de cada tipo
*/
void print_Ttab(TYPTAB *tt){
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
      if(t->tb.is_est==0){
          tip=t->tb.tipo.tipo;
      }
      else tip=t->tb.is_est;
      printf("%3d     %8s        %4d         %3d",t->id,t->nombre,t->tam,tip);
      printf("\n");
      t = t->next;
    }
    printf("\n");
}
