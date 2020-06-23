#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"

/*
 Fecha: 05/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Agrega una cuadrupla al final de la lista
 Modifico: Pachuca Cortes Santiago Emilio el 06/06/2020
 Modificacion:
*/
void append_tipo(list *l, TIPO *t){
    if(l->head == NULL)
     {
       l->head = l->tail = t;
     }
   else
     {
      l->tail->next = t;
      l->tail = t;
     }
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para una cuadrupla
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:

*/
TIPO *init_tipo(int tipo){
    TIPO *t;
    t = (TIPO *)malloc(sizeof(TIPO));
    t -> next = NULL;
    t -> tipo = tipo;
    return t;
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para el codigo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 
*/
list *nuevaLista(){
    list *L;
    L = (list *)malloc(sizeof(list));
    L -> head = NULL;
    L -> tail = NULL;
    L -> num = 0;
    return L;
}

/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera la memoria de una cuadrupla
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se cambio la condicion del while, antes era ttt!=NULL pero causaba problemas, 
 se cambio a que el tail de la pila fuera distinto de NULL para que terminara
*/
void finish_tipo(TIPO *t){
    t -> next = NULL;
    free(t);
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera la memoria de la lista ligada del codigo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:

*/
void finish_list(list *l){
    TIPO *t;
    while(l->head!=NULL){
        t = l->head;
        l->head = l->head->next;
        finish_quad(t);
    }
    free(l);
}