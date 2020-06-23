#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "backpatch.h"

/*
 Fecha: 05/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Agrega un nodo indice al final de la lista
 Modifico: Pachuca Cortes Santiago Emilio el 06/06/2020
 Modificacion:

*/
void append_index(LINDEX *l, INDEX *i){
    if(l->head == NULL)
     {
       l->head = l->tail = i;
     }
   else
     {
      l->tail->next = i;
      l->tail = i;
     }
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para un nodo de indice
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:

*/
INDEX *init_index(){
    INDEX *i;
    i = (INDEX *)malloc(sizeof(INDEX));
    i -> next = NULL;
    i -> indice = strcpy(i -> indice,"");
    return i;
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reserva memoria para la lista de indices e inserta el primero
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 
*/
LINDEX *init_list_index(INDEX *i){
    LINDEX *l;
    l = (LINDEX *)malloc(sizeof(LINDEX));
    l -> head = l -> tail = i;
    return l;
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera la memoria de un nodo indice
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:
 se cambio la condicion del while, antes era ttt!=NULL pero causaba problemas, 
 se cambio a que el tail de la pila fuera distinto de NULL para que terminara
*/
void finish_index(INDEX *i){
    i -> next = NULL;
    free(i);
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Libera la memoria de la lista y de todos los nodos dentro
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:

*/
void finish_list_index(LINDEX *l){
    INDEX *i;
    while(l->head!=NULL){
        i = l->head;
        l->head = l->head->next;
        finish_index(i);
    }
    free(l);
}



/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Retorna una lista ligada de la concatenacion de l1 con l2
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:

*/
LINDEX *combinar(LINDEX *l1,LINDEX *l2){
    l1->tail->next = l2->head;
    l1->tail = l2->tail;
    return l1;
}


/*
 Fecha: 30/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Reemplaza label en cada aparicion de un indice en las
 cuadruplas del codigo c
 Modifico: Pachuca Cortes Santiago Emilio el 31/05/2020
 Modificacion:

*/
//Reemplaza label en cada aparicion de un indice en las
//cuadruplas del codigo c
void backpatch(CODE *c, LINDEX *l, char *label){
    QUAD *Q;
    INDEX *I;
    Q = c->head;
    while(Q!=NULL){
        I = l->head;
        while(I!=NULL){
            if(strcmp(Q->res,I->indice)==0){
                Q->res = strcpy(Q->res,label);
            }
            I = I->next;
        }
        Q = Q->next;
    }
}
/*
typedef struct index INDEX;
struct index{
    char *indice;
    INDEX *next;
};

typedef struct list_index{
    INDEX *head;
    INDEX *tail;
} LINDEX;
*/