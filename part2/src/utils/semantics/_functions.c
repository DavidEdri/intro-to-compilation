void apply_semantics(struct node* tree){
    handle_token(tree);
    print_cs(main_stack);
    cs_pop(main_stack);
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
        // handle new block
    }else if(strcmp(token, "RET") == 0){
        // skip
    }else if(strcmp(token, "FUNCTION-CALL") == 0){
        validate_func_call(tree);
    }else if(strcmp(token, "INT") == 0
            || strcmp(token, "REAL") == 0
            || strcmp(token, "BOOL") == 0
            || strcmp(token, "CHAR") == 0
        ){
        validate_var_decleration(tree, var_type_to_int(token));
    }else{
        printf("unsuported token : %s\n", token);
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
    cs_pop(main_stack);
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
