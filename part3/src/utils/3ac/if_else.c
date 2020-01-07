void cg_if_else(struct node *tree){
    // add true label
    add_true_label(tree->first, freshLabel());
    // handle true body labels
    cg_handle_children(tree->second);
    // add false label
    add_false_label(tree->first, freshLabel());
    // handle true body labels
    cg_handle_children(tree->third);
    // handle bool expression
    cg_bool_exp(tree->first);
    // add next label
    add_next(tree, freshLabel());
    add_next(tree->first, tree->next);
    add_next(tree->second, tree->next);

    char *code;
    asprintf(&code,"%s%s: %s\n\tgoto %s\n%s: %s\n%s:",tree->first->code, tree->first->trueLabel, get_first_code(tree->second)->code, tree->next, tree->first->falseLabel, get_first_code(tree->third)->code, tree->next);
    add_code(tree, code);
    
}