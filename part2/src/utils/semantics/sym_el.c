

typedef struct sym_el{
    int type;
    struct func *f;
    struct var *v;
} sym_el;

sym_el *new_sym_el(){
    sym_el *res = (sym_el*)malloc(sizeof(sym_el));
    
    res->type = -1;
    res->f = NULL;
    res->v = NULL;

    return res;
}

void se_add_var(struct sym_el *s, struct var *v){
    s->type = TYPE_VAR;
    s->v = v;
}

void se_add_func(struct sym_el *s, struct func *f){
    s->type = TYPE_FUNC;
    s->f = f;
}

void print_se(struct sym_el *se){
        switch(se->type){
        case TYPE_FUNC:
            printf("se func - ");
            print_func(se->f);
            break;
        case TYPE_VAR:
            printf("se var - ");
            print_var(se->v);
            break;
        default:
            printf("unexpected type: %d in print_se func\n", se->type);
    }
}