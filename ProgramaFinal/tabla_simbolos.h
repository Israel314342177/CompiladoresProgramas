#ifndef SYMTAB_H
#define SYMTAB_H
#include <stdbool.h>
#include "DATOS.h"

void append_sym(SYMTAB *SS, SYM *S); //Agrega al final de la tabla un nuevo simbolo
void clear_sym_tab(SYMTAB *SS); // Deja vacia la tabla

SYMTAB *pop_st(SSTACK *SSS); // Ejecuta un pop sobre la pila de tablas de simbolos
void push_st(SSTACK *SSS, SYMTAB *SS); // Ingresa una tabla a la pila de tablas de simbolos
SSTACK *init_sym_tab_stack(); // Reserva memoria para la pila
SYMTAB *init_sym_tab(); // Reserva memoria para una tabla de simbolos vacia
SYM *init_sym(); // Reserva memoria para un simbolo vacio

void finish_sym_tab_stack(SSTACK *SSS); // Libera la memoria para la pila
void finish_sym_tab(SYMTAB *SS); // Libera memoria para una tabla de simbolos
void finish_sym(SYM *S); // libera memoria para un simbolo vacio
void print_Stab(SYMTAB *SS); // Imprime en pantalla la tabla de simbolos

SYM *search_sym_tab_stack(SSTACK *SSS,char *id); // Busca un simbolo en la pila de tablas de simbolos
SYM *search_sym_tab(SYMTAB *SS,char *id); // Busca un simbolo en una tabla de simbolos
#endif