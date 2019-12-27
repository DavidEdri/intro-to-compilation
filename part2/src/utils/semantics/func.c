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
    struct node *t = tree;
    int got_ret = tree_find(t, "RET") != NULL;
    int is_void = f->type == TYPE_VOID;

    if(!got_ret && !is_void){
        printf("func: %s has no return\n", f->id);
        exit(1);
    }

    if(is_void && got_ret){
        printf("void function cannot have return statement in function: %s \n", f->id);
        exit(1);
    }

    if(got_ret){
        validate_rets(t, f);
    }
}

void validate_ret(struct node *tree, struct func *f){
    int f_type = f->type;
    int is_void = f_type == TYPE_VOID;
    int ret_type;

    if(get_operand_type(tree->first) == TYPE_STR){
        printf("function: %s cannot return string\n", f->id);
        exit(1);
    }

    ret_type = get_expression_type(tree->first);

    if(ret_type != f_type){
        printf("function %s expected %s return type, but got %s type instead\n",f->id, type_to_str(f->type), type_to_str(ret_type));
        exit(1);
    }
}

void validate_rets(struct node *tree, struct func *f){
    if(strcmp(tree->token, "RET") == 0){
        validate_ret(tree, f);
    }else{
        if(strcmp(tree->token, "FUNCTION") != 0){
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
}

void print_func(struct func *f){
    printf("id:%s\ttype:%s\n", f->id, type_to_str(f->type));
    // printf("args:\n");
    // print_arg_arr(f->args);
}

int validate_func_call(struct node* t){
    char *f_id = t->first->token;
    struct sym_el *tmp = cs_find(main_stack,f_id);
    struct func* tmp_func = NULL;  
    int func_args_count , func_call_args_count, wrong_arg;
    struct node* tmp_tree = t;
    struct arg_arr * a_arr = new_arg_arr();

    if(!tmp){
        printf("%s is undefined\n",f_id);
        exit(1);
    }

    if(tmp->type!=TYPE_FUNC){ 
        printf("%s is not a function\n",f_id);
        exit(1);
    }
    tmp_func = tmp->f;

    func_args_count = num_of_args(tmp_func->args);
    func_call_args_count = count_tree_args(tmp_tree->second); 

    func_call_to_args(a_arr, tmp_tree->second);

    if(func_args_count != func_call_args_count){
        printf("in function: %s expected: %d arguments and got: %d\n", tmp_func->id, func_args_count, func_call_args_count);
        exit(1);
    }

    wrong_arg = comapre_args(a_arr, tmp_func->args);
    
    if(wrong_arg){
        struct arg *a_tmp1 = get_arg_by_index(a_arr, wrong_arg - 1);
        struct arg *a_tmp2 = get_arg_by_index(tmp_func->args, wrong_arg - 1);

        printf("wrong argument type in function call : %s\nin argument %s expected %s and got %s\n", tmp_func->id, a_tmp2->id, type_to_str(a_tmp2->type), type_to_str(a_tmp1->type));

        exit(1);
    }

    return tmp_func->type;
}