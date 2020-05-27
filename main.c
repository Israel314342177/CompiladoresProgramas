#include<stdio.h>
extern int yyparse();
extern FILE *yyin;

/*
 Fecha: 25/05/2020
 Autor: Martínez Martínez Brayan Eduardo
 Descripción: Función main para comunicarse con el analizador léxico y con el sintáctico
 */

int main(int argc,char** argv){
    FILE *f;
    if(argc<2){
        printf("Faltan argumentos\n");
        return -1;
    }
    f=fopen(argv[1],"r");
    if(!f){
        printf("El archivo %s no se puede abrir\n",argv[1]);
    }

    yyin=f;
    yyparse();
    fclose(yyin);
    return 0;
}
