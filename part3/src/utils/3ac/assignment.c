void cg_assignment(struct node *tree){
    char *code;
    asprintf(&code,"\t%s %s %s",tree->first->token, tree->token, tree->second->token);
    
    add_code(tree, code);
}