#ifndef estructuras_H
#define estructuras_H

typedef struct tipo TIPO;
struct tipo{
    int tipo;
    TIPO *next;
};

typedef struct lista_t{
    TIPO *head;
    TIPO *tail;
    int num;
}list;

list *nuevaLista();// Reserva memoria para una lista de tipos
void finish_list(list *l);// Libera la memoria de una lista de tipos
TIPO *init_tipo(int tipo);// Reserva memoria para un tipo
void finish_tipo(TIPO *t);// Libera la memoria de un tipo
void append_tipo(list *l, TIPO *t);// Agrega un tipo al final de la lista de tipos

#endif