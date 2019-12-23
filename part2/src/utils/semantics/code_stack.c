typedef struct code_stack{
    struct sym_table *top;
    struct code_stack *next;
}code_stack;

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