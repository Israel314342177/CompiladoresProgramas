#include <stdio.h>
#include "tipos.h"
#include <string.h>

/*
 Fecha: 10/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción:
    Recibe dos tipos, cada uno es su indice hacia la tabla de tipos en la cima de la pila
    Si el primero es mas chico que el segundo genera la cuadrupla
    para realizar la ampliacion del tipo.
    Retorna la nueva direccion generada por la ampliacion
    o la direccion original en caso de que no se realice
 Modifico: Carrillo Molina Israel el 11/06/2020
 Modificacion: cambiar la llamada a min
*/
char *ampliar(char *dir, int t1, int t2, CODE *c,TSTACK *Tpila){
    if(t1==min(t1,t2,Tpila)){
		QUAD *newQuad = init_quad();
        strcpy(newQuad->op,strcat(strcat("(",buscaTipo(t1,Tpila->top)),")"));
        strcpy(newQuad->arg1,"");
        strcpy(newQuad->arg2,dir);
        append_quad(c,newQuad);
		return newQuad->res;  //Retorna la nueva direccion generada por la ampliacion
	}
	else{
		return dir; //Retorna la dirección original
	}
}


/*
 Fecha: 10/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción:
    Recibe dos tipos , cada uno es su indice hacia la tabla de tipos en la cima de la pila
    Si el primero es mas grande que el segundo genera la cuadrupla
    para realizar la reduccion del tipo.
    Retorna la nueva direccion generada por la reduccion
    o la direccion original en caso de que no se realice
 Modifico: Carrillo Molina Israel el 11/06/2020
 Modificacion: cambiar la llamada a max
*/
char *reducir(char *dir, int t1, int t2, CODE *c,TSTACK *Tpila){
    if(t1==max(t1,t2,Tpila)){
		QUAD *newQuad = init_quad();
        strcpy(newQuad->op,strcat(strcat("(",buscaTipo(t1,Tpila->top)),")"));
        strcpy(newQuad->arg1,"");
        strcpy(newQuad->arg2,dir);
        append_quad(c,newQuad);
		return newQuad->res;  //Retorna la nueva direccion generada por la ampliacion
	}
	else{
		return dir; //Retorna la dirección original
	}
}


/*
 Fecha: 10/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción:
    Recibe dos tipos, cada uno es indice hacia la tabla de tipos en la cima de la pila
    retorna el indice de mayor dimension
Modifico: Carrillo Molina Israel el 11/06/2020
 Modificacion: cambiar la llamada a buscaTamTipos
*/
int max(int t1, int t2,TSTACK *Tpila){
    return buscaTamTipos(t1,t2,Tpila->top,1);
}


/*
 Fecha: 10/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción:
    Recibe dos tipos, cada uno es indice hacia la tabla de tipos en la cima de la pila
    retorna el indice de menor dimension
Modifico: Carrillo Molina Israel el 11/06/2020
 Modificacion: cambiar la llamada a buscaTamTipos
*/
int min(int t1, int t2,TSTACK *Tpila){
    return buscaTamTipos(t1,t2,Tpila->top,0);
}


/*
 Fecha: 10/06/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción:
    Recibe dos tipos, cada uno es indice hacia la tabla de tipos en la cima de la pila
    retorna el indice de menor dimension si minMax es 0,
    o retorna el indice de mayor dimension si minMax es 1
Modifico: Carrillo Molina Israel el 11/06/2020
 Modificacion: agregar las condiciones finales para que regrese el minimo o el maximo
*/
int buscaTamTipos(int t1, int t2,TYPTAB *top,int minMax){
    TYP *aux = top->head;
	int tam_t1 = -1;
	int tam_t2 = -1;
	
	while(aux!=NULL){
		if(aux->id == t1){
            tam_t1 = aux->tam;
            if(tam_t1>0 && tam_t2>0)
                break;
		}
        
        if(aux->id == t2){
            tam_t2 = aux->tam;
            if(tam_t1>0 && tam_t2>0)
                break;
		}
		aux = aux->next;
	}
	
	if(tam_t1>0 && tam_t2>0){
        if(tam_t1 > tam_t2){ //t1 es mayor a t2
            if(minMax==0) //Regresar el minimo
                return t2;
            else        //Regresar el maximo
                return t1;
        }
        else{            //t2 es mayor a t1
            if(minMax==0) //Regresar el minimo
                return t1;
            else        //Regresar el maximo
                return t2;
        }
    }
	else
		return 0;
}


char *buscaTipo(int t, TYPTAB *top){
    TYP *aux = top->head;
	char tipo;
	while(aux!=NULL){
		if(aux->id == t){
            return aux->nombre;
		}
		aux = aux->next;
	}
}