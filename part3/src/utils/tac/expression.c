int is_additive_exp(char *token){
    return  strcmp(token, "+") == 0 ||
            strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 ||
            strcmp(token, "/") == 0 ;
}

void cg_expression(struct node *tree){
    struct node *f = tree->first, *s = tree->second;
    char *token = tree->token, *code;

    if(is_additive_exp(token)){
        if(f){
            cg_expression(f);
        }

        if(s){
            cg_expression(s);
        }

        add_var(tree, freshVar());
        asprintf(&code, "%s%s\t%s = %s %s %s\n", f->code, s->code, tree->var, f->var, token, s->var);
        add_code(tree, code);
    }else if(strcmp(token, "UMINUS") == 0){
        add_var(tree, freshVar());
        cg_expression(f);
        asprintf(&code, "\t%s%s = %s\n", f->code, tree->var, f->var);
        add_code(tree, code);
    }else if(is_operator(token)){
            printf("%s in cg_expresssion not handled yet\n", token);
            exit(1);
    }else{
            add_var(tree, token);
            add_code(tree, "");
    }
}