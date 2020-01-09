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

void check_for_str_assignment(struct node *tree){

}