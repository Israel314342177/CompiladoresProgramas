#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cuadruplas.h"

/*
 Fecha: 05/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Agrega una cuadrupla al final de la lista
 Modifico: Pachuca Cortes Santiago Emilio el 06/06/2020
 Modificacion:

*/
void append_quad(CODE *C, QUAD *cd){
    if(C->head == NULL)
     {
       C->head = C->tail = cd;
     }
   else
     {
      C->tail->next = cd;
      C->tail = cd;
     }
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para una cuadrupla
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:

*/
QUAD *init_quad(){
    QUAD *Q;
    Q = (QUAD *)malloc(sizeof(QUAD));
    Q -> next = NULL;
    Q -> arg1 = strcpy(Q -> arg1,"");
    Q -> arg2 = strcpy(Q -> arg2,"");
    Q -> op = strcpy(Q -> op,"");
    Q -> res = strcpy(Q -> res,"");
    return Q;
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para el codigo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 
*/
CODE *init_code(){
    CODE *C;
    C = (CODE *)malloc(sizeof(CODE));
    C -> head = NULL;
    C -> tail = NULL;
    return C;
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
void finish_quad(QUAD *c){
    c -> next = NULL;
    free(c);
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera la memoria de la lista ligada del codigo
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:

*/
void finish_code(CODE *c){
    QUAD *Q;
    while(c->head!=NULL){
        Q = c->head;
        c->head = c->head->next;
        finish_quad(Q);
    }
    free(c);
}