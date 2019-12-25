code_stack *new_code_stack(){
    struct code_stack *res = (struct code_stack*)malloc(sizeof(struct code_stack));
    
    res->top = NULL;
    res->next = NULL;

    return res;
}

void cs_push(struct code_stack* cs, struct sym_table *item){
    struct code_stack *tmp = new_code_stack();
    
    if(!cs->top){
        cs->top = item;
        return;
    }

    tmp->top = cs->top;
    tmp->next = cs->next;

    cs->top = item;
    cs->next = tmp;
}

sym_table *cs_pop(struct code_stack* cs){
    struct code_stack *tmp = cs->next;

    if(!cs->top){
        printf("cannot pop empty stack\n");
        return NULL;
    }

    if(!tmp){
        // only 1 item
        cs->top = NULL;
        return NULL;
    }

    cs->next = tmp->next;
    cs->top = tmp->top;

    return tmp->top;
}

sym_el *cs_find(struct code_stack *cs, char *id){
    struct code_stack *current = cs;
    struct sym_el *res = NULL;

    while(current){
        res = st_find(current->top, id);
        if(res){
            return res;
        }
        current = current->next;
    }

    return res;
}

void print_cs(struct code_stack* cs){
    struct code_stack *tmp_cs = cs;
    struct sym_table *tmp_st = NULL;

    printf("cs - \n");
    while(tmp_cs){
        tmp_st = tmp_cs->top;
        printf("\t");
        print_st(tmp_st);
        tmp_cs = tmp_cs->next;
    }
}