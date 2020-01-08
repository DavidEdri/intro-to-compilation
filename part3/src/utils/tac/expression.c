int is_additive_exp(char *token){
    return  strcmp(token, "+") == 0 ||
            strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 ||
            strcmp(token, "/") == 0 ;
}

int is_bool_exp(char *token){
    return  strcmp(token, "||") == 0 ||
            strcmp(token, "&&") == 0 ||
            strcmp(token, ">") == 0 ||
            strcmp(token, "<") == 0 ||
            strcmp(token, ">=") == 0 ||
            strcmp(token, "<=") == 0 ||
            strcmp(token, "!=") == 0 ||
            strcmp(token, "==") == 0 ||
            strcmp(token, "NOT") == 0 
        ;
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
    }else if(strcmp(token, "STRCHAR") == 0){
        cg_strchar(tree);
    }else{
            add_var(tree, token);
            add_code(tree, "");
    }
}

void cg_strchar(struct node *tree){
    struct node *id = tree->first, *exp = tree->second;
    char *code;

    asprintf(&code, "&%s", id->token);
    add_code(id, code);
    add_var(id,freshVar());

    cg_expression(exp);

    add_var(tree,freshVar());

    asprintf(&code, "\t%s = %s\n%s\t%s = %s + %s\n", id->var, id->code, exp->code, tree->var, id->var, exp->var);
    add_code(tree, code);
}