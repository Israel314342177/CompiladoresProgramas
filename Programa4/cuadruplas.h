#ifndef cuadruplas_H
#define cuadruplas_H

typedef struct cuad QUAD;
struct cuad{
    char *op;
    char *arg1;
    char *arg2;
    char *res;
    QUAD *next;
};

typedef struct code{
    QUAD *head;
    QUAD *tail;
}CODE;

QUAD *init_quad();// Reserva memoria para una cuadrupla
void finish_quad(QUAD *c);// Libera la memoria de una cuadrupla
CODE *init_code();// Reserva memoria para el codigo
void finish_code(CODE *c);// Libera la memoria de la lista ligada del codigo
void append_quad(CODE *C, QUAD *cd);// Agrega una cuadrupla al final de la lista

#endif