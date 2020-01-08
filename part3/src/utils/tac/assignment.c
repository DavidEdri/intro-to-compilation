void cg_assignment(struct node *tree){
    struct node *id = tree->first, *exp = tree->second;
    char *code, *token = tree->token;

    if(strcmp(token,"STRCHAR") == 0){
        cg_assign_strchar(tree);
    }else{
        // handle expression
        cg_expression(exp);

        // add assignment to code
        asprintf(&code,"%s\t%s = %s",exp->code,id->token, exp->var);
        add_code(tree, code);
    }
}

void cg_assign_strchar(struct node *tree){
    struct node *id = tree->first, *exp = tree->second;
    char *code;

    // handle expression
    cg_expression(exp);
    
    // add assignment 
    asprintf(&code,"%s\t%s = *%s\n",exp->code, id->token, exp->var);
    add_code(tree, code);
}