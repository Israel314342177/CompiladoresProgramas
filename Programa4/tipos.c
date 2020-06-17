#include <stdio.h>
#include "tipos.h"

/*
Recibe dos tipos, cada uno es su indice hacia la tabla de tipos en la cima de la pila
Si el primero es mas chico que el segundo genera la cuadrupla
para realizar la ampliacion del tipo.
Retorna la nueva direccion generada por la ampliacion
o la direccion original en caso de que no se realice
*/
char *ampliar(char *dir, int t1, int t2, CODE *c){
    if (t1<t2){
        //cuadrupla;
    }
    else
        return dir;
}


/*
Recibe dos tipos , cada uno es su indice hacia la tabla de tipos en la cima de la pila
Si el primero es mas grande que el segundo genera la cuadrupla
para realizar la reduccion del tipo.
Retorna la nueva direccion generada por la reduccion
o la direccion original en caso de que no se realice
*/
// Reserva memoria para un nodo de indice
char *reducir(char *dir, int t1, int t2, CODE *c){
    if (t1>t2){
        //cuadrupla;
    }
    else
        return dir;
}


/*
Recibe dos tipos, cada uno es indice hacia la tabla de tipos en la cima de la pila
retorna el indice de mayor dimension
*/
int max(int t1, int t2){
    ;
}


/*
Recibe dos tipos, cada uno es indice hacia la tabla de tipos en la cima de la pila
retorna el indice de menor dimension
*/
int min(int t1, int t2){
    ;
}