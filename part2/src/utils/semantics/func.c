func *new_func(char *id, int type){
    func* res = (func*)malloc(sizeof(func));
    char* tmp_id = (char*)malloc(sizeof(id) + 1);
    
    strcpy(tmp_id, id);

    res->id = tmp_id;
    res->type = type;
    res->args = new_arg_arr();

    return res;
}

int func_type_to_int(char *type){
    if(strcmp(type, "TYPE BOOL") == 0){
        return TYPE_BOOL;
    }
    else if(strcmp(type, "TYPE VOID") == 0){
        return TYPE_VOID;
    }
    else if(strcmp(type, "TYPE REAL") == 0){
        return TYPE_REAL;
    }
    else if(strcmp(type, "TYPE INT") == 0){
        return TYPE_INT;
    }
    else if(strcmp(type, "TYPE CHAR") == 0){
        return TYPE_CHAR;
    }
    return -1;
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
    struct node *ret_tree = NULL;//tree_find(tree, "RET");
    int is_void = f->type == TYPE_VOID;

    if(!ret_tree && !is_void){
        printf("func: %s has no return\n", f->id);
        exit(1);
    }else if(!is_void){
        //got return TODO: validate type
        printf("ret - %s, t: %d\n", ret_tree->first->token, f->type);
    }
}

void print_func(struct func *f){
    printf("id:%s\ttype:%d\n", f->id, f->type);
    // printf("args:\n");
    // print_arg_arr(f->args);
}