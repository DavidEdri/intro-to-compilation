void cg_assignment(struct node *tree){
    struct node *id = tree->first, *exp = tree->second;
    char *code, *token = tree->token;

    // handle expression
    cg_expression(exp);
    cg_expression(id);

    // add assignment to code
    asprintf(&code,"%s%s\t%s%s = %s%s\n",
        exp->code, // expression temp vars
        strcmp(id->code, "") != 0 ? id->code : "", // if assigning to strchar(x[i] = 'a')
        strcmp(id->token, "STRCHAR") == 0 ? "*" : "", // if assigning to strchar(x[i] = 'a')
        id->var, // assignment target
        strcmp(exp->token, "STRCHAR") == 0 ? "*" : "", // if assiging from strchar (a = x[i])
        exp->var); // last var used on expression
    add_code(tree, code);
    
}
