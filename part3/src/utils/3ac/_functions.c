void codegen(struct node *tree)
{
    // cg_handle_token(tree);
    print_cs(main_stack);
}

void cg_handle_token(struct node *tree)
{
    char *token = tree->token;

    if (strcmp(token, "CODE") == 0)
    {
    }
    else
    {
        // show unhandled tokens
        if (strcmp(token, "") != 0)
        {
            printf("unsuported token : %s\n", token);
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