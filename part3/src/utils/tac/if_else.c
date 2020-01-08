void cg_if_else(struct node *tree){
    labels_if_else(tree);
    gencode_if_else(tree);
}

void labels_if_else(struct node *tree){
    add_true_label(tree->first, freshLabel());
    // handle true body
    cg_handle_children(tree->second);
    // cpy code from block children
    cpy_code(tree->second);

    add_false_label(tree->first, freshLabel());
    // handle true body
    cg_handle_children(tree->third);
    // cpy code from block children
    cpy_code(tree->third);

    // handle bool expression
    cg_bool_exp(tree->first);

    // add next labels
    add_next(tree, freshLabel());
    add_next(tree->first, tree->next);
    add_next(tree->second, tree->next);
}

void gencode_if_else(struct node *tree){
    struct node *f = tree->first, *s = tree->second;
    char *code;
    asprintf(&code,"%s%s: %s\tgoto %s\n%s: %s\n%s:",tree->first->code, tree->first->trueLabel, tree->second->code, tree->next, tree->first->falseLabel, tree->third->code, tree->next);
    add_code(tree, code);
}