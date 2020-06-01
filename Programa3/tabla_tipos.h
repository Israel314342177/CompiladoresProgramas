#ifndef TYPTAB_H
#define TYPTAB_H
#include "DATOS.h"

void append_type(TYPTAB *tt, TYP *t); //Agrega al final de la tabla un nuevo tipo
void clear_type_tab(TYPTAB *tt); // Deja vacia la tabla

TYPTAB pop_st(TSTACK *ttt); // Ejecuta un pop sobre la pila de tablas de tipos
void push_st(TSTACK *ttt, TYPTAB *tt); // Ingresa una tabla a la pila de tablas de tipos

TSTACK *init_type_tab_stack(); // Reserva memoria para la pila
TYPTAB *init_type_tab(); // Reserva memoria para una tabla de tipos e inserta los tipos nativos
TYP *init_type(); // Reserva memoria para un tipo

void finish_sym_tab_stack(SSTACK *ttt); // Libera la memoria para la pila
void finish_sym_tab(TYPTAB *tt); // Libera memoria para una tabla de tipos
void finish_sym(TYP *t) ; // libera memoria para un tipo
void print_tab(TYPTAB *tt); // Imprime en pantalla la tabla de tipos

#endif