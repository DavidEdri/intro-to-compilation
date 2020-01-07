void cg_bool_exp(struct node *tree){
    char *token = tree->token;

    if(is_relop(token)){
        handle_relop(tree);
    }

    // printtree(tree,0,1);
}

int is_relop(char *t){
    return  strcmp(t, "==") == 0 ||
            strcmp(t, "!=") == 0 ||
            strcmp(t, "<") == 0 ||
            strcmp(t, ">") == 0 ||
            strcmp(t, "<=") == 0 ||
            strcmp(t, ">=") == 0 ;
}

void handle_relop(struct node *tree){
    char *code;
    asprintf(&code,"\tif %s %s %s goto %s\n\tgoto %s\n",tree->first->token, tree->token, tree->second->token, tree->trueLabel, tree->falseLabel);

    add_code(tree, code);
}