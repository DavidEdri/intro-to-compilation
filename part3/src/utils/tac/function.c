void cg_function(struct node *tree){
    struct node *id = tree->first, *body = tree->fourth;
    char *code;

    cg_handle_token(body);

    asprintf(&code, "%s:\n\tBeginFunc\n%s\tEndFunc\n\n", id->token, body->code);
    add_code(tree, code);
}

void cg_main(struct node *tree){
    struct node *body = tree->first;
    char *code;

    cg_handle_token(body);

    asprintf(&code, "main:\n\tBeginFunc\n%s\tEndFunc\n\n", body->code);
    add_code(tree, code);
}

void cg_return(struct node *tree){
    struct node *exp = tree->first;
    char *code;

    cg_expression(exp);

    asprintf(&code, "%s%s\tReturn %s\n", exp->code, 
            strcmp(exp->code, "") != 0 ? "\n\t" : "", // if returning expression add \n \t
            exp->var
            );
    add_code(tree, code);
}

void cg_func_call(struct node *tree, char *assignment_id){
    struct node *id = tree->first, *args = tree->second;
    char *code, *paramsCode = "", *popParamsCode, *tmpVar;
    int paramsSize = get_params_size(id->token);

    cg_func_args(args);
    cg_push_params(args, &paramsCode);

    if(assignment_id){
        add_var(tree, freshVar());
    }

    asprintf(&popParamsCode, "\tPopParams %d\n", paramsSize);
    asprintf(&code, "%s%s\t%s%sLCall %s\n%s", 
            args->code,
            paramsCode,
            assignment_id ? tree->var : "",
            assignment_id ? " = " : "",
            id->token,
            paramsSize ? popParamsCode : ""
    );
    add_code(tree, code);
}

void cg_func_args(struct node *tree){
    struct node *f = tree->first, *s = tree->second;
    char *token = tree->token;
    
    if(strcmp(token, "NONE") == 0){
        add_code(tree, "");
    }else if(strcmp(token, "") == 0 || strcmp(token, "ARGS") == 0){
        if(f) cg_func_args(f);
        if(s) cg_func_args(s);
        cpy_code(tree);
    }else{
        if( // check if need to assign to temp var
            !is_var(token) && 
            !is_operator(token) && 
            strcmp(token, "TRUE") != 0 && 
            strcmp(token, "FALSE") != 0
        ){
            char *code;

            add_var(tree, freshVar());
            asprintf(&code, "\t%s = %s\n", tree->var, token);
            add_code(tree, code);
        }else{
            cg_expression(tree);
        }
    }
}

void cg_push_params(struct node *tree, char **code){
    struct node *f = tree->first, *s = tree->second;
    char *token = tree->token, *tmp;
    
    if(strcmp(token, "NONE") == 0){
        return;
    }else if(strcmp(token, "") == 0 || strcmp(token, "ARGS") == 0){
        if(f) cg_push_params(f,code);
        if(s) cg_push_params(s,code);
    }else{
        asprintf(&tmp, "\tPushParam %s\n", tree->var);
        *code = my_str_cat(tmp, *code);
    }
}

int get_params_size(char *id){
    struct sym_el *func = cs_find(main_stack, id);
    struct arg_arr *args = NULL;
    int res = 0;

    if(!func){
        printf("didnt found %s at get_params_size\n", id);
        exit(1);
    }

    if(func->type != TYPE_FUNC){
        printf("%s not a func at get_params_size\n", id);
        exit(1);
    }

    args = func->f->args;

    while(args && args->data){
        int type = args->data->type;

        switch(type){
            case TYPE_CHAR:
                res += 1;
                break;
            case TYPE_INT:
            case TYPE_BOOL:
                res += 4;
                break;
            case TYPE_REAL:
                res += 8;
                break;
            case TYPE_INTPTR:
            case TYPE_REALPTR:
            case TYPE_CHARPTR:
                res += 8;
                break;
            default:
                printf("unknown type %s in get_params_size\n", type_to_str(type));
                exit(1);
        }
        args = args->next;
    }

    return res;
}