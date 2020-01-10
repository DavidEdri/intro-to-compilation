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
        
        // if adding between number and id add id to temp var
        if_temp_needed(tree);

        add_var(tree, freshVar());

        asprintf(&code, "%s%s\t%s = %s %s %s\n", f->code, s->code, tree->var, f->var, token, s->var);
        add_code(tree, code);
    }else if(is_bool_exp(token) || strcmp(token, "TRUE") == 0|| strcmp(token, "FALSE") == 0){
        cg_bool_exp(tree);
    }else if(strcmp(token, "UMINUS") == 0){
        add_var(tree, freshVar());
        cg_expression(f);
        asprintf(&code, "\t%s%s = -%s\n", f->code, tree->var, f->var);
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

    asprintf(&code, "%s = %s\n%s\t%s = %s + %s\n", id->var, id->code, exp->code, tree->var, id->var, exp->var);
    add_code(tree, code);
}

void cg_bool_exp(struct node *tree){
    struct node *f = tree->first, *s = tree->second;
    char *token = tree->token, *code;
    
    /* opt 1
    condition_lables(tree);
    add_true_label(tree, freshLabel());
    add_false_label(tree, freshLabel()); 
    cg_condition(tree);
    add_next(tree, freshLabel()); 

    add_var(tree, freshVar());

    asprintf(&code,"%s%s:\t%s = 1\n\tgoto %s\n%s:\t%s = 0\n%s:", tree->code,tree->trueLabel,tree->var,tree->next,tree->falseLabel, tree->var, tree->next);

    add_code(tree, code);

    */

    // opt 2
    if(f){
        cg_bool_exp(f);
    }

    if(s){
        cg_bool_exp(s);
    }

    if(strcmp(token, "&&") == 0){
        add_var(tree, freshVar());
        asprintf(&code, "%s%s\t%s = %s && %s\n", f->code, s->code, tree->var, f->var, s->var);
        add_code(tree, code);
    }else if(strcmp(token, "||") == 0){
        add_var(tree, freshVar());
        asprintf(&code, "%s%s\t%s = %s || %s\n", f->code, s->code, tree->var, f->var, s->var);
        add_code(tree, code);
    }else if(is_relop(token)){
        // if adding between number and id add id to temp var
        if_temp_needed(tree);
        
        add_var(tree, freshVar());
        asprintf(&code, "%s%s\t%s = %s %s %s\n", f->code, s->code, tree->var, f->var, token, s->var);
        add_code(tree, code);      
    }else if(strcmp(token, "NOT") == 0){
        add_var(tree, freshVar());
        asprintf(&code, "%s\t%s = %s == 0\n", f->code, tree->var, f->var);
        add_code(tree, code);      
    }else if(strcmp(token, "TRUE") == 0){
        add_var(tree, freshVar());
        asprintf(&code, "\t%s = 1\n", tree->var);
        add_code(tree, code);      
    }else if(strcmp(token, "FALSE") == 0){
        add_var(tree, freshVar());
        asprintf(&code, "\t%s = 0\n", tree->var);
        add_code(tree, code);      
    }else{
        cg_expression(tree);
    }
}

int is_relop(char *t){
    return  strcmp(t, "==") == 0 ||
            strcmp(t, "!=") == 0 ||
            strcmp(t, "<") == 0 ||
            strcmp(t, ">") == 0 ||
            strcmp(t, "<=") == 0 ||
            strcmp(t, ">=") == 0 ;
}

void if_temp_needed(struct node *tree){
    struct node *f = tree->first, *s = tree->second;
    char *token = tree->token, *code;
    // if adding between number and id add id to temp var
    if(is_var(f->token) && !is_var(s->token) && !is_additive_exp(s->token)){
        char *val = s->var;
        add_var(s, freshVar());
        asprintf(&code, "\t%s = %s\n", s->var, val);
        add_code(s, code);
    }else if (!is_var(f->token) && is_var(s->token) && !is_additive_exp(f->token)){
        char *val = f->var;
        add_var(f, freshVar());
        asprintf(&code, "\t%s = %s\n", f->var, val);
        add_code(f, code);
    }
}