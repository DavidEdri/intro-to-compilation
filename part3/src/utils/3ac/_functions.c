void codegen(struct node *tree)
{
    cg_handle_token(tree);
    //print_cs(main_stack);
    // printtree(tree,0,1);
}

void cg_handle_token(struct node *tree)
{
    char *token = tree->token;
    if (strcmp(token, "IF-ELSE") == 0)
    {
        cg_if_else(tree);
        printtree(tree,0,1);
    }
    else
    {
        // show unhandled tokens
        if (strcmp(token, "") != 0)
        {
            // printf("unsuported token : %s\n", token);
        }
        cg_handle_children(tree);
    }
}

void cg_handle_children(struct node *tree)
{
    if (tree->first)
    {
        cg_handle_token(tree->first);
    }

    if (tree->second)
    {
        cg_handle_token(tree->second);
    }

    if (tree->third)
    {
        cg_handle_token(tree->third);
    }

    if (tree->fourth)
    {
        cg_handle_token(tree->fourth);
    }
}

char *freshLabel(){
    char* res;
    asprintf(&res,"L%d",label_count++);
    return res;
}

void cg_if_else(struct node *tree){
    // add true label
    add_true_label(tree->first, freshLabel());
    // handle true body labels
    cg_handle_children(tree->second);
    // add false label
    add_false_label(tree->first, freshLabel());
    // handle true body labels
    cg_handle_children(tree->third);
    // add next label
    add_next(tree, freshLabel());
}