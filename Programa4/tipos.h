#ifndef tipos_H
#define tipos_H
#include "cuadruplas.h"
#include "tabla_tipos.h"
/*
Recibe dos tipos, cada uno es su indice hacia la tabla de tipos en la cima de la pila
Si el primero es mas chico que el segundo genera la cuadrupla
para realizar la ampliacion del tipo.
Retorna la nueva direccion generada por la ampliacion
o la direccion original en caso de que no se realice
*/
char *ampliar(char *dir, int t1, int t2, CODE *c,TSTACK *Tpila);


char *reducir(char *dir, int t1, int t2, CODE *c,TSTACK *Tpila);


/*
Recibe dos tipos, cada uno es indice hacia la tabla de tipos en la cima de la pila
retorna el indice de mayor dimension
*/
int max(int t1, int t2,TSTACK *Tpila);


/*
Recibe dos tipos, cada uno es indice hacia la tabla de tipos en la cima de la pila
retorna el indice de menor dimension
*/
int min(int t1, int t2,TSTACK *Tpila);


/*
Recibe dos tipos, cada uno es indice hacia la tabla de tipos en la cima de la pila
retorna el indice de menor dimension si minMax es 0, o retorna el indice de mayor dimension si minMax es 1
*/
int buscaTamTipos(int t1, int t2,TYPTAB *top,int minMax);

#endif
