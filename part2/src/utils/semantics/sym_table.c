typedef struct sym_table{
    struct sym_el *data;
    struct sym_table *next;
}sym_table;

sym_table *new_sym_table(){
    struct sym_table *res = (struct sym_table*)malloc(sizeof(struct sym_table));

    res->data = NULL;
    res->next = NULL;

    return res;
}

int st_is_declared(struct sym_table *st, char *id){
    struct sym_table *current = st;

    while(current){
        if(strcmp(se_get_id(current->data), id) == 0){
            return 1;
        }
        
        current = current->next;
    }

    return 0;
}

void st_add_item(struct sym_table *st, struct sym_el *data){
    struct sym_table *tmp = new_sym_table();
    struct sym_table *current = st;
    char *data_id = se_get_id(data);

    if(!st->data){
        st->data = data;
        return;
    }

    if(st_is_declared(st, data_id)){
        printf("%s is already declared in current scope!\n", data_id);
        return;
    }
    
    while(current->next){
        current = current->next;
    }

    tmp->data = data;
    tmp->next = NULL;
    current->next = tmp;
}

sym_el *st_find(struct sym_table *st, char *id){
    struct sym_table *current = st;
    struct sym_el *res = NULL;

    while(current){
        if(se_cmp_id(current->data, id)){
            res = current->data;
            return res;
        }
        current = current->next;
    }

    return res;
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