typedef struct args ARG;

struct args{
    int arg;
    ARG next;
};


typedef struct args{
    ARG *head;
    ARG *tail;
    int num; // numero de elementos en la lista
};


typedef struct sym SYM;

struct sym{
    char id [33]; // identificador
    int dir; // direccion para la variable
    int tipo; // tipo como indice a la tabla de tipos
    int var; // tipo de variable
    ARG *args; // Lista de argumentos
    int num; // numero de argumentos
    SYM *next; // apuntador al siguiente simbolo
};


typedef struct sym_tab SYMTAB;

struct sym_tab{
    SYM *head;
    SYM *tail;
    int num; //Numero de elementos en la tabla
    SYMTAB *next; // apuntador a la tabla siguiente
};


typedef struct sym_stack{
    SYMTAB *top;
    SYMTAB *tail;
}SSTACK;


typedef struct tipobase{
    int is_est; // 1: si es estructura 0: si es tipo simple -1: si no tiene tipo base
    union{
    TYPTAB *est;
    int tipo;
    }tipo;
}TB;


typedef struct type TYP;

struct type{
    int id;
    char nombre[12];
    TB tb;
    int tam;
    TYP *next; //apuntador al siguiente tipo en la tabla de tipos
};


typedef struct type_tab TYPTAB;

struct type_tab{
    TYP *head;
    TYP *tail;
    int num; //Contador de elementos en la tabla
    TYPTAB *next ; //apuntador a la tabla siguiente
};


typedef struct typ_stack{
    TYPTAB *top;
    TYPTAB *tail;
}TSTACK;


