void cg_assignment(struct node *tree){
    struct node *id = tree->first, *exp = tree->second;
    char *code, *token = tree->token;

    if(strcmp(exp->token, "FUNCTION-CALL") == 0){
        cg_func_call(exp, id->token);
        asprintf(&code, "%s\t%s = %s\n",exp->code, id->token, exp->var);
        add_code(tree, code);
    }else{
        // check if need to assign first to temp var
        if(!exp->first && strcmp(exp->token, "TRUE") != 0 && 
            strcmp(exp->token, "FALSE") != 0 && !is_var(exp->token) &&
            strcmp(exp->token, "CSNULL") != 0 && !is_var(exp->token) 
            ){
            char *val = exp->token;

            add_var(exp, freshVar());
            asprintf(&code, "\t%s = %s\n", exp->var, val);
            add_code(exp, code);
        }else{
            cg_expression(exp);
        }
        cg_expression(id);

        // add assignment to code
        asprintf(&code,"%s%s\t%s = %s\n",
            exp->code, // expression temp vars
            strcmp(id->code, "") != 0 ? id->code : "", // if assigning to strchar(x[i] = 'a')
            id->var, // assignment target
            exp->var); // last var used on expression
        add_code(tree, code);
    }
    
    
}

void check_for_assignment(struct node *tree){
    struct node *f = tree->first, *s = tree->second;
    char *token = tree->token;

    if(strcmp(token, "=") == 0){
        if(strcmp(f->token, "STRCHAR") == 0){
            // set id to be the token instead of STRCHAR
            struct node *tmp_first = mknode(f->first->token, NULL, NULL, NULL, NULL, -1),
                        *tmp = mknode("=", tmp_first, s, NULL, NULL, -1);

            cg_assignment(tmp);
            add_code(tree, tmp->code); // copy the code from the tmp node
        }else{
            cg_assignment(tree);
        }
        // printtree(tree,0,1);
    }else{
        if(f){
            check_for_assignment(f);
        }

        if(s){
            check_for_assignment(s);
        }
        cpy_code(tree);
    }
}
