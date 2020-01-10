void cg_while(struct node *tree){
    struct node *condition = tree->first, *body = tree->second;
    char *code;

    add_true_label(tree, freshLabel());

    cg_expression(condition);
    cg_handle_token(body);
    
    add_next(tree, freshLabel());

    asprintf(&code, "%s:\t%s\tifz %s Goto %s\n%s\tGoto %s\n%s:\t", tree->trueLabel, condition->code, condition->var, tree->next, body->code, tree->trueLabel, tree->next );
    add_code(tree, code);
}

void cg_for(struct node *tree){
    struct node *start = tree->first, *condition = tree->second, 
                *update = tree->third, *body = tree->fourth;
    char *code;

    check_for_assignment(start); // handle assignment / declare
    add_true_label(tree, freshLabel());
    cg_expression(condition);
    cg_handle_token(body);
    cg_assignment(update);
    add_next(tree, freshLabel());

    asprintf(&code, "%s%s:\t%s\tifz Goto %s\n%s%s\tGoto %s\n%s:\t", start->code, tree->trueLabel, condition->code, tree->next, body->code, update->code,tree->trueLabel, tree->next);
    add_code(tree, code);
}

void cg_do_while(struct node *tree){
    struct node *condition = tree->first, *body = tree->second;
    char *code;

    add_true_label(tree, freshLabel());
    cg_handle_token(body);
    cg_expression(condition);
    add_next(tree, freshLabel());

    asprintf(&code, "%s:\t%s%s\tifz %s Goto %s\n\tGoto %s\n%s:", tree->trueLabel, body->code, condition->code, condition->var, tree->next, tree->trueLabel, tree->next);
    add_code(tree, code);
}