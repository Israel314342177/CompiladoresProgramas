#include<stdio.h>
#include "tabla_tipos.h"
#include "tabla_simbolos.h"
#include "DATOS.h"

/*
 Fecha: 25/05/2020
 Autor(es): Martínez Martínez Brayan Eduardo
 Pachuca Cortes Santiago Emilio
 Descripción: Función main para comunicarse con el analizador léxico y con el sintáctico
 */
    /*En main.c escribir c´odigo donde se hagan pruebas con datos para
    *crear una pila
    *insertar dos tablas
    *llenar con tres elementos la tabla de la cima e imprimirla
    sacar de la pila
    liberar la memora de la pila,
    tanto para los tipos como para los s´ımbolos.*/
void menu();
void acciones(int accion,int simTip);
void autom();

int main(){
    printf(" Operaciones con tablas de simbolos y tipos\n");
    int opTS,opA;
    do{
        printf("\t --- Menu principal: ---\n\n");
        printf(" [0] TERMINAR (SALIR).\n");
        printf(" [1] PRUEBAS CON SIMBOLOS (PRUEBA MANUAL).\n");
        printf(" [2] PRUEBAS CON TIPOS (PRUEBA MANUAL).\n");
        printf(" [3] PRUEBA AUTOMATICA.\n");
        fflush(stdin);
        scanf("%d",opTS);
        if(opTS==1 || opTS==2){ 
            if(opTS==1) printf("\n Pruebas con Simbolos.\n");
            else printf("\n Pruebas con Tipos.\n");
            menu();
            fflush(stdin);
            scanf("%d",opA);
            acciones(opA,opTS);
        }
        else if(opTS==3){
            printf("\n Prueba Automatica con Simbolos y Tipos.\n");
            autom();
        }
    }while(opTS!=0);
    return 0;
}


void menu()
{
    printf(" [0] TERMINAR (SALIR).\n");
    printf(" [1] CREAR UNA PILA.\n");
    printf(" [2] LLENAR TABLA DE LA CIMA DE LA PILA.\n");
    printf(" [3] SACAR TABLA (POP) DE LA PILA.\n");
    printf(" [4] LIBERAR MEMORIA DE LA PILA.\n");
    printf("\n  Elija una opcion:  ");
}

void acciones(int accion,int simTip){
    switch(accion){
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            printf(" Esa opcion no existe\n\n");
            break;
    }
}

void autom(){
    tipos();
    simbolos();
}

void tipos(){
    TSTACK *pila;
    TYPTAB *tabla1,*tabla2,*aux;
    TYP *tipo1,*tipo2,*tipo3;
    printf("\n Pruebas con Tipos.\n");
    
    printf(" Crear una pila:\n");
    pila = init_type_tab_stack(); // Reserva memoria para la pila
    printf("... La pila de tablas de tipos ha sido creada.\n");
    
    printf(" Insertar dos tablas a la pila:\n");
    tabla1 = init_type_tab(); // Reserva memoria para una tabla de tipos e inserta los tipos nativos
    printf("... Tabla 1 ha sido creada.\n");
    tabla2 = init_type_tab(); // Reserva memoria para una tabla de tipos e inserta los tipos nativos
    printf("... Tabla 2 ha sido creada.\n");
    push_tt(pila,tabla1); // Ingresa una tabla a la pila de tablas de tipos
    printf("... Tabla 1 ha sido agregada a la pila.\n");
    push_tt(pila,tabla2); // Ingresa una tabla a la pila de tablas de tipos
    printf("... Tabla 2 ha sido agregada a la pila.\n");

    printf(" Llenar con tres elementos la tabla de la cima de la pila:\n");
    tipo1 = init_type(); // Reserva memoria para un tipo
    tipo1->nombre[0] = "array";
    tipo1->tam=12;
    tipo1->tb.is_est=0;
    tipo1->tb.tipo.tipo=0;
    append_type(pila->top,tipo1); //Agrega al final de la tabla un nuevo tipo
    printf("... El primer tipo se ha agregado a la tabla de la cima de la pila.\n");
    tipo2 = init_type(); // Reserva memoria para un tipo
    tipo2->nombre[0] = "array";
    tipo2->tam=48;
    tipo1->tb.is_est=0;
    tipo1->tb.tipo.tipo=2;
    append_type(pila->top,tipo2); //Agrega al final de la tabla un nuevo tipo
    printf("... El segundo tipo se ha agregado a la tabla de la cima de la pila.\n");
    tipo3 = init_type(); // Reserva memoria para un tipo
    tipo3->nombre[0] = "array";
    tipo3->tam=240;
    tipo3->tb.is_est=0;
    tipo3->tb.tipo.tipo=3;
    append_type(pila->top,tipo3); //Agrega al final de la tabla un nuevo tipo
    printf("... El tercer tipo se ha agregado a la tabla de la cima de la pila.\n");
    printf(" Imprimir la tabla.\n");
    printf(" Tabla de tipos de la cima de la pila:\n");
    print_Ttab(pila->top); // Imprime en pantalla la tabla de tipos

    printf(" Sacar la tabla de la pila.\n");
    aux = pop_tt(pila); // Ejecuta un pop sobre la pila de tablas de tipos
    printf("... La tabla ha sido extraida de la pila.\n");
    finish_typ_tab(aux); // Libera memoria para una tabla de tipos

    printf(" Liberar la memoria de la pila:\n");
    finish_typ_tab_stack(pila); // Libera la memoria para la pila
    printf("... La memoria de la pila ha sido liberada.\n");
    
    finish_typ_tab(tabla2); // Libera memoria para una tabla de tipos
}

void simbolos(){
    SSTACK *pila;
    SYMTAB *tabla1,*tabla2,*aux;
    SYM *simb1,*simb2,*simb3;
    printf("\n Pruebas con Simbolos.\n");
    printf(" Crear una pila:\n");
    pila = init_sym_tab_stack(); // Reserva memoria para la pila
    printf("... La pila de tablas de tipos ha sido creada.\n");
    
    printf(" Insertar dos tablas a la pila:\n");
    tabla1 = init_sym_tab(); // Reserva memoria para una tabla de simbolos vacia
    printf("... Tabla 1 ha sido creada.\n");
    tabla2 = init_sym_tab(); // Reserva memoria para una tabla de simbolos vacia
    printf("... Tabla 2 ha sido creada.\n");
    push_st(pila,tabla1); // Ingresa una tabla a la pila de tablas de simbolos
    printf("... Tabla 1 ha sido agregada a la pila.\n");
    push_st(pila,tabla2); // Ingresa una tabla a la pila de tablas de simbolos
    printf("... Tabla 2 ha sido agregada a la pila.\n");

    printf(" Llenar con tres elementos la tabla de la cima de la pila:\n");
    simb1 = init_sym(); // Reserva memoria para un simbolo vacio
    simb1->id[0] = "x";
    simb1->dir=0;//Calcular
    simb1->tipo=0;
    simb1->var=0;
    simb1->num=0;
    append_sym(pila->top,simb1);//Agrega al final de la tabla un nuevo simbolo
    printf("... El primer simbolo se ha agregado a la tabla de la cima de la pila.\n");
    simb2 = init_sym(); // Reserva memoria para un simbolo vacio
    simb2->id[0] = "y";
    simb2->dir=0;//Calcular
    simb2->tipo=0;
    simb2->var=0;
    simb2->num=0;
    append_sym(pila->top,simb2);//Agrega al final de la tabla un nuevo simbolo
    printf("... El segundo simbolo se ha agregado a la tabla de la cima de la pila.\n");
    simb3 = init_sym(); // Reserva memoria para un simbolo vacio
    simb3->id[0] = "z";
    simb3->dir=0;//Calcular
    simb3->tipo=0;
    simb3->var=0;
    simb3->num=0;
    append_sym(pila->top,simb3);//Agrega al final de la tabla un nuevo simbolo
    printf("... El tercer simbolo se ha agregado a la tabla de la cima de la pila.\n");
    printf(" Imprimir la tabla.\n");
    printf(" Tabla de simbolos de la cima de la pila:\n");
    print_Stab(pila->top); // Imprime en pantalla la tabla de simbolos

    printf(" Sacar la tabla de la pila.\n");
    aux = pop_st(pila); // Ejecuta un pop sobre la pila de tablas de simbolos
    printf("... La tabla ha sido extraida de la pila.\n");
    finish_sym_tab(aux);// Libera memoria para una tabla de simbolos

    printf(" Liberar la memoria de la pila:\n");
    finish_sym_tab_stack(pila); // Libera la memoria para la pila
    printf("... La memoria de la pila ha sido liberada.\n");
    
    finish_sym_tab(tabla2);// Libera memoria para una tabla de simbolos
}
