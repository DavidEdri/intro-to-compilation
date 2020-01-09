void cg_function(struct node *tree){
    struct node *id = tree->first, *body = tree->fourth;
    char *code;

    cg_handle_token(body);

    asprintf(&code, "%s:\n\tBeginFunc\n%s\tEndFunc\n\n", id->token, body->code);
    add_code(tree, code);
}

void cg_main(struct node *tree){
    struct node *body = tree->first;
    char *code;

    cg_handle_token(body);

    asprintf(&code, "main:\n\tBeginFunc\n%s\tEndFunc\n\n", body->code);
    add_code(tree, code);
}

void cg_return(struct node *tree){
    struct node *exp = tree->first;
    char *code;

    cg_expression(exp);

    asprintf(&code, "%s%sReturn %s\n", exp->code, 
            strcmp(exp->code, "") != 0 ? "\n\t" : "", // if returning expression add \n \t
            exp->var
            );
    add_code(tree, code);
}