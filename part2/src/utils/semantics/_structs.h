typedef struct func{
    char *id;
    struct arg_arr *args;
    int type;
} func;

typedef struct arg {
    char *id;
    int type;
} arg;

typedef struct arg_arr {
    struct arg *data;
    struct arg_arr *next;
} arg_arr;

typedef struct var{
    char *id;
    char *val;
    int type;
} var;

typedef struct code_stack{
    struct sym_table *top;
    struct code_stack *next;
}code_stack;

typedef struct sym_el{
    int type;
    struct func *f;
    struct var *v;
} sym_el;

typedef struct sym_table{
    struct sym_el *data;
    struct sym_table *next;
}sym_table;