#define NUM_OF_IGNOORES 3

void handle_token(struct node* tree){
    char *token = tree->token;

    if(strcmp(token, "CODE") == 0){
        main_stack = new_code_stack();
        cs_push(main_stack, new_sym_table());
        handle_children(tree);
    }else if(strcmp(token, "FUNCTION") == 0){
        ast_to_func(tree);
        print_cs(main_stack);
    }else if(strcmp(token, "BLOCK") == 0){
        // handle new block
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

void apply_semantics(struct node* tree){
    handle_token(tree);
}

void handle_code_block(struct node* tree, struct func *f){
    
}