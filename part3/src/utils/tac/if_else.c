void cg_if_else(struct node *tree){
    struct node *condition = tree->first, *t_body = tree->second, *f_body = tree->third;
    char *token = tree->token, *code;
    
    cg_expression(condition);
    // handle true body
    cg_handle_token(t_body);
    // cpy code from block children
    if(should_cpy_code(t_body->token)) cpy_code(t_body);
    // if(strcmp(t_body->code, "") == 0) add_code(t_body, "\n");

    if(strcmp(token, "IF-ELSE") == 0){
        add_false_label(condition, freshLabel());
        cg_handle_token(f_body);
        // cpy code from block children
        if(should_cpy_code(f_body->token)) cpy_code(f_body);
        if(strcmp(f_body->code, "") == 0 )  add_code(f_body, "\n"); // if empty false body

        // add next labels
        add_next(tree, freshLabel());
        add_next(t_body, tree->next);
        add_next(f_body, tree->next);

        // codegen
        asprintf(&code, "%s\tifz %s Goto %s\n\t%s\tGoto %s\n%s:\t%s%s:\t",
                condition->code, condition->var, condition->falseLabel,
                t_body->code, t_body->next, 
                condition->falseLabel, f_body->code,
                tree->next
        );
        add_code(tree, code);
    }else{
        add_next(tree, freshLabel());
        add_next(t_body, tree->next);
        add_false_label(condition, tree->next);         

        // codegen if
        asprintf(&code, "%s\tifz %s Goto %s\n%s%s:\t",condition->code, condition->var, condition->falseLabel, t_body->code, tree->next);
        add_code(tree, code);
    }

}