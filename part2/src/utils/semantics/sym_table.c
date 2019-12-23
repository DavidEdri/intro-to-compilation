typedef struct sym_table{
    struct sym_el *data;
    struct sym_table *next;
}sym_table;

sym_table * new_sym_table(){
    struct sym_table *res = (struct sym_table*)malloc(sizeof(struct sym_table));

    res->data = NULL;
    res->next = NULL;

    return res;
}

void st_add_item(struct sym_table *st, struct sym_el *data){
    struct sym_table *tmp = new_sym_table();
    struct sym_table *current = st;

    if(!st->data){
        st->data = data;
        return;
    }
    
    while(current->next){
        current = current->next;
    }

    tmp->data = data;
    tmp->next = NULL;
    current->next = tmp;
}

void print_st(struct sym_table *st){
    struct sym_table *current = st;

    printf("st - \n");
    while(current){
        printf("\t");
        print_se(current->data);
        current = current->next;
    }
}