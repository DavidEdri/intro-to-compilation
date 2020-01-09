void cg_if_else(struct node *tree){
    labels_if_else(tree);
    gencode_if_else(tree);
}

void labels_if_else(struct node *tree){
    //gen labels for condition
    condition_lables(tree->first);
    add_true_label(tree->first, freshLabel());

    // handle true body
    cg_handle_token(tree->second);
    // cpy code from block children
    cpy_code(tree->second);
    if(strcmp(tree->second->code, "") == 0) add_code(tree->second, "\n");

    add_false_label(tree->first, freshLabel());
    // codgen condition
    cg_condition(tree->first);

    // handle false body
    cg_handle_token(tree->third);
    // cpy code from block children
    cpy_code(tree->third);
    if(strcmp(tree->third->code, "") == 0 )  add_code(tree->third, "\n");

    // add next labels
    add_next(tree, freshLabel());
    add_next(tree->second, tree->next);
    add_next(tree->third, tree->next);
}

void gencode_if_else(struct node *tree){
    struct node *exp = tree->first, *t_true = tree->second, *t_false = tree->third;
    char *code;
    asprintf(&code,"%s%s:\t%s\tgoto %s\n%s:\t%s%s:\t",exp->code, exp->trueLabel, t_true->code, tree->next, exp->falseLabel, t_false->code, tree->next);
    add_code(tree, code);
}

void cg_if(struct node *tree){
    labels_if(tree);
    gencode_if(tree);
}

void labels_if(struct node *tree){
    //gen labels for condition
    condition_lables(tree->first);
    add_true_label(tree->first, freshLabel());

    // handle true body
    cg_handle_token(tree->second);
    // cpy code from block children
    cpy_code(tree->second);
    if(strcmp(tree->second->code, "") == 0) add_code(tree->second, "\n");
    // add next labels
    add_next(tree, freshLabel());
    add_next(tree->second, tree->next);
    add_false_label(tree->first, tree->next); 
    // handle condition
    cg_condition(tree->first);
}

void gencode_if(struct node *tree){
    struct node *exp = tree->first, *t_true = tree->second;
    char *code;
    asprintf(&code,"%s%s:\t%s%s:\t",exp->code, exp->trueLabel, t_true->code, exp->falseLabel);
    add_code(tree, code);
}