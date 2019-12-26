func *new_func(char *id, int type){
    func* res = (func*)malloc(sizeof(func));
    char* tmp_id = (char*)malloc(sizeof(id) + 1);
    
    strcpy(tmp_id, id);

    res->id = tmp_id;
    res->type = type;
    res->args = new_arg_arr();

    return res;
}

void ast_to_func(struct node* tree){
    char *id, *type;
    struct func *res = NULL;
    struct sym_el *tmp_se = NULL;
    struct sym_table *top = main_stack->top;

    id = tree->first->token;
    type = tree->third->token;

    if(st_is_declared(top, id)){
        printf("id: [%s] already declared in scope\n", id);
        exit(1);
    }
    
    res  = new_func(id, func_type_to_int(type));
    ast_to_args(res, tree->second);
    tmp_se = new_sym_el();
    se_add_func(tmp_se, res);
    st_add_item(top, tmp_se);

    handle_code_block(tree->fourth, res);

}

void validate_return(struct node *tree, struct func *f){
    int got_ret = tree_find(tree, "RET") != NULL;
    int is_void = f->type == TYPE_VOID;

    if(got_ret){
        validate_rets(tree, f);
    }

    if(!got_ret && !is_void){
        printf("func: %s has no return\n", f->id);
        exit(1);
    }
}

void validate_ret(struct node *tree, struct func *f){
    int f_type = f->type;
    int is_void = f_type == TYPE_VOID;

    if(is_void && strcmp(tree->first->token, "VOID") !=0){
        printf("func %s must return void\n", f->id);
        exit(1);
    }

    // TODO : validate types
}

void validate_rets(struct node *tree, struct func *f){
    if(strcmp(tree->token, "RET") == 0){
        validate_ret(tree, f);
    }else{
        if(tree->first){
            validate_rets(tree->first, f);
        }
        if(tree->second){
            validate_rets(tree->second, f);
        }
        if(tree->third){
            validate_rets(tree->third, f);
        }
        if(tree->fourth){
            validate_rets(tree->fourth, f);
        }
    }
}

void print_func(struct func *f){
    printf("id:%s\ttype:%d\n", f->id, f->type);
    // printf("args:\n");
    // print_arg_arr(f->args);
}

//handle exp as args
void validate_func_call(struct node* t){
    char *f_id = t->first->token;
    struct sym_el *tmp = cs_find(main_stack,f_id);
    struct func* tmp_func = tmp->f;  
    int func_args_count , func_call_args_count;
    struct node* tmp_tree = t;
    
    if(!tmp){
        printf("%s is undefined\n",f_id);
        exit(1);
    }

    if(tmp->type!=TYPE_FUNC){ 
        printf("%s is not a function\n",f_id);
        exit(1);
    }
    
    func_args_count = num_of_args(tmp_func->args);
    func_call_args_count = count_tree_args(tmp_tree); 
    
    if(func_args_count != func_call_args_count){
        printf("expected: %d arguments and got: %d\n",func_args_count, func_call_args_count);
        exit(1);
    }

    //check_args_types(t,tmp->args);
}