void apply_semantics(struct node *tree)
{
    // printtree(tree->first,0,1);
    handle_token(tree);
    // print_cs(main_stack);
    got_main();
    // cs_pop(main_stack);
}

void handle_token(struct node *tree)
{
    char *token = tree->token;

    if (strcmp(token, "CODE") == 0)
    {
        main_stack = new_code_stack();
        cs_push(main_stack, new_sym_table());
        handle_children(tree);
    }
    else if (strcmp(token, "FUNCTION") == 0)
    {
        ast_to_func(tree);
    }
    else if (strcmp(token, "FUNCTION-CALL") == 0)
    {
        validate_func_call(tree);
    }
    else if (strcmp(token, "MAIN") == 0)
    {
        validate_main(tree);
    }
    else if (strcmp(token, "BLOCK") == 0)
    {
        handle_code_block(tree, NULL);
    }
    else if (is_operator(token))
    {
        get_expression_type(tree);
    }
    else if (strcmp(token, "RET") == 0)
    {
        // skip
    }
    else if (strcmp(token, "IF") == 0 || strcmp(token, "IF-ELSE") == 0 || strcmp(token, "WHILE") == 0 || strcmp(token, "DO-WHILE") == 0)
    {
        validate_if(tree, token);
    }
    else if (strcmp(token, "FOR") == 0)
    {
        validate_for(tree);
    }
    else if (is_arg_type(token))
    {
        validate_var_decleration(tree, var_type_to_int(token));
    }
    else if (strcmp(token, "=") == 0)
    {
        validate_assignment(tree, 0, 0);
    }
    else if (strcmp(token, "STR") == 0)
    {
        validate_str_declare(tree);
    }
    else if (strcmp(token, "STRCHAR") == 0)
    {
        validate_str_char(tree);
    }
    else if (strcmp(token, "DREF") == 0)
    {
        validate_dref(tree);
    }
    else
    {
        // show unhandled tokens
        if (strcmp(token, "") != 0 &&
            strcmp(token, "TRUE") != 0 &&
            strcmp(token, "FALSE") != 0 &&
            strcmp(token, "GLOBAL") != 0)
        {
            struct sym_el *el = cs_find(main_stack, token);
            
            if(!el){
                print_line(tree);
                printf("%s is undefined\n", token);
                exit(1);
            }

            if(el->type != TYPE_ID){
                print_line(tree);
                printf("%s must be a function call\n", token);
                exit(1);                
            }
        }
        handle_children(tree);
    }
}

void handle_children(struct node *tree)
{
    if (tree->first)
    {
        handle_token(tree->first);
    }

    if (tree->second)
    {
        handle_token(tree->second);
    }

    if (tree->third)
    {
        handle_token(tree->third);
    }

    if (tree->fourth)
    {
        handle_token(tree->fourth);
    }
}

void handle_code_block(struct node *tree, struct func *f)
{
    cs_push(main_stack, new_sym_table());

    if (f)
    {
        args_to_st(f);
    }

    handle_children(tree);

    if (f)
    {
        validate_return(tree, f);
    }

    cs_pop(main_stack);
}

struct node *tree_find(struct node *tree, char *id)
{
    struct node *res = NULL;

    if (strcmp(tree->token, id) == 0)
    {
        return tree;
    }
    else
    {
        if (tree->first)
        {
            res = tree_find(tree->first, id);
        }
        if (tree->second)
        {
            res = tree_find(tree->second, id);
        }
        if (tree->third)
        {
            res = tree_find(tree->third, id);
        }
        if (tree->fourth)
        {
            res = tree_find(tree->fourth, id);
        }
    }

    return res;
}

void validate_if(struct node *tree, char *token)
{
    int x;
    x = get_expression_type(tree->first);
    if (x != TYPE_BOOL)
    {
        print_line(tree);
        printf(" %s condition must be of boolean type not %s\n", token, type_to_str(x));
        exit(1);
    }
    handle_children(tree);
}

void validate_for(struct node *tree)
{
    int x;
    if (strcmp(tree->first->token, "=") == 0)
    {
        validate_assignment(tree->first, 0, 0);
    }
    else
    {
        validate_var_decleration(tree->first, var_type_to_int(tree->first->token));
    }

    x = get_expression_type(tree->second);
    if (x != TYPE_BOOL)
    {
        print_line(tree);
        printf("FOR condition must be of boolean type not %s\n", type_to_str(x));
        exit(1);
    }
    validate_assignment(tree->third, 0, 0);
    handle_children(tree->fourth);
}

void validate_assignment(struct node *tree, int new_var, int new_var_type)
{
    char *id = tree->first->token;
    struct sym_el *id_el = cs_find(main_stack, id);
    int ltype, rtype;

    if (strcmp(id, "DREF") == 0)
    {
        new_var_type = validate_dref(tree->first);
        new_var = 1;
        id = (char *)malloc(strlen(tree->first->first->token) + 9);
        strcpy(id, "dref of ");
        strcat(id, tree->first->first->token);
    }

    if (strcmp(id, "STRCHAR") == 0)
    {
        validate_str_char(tree->first);
        new_var = 1;
        new_var_type = TYPE_CHAR;
    }

    if (!id_el && !new_var)
    {
        print_line(tree);
        printf("%s is undefined\n", id);
        exit(1);
    }

    if (!new_var && id_el->type == TYPE_FUNC)
    {
        print_line(tree);
        printf("%s is a function, cannot assign to function\n", id);
        exit(1);
    }

    if (new_var)
    {
        ltype = new_var_type;
    }
    else
    {
        ltype = se_get_type(id_el);
    }

    rtype = strcmp(tree->second->token, "DREF") == 0 ? // handle assingment to *y
                validate_dref(tree->second)
                                                     : get_expression_type(tree->second);

    if (ltype != rtype &&
        !(ltype == TYPE_REAL && rtype == TYPE_INT) &&
        !(is_ptr(ltype) && rtype == TYPE_NULL))
    {
        print_line(tree);
        printf("%s is %s cannot assign %s\n", id, type_to_str(ltype), type_to_str(rtype));
        exit(1);
    }
}

int validate_dref(struct node *tree)
{
    int type = get_expression_type(tree->first);

    if (!is_ptr(type))
    {
        print_line(tree);
        printf("operator * can only be used on pointers but used on %s\n", type_to_str(type));
        exit(1);
    }
    if (type == TYPE_INTPTR)
        return TYPE_INT;
    if (type == TYPE_REALPTR)
        return TYPE_REAL;
    if (type == TYPE_CHARPTR)
        return TYPE_CHAR;
}

void validate_main(struct node *tree)
{
    struct func *f = new_func("main", TYPE_VOID);
    struct sym_el *tmp = NULL;

    // add main to symbol table
    tmp = new_sym_el();
    se_add_func(tmp, f);
    st_add_item(main_stack->top, tmp);

    handle_code_block(tree->first, f);
}

void print_line(struct node *tree)
{
    int line = tree->line;

    if (line != -1)
    {
        printf("line %d: ", tree->line);
    }
}

void got_main()
{
    struct sym_el *main = cs_find(main_stack, "main");

    if (!main)
    {
        printf("legal program must have main!\n");
        exit(1);
    }
}