void apply_semantics(struct node* tree){
    handle_token(tree);
    // cs_pop(main_stack);
    print_cs(main_stack);
}

void handle_token(struct node* tree){
    char *token = tree->token;
    
    if(strcmp(token, "CODE") == 0){
        main_stack = new_code_stack();
        cs_push(main_stack, new_sym_table());
        handle_children(tree);
    }else if(strcmp(token, "FUNCTION") == 0){
        ast_to_func(tree);
    }else if(strcmp(token, "BLOCK") == 0){
        handle_code_block(tree, NULL);
    }else if(is_operator(token)){
        get_expression_type(tree);
    }else if(strcmp(token, "RET") == 0){
        // skip
    }else if(strcmp(token, "IF") == 0 || strcmp(token, "IF-ELSE") == 0 || strcmp(token, "WHILE") == 0 || strcmp(token, "DO-WHILE") == 0){
            validate_if(tree,token);
    }else if(strcmp(token, "FOR") == 0){
        validate_for(tree);
    }else if(is_arg_type(token)){
        validate_var_decleration(tree, var_type_to_int(token));
    }else if(strcmp(token, "=") == 0){
        validate_assignment(tree,0,0);
    }
    else if(strcmp(token, "STR") == 0){
        validate_str_declare(tree);
    }else if(strcmp(token, "DREF") == 0){
        validate_dref(tree);
    }else{
        if(strcmp(token, "") != 0){
            printf("unsuported token : %s\n", token);
        }
        handle_children(tree);
    }
}

void handle_children(struct node* tree){
if (tree->first)
    {
        handle_token(tree->first);
    }

    if (tree->second)
    {
        handle_token(tree->second);
    }

    if (tree->third)
    {
        handle_token(tree->third);
    }

    if (tree->fourth)
    {
        handle_token(tree->fourth);
    }
}


void handle_code_block(struct node* tree, struct func *f){
    cs_push(main_stack, new_sym_table());
   
    if(f){
        args_to_st(f);
        validate_return(tree, f);
    }
    
    handle_children(tree);
    // cs_pop(main_stack);
}

struct node *tree_find(struct node *tree, char *id){
    struct node *res = NULL;

    if(strcmp(tree->token, id) == 0){
        return tree;
    }else{
        if(tree->first){
            res = tree_find(tree->first, id);
        }
        if(tree->second){
            res = tree_find(tree->second, id);
        }
        if(tree->third){
            res = tree_find(tree->third, id);
        }
        if(tree->fourth){
            res = tree_find(tree->fourth, id);
        }
    }

    return res;
}


void validate_if(struct node* tree,char* token){
    int x;

    x=get_expression_type(tree->first);
    if(x!=TYPE_BOOL){
        printf(" %s condition must be of boolean type not %s\n",token,type_to_str(x));
        exit(1);
    }
   handle_children(tree);
}

void validate_for(struct node* tree){
   int x;

   validate_var_decleration(tree->first,var_type_to_int(tree->first->token));
    x=get_expression_type(tree->second);
    if(x!=TYPE_BOOL){
        printf(" FOR condition must be of boolean type not %s\n",type_to_str(x));
        exit(1);
    }
    validate_assignment(tree->third, 0, 0);

}

void validate_assignment(struct node* tree, int new_var, int new_var_type){
    char *id = tree->first->token;
    struct sym_el *id_el = cs_find(main_stack, id);
    int ltype, rtype;

    if(!id_el && !new_var){
        printf("%s is undefined\n", id);
        exit(1);
    }

    if(!new_var && id_el->type == TYPE_FUNC){
        printf("%s is a function, cannot assign to function\n", id);
        exit(1);
    }

    if(new_var){
        ltype = new_var_type;
    }else{
        ltype = se_get_type(id_el);
    }
    rtype = get_expression_type(tree->second);

    if( ltype != rtype && 
        !(ltype == TYPE_REAL && rtype == TYPE_INT) &&
        !(is_ptr(ltype) && rtype == TYPE_NULL)
      ){
        printf("%s is %s cannot assign %s\n", id, type_to_str(ltype), type_to_str(rtype));
        exit(1);
    }
}

void validate_dref(struct node *tree){
    int type = get_expression_type(tree->first);

    if(!is_ptr(type)){
        printf("operator * can only be used on pointers but used on %s\n", type_to_str(type));
        exit(1);
    }
}