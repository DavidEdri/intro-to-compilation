void cg_if_else(struct node *tree){
    labels_if_else(tree);
    gencode_if_else(tree);
}

void labels_if_else(struct node *tree){
    add_true_label(tree->first, freshLabel());
    // handle true body
    cg_handle_token(tree->second);
    // cpy code from block children
    cpy_code(tree->second);

    add_false_label(tree->first, freshLabel());
    // handle true body
    cg_handle_token(tree->third);
    // cpy code from block children
    cpy_code(tree->third);

    // handle bool expression
    cg_bool_exp(tree->first);

    // add next labels
    add_next(tree, freshLabel());
    add_next(tree->second, tree->next);
    add_next(tree->third, tree->next);
}

void gencode_if_else(struct node *tree){
    struct node *exp = tree->first, *t_true = tree->second, *t_false = tree->third;
    char *code;
    asprintf(&code,"%s%s: %s\tgoto %s\n%s: %s%s:",exp->code, exp->trueLabel, t_true->code, tree->next, exp->falseLabel, t_false->code, tree->next);
    add_code(tree, code);
}

void cg_if(struct node *tree){
    labels_if(tree);
    gencode_if(tree);
}

void labels_if(struct node *tree){
    add_true_label(tree->first, freshLabel());
    // handle true body
    cg_handle_token(tree->second);
    // cpy code from block children
    cpy_code(tree->second);
    // add next labels
    add_next(tree, freshLabel());
    add_next(tree->second, tree->next);
    add_false_label(tree->first, tree->next); 
    // handle bool expression
    cg_bool_exp(tree->first);
}

void gencode_if(struct node *tree){
    struct node *exp = tree->first, *t_true = tree->second;
    char *code;
    asprintf(&code,"%s%s: %s%s:",exp->code, exp->trueLabel, t_true->code, exp->falseLabel);
    add_code(tree, code);
}