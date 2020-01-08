void codegen(struct node *tree){
    printtree(tree,0,1);
    cg_handle_token(tree);
    // print_cs(main_stack);
    write_code(tree);
}

void cg_handle_token(struct node *tree){
    char *token = tree->token;
    if (strcmp(token, "IF-ELSE") == 0){
        cg_if_else(tree);
    }else if(strcmp(token, "IF") == 0){
        cg_if(tree);
    }else if(strcmp(token, "=") == 0){
        cg_assignment(tree);
    }else if(is_operator(token)){
        cg_expression(tree);
    }else if(is_arg_type(token)){
        // check for assignments
    }else{
        // show unhandled tokens
        if (strcmp(token, "") != 0){
            // printf("unsuported token : %s\n", token);
        }

        cg_handle_children(tree);

        if(should_cpy_code(token)){
            cpy_code(tree);
        }
    }
}

void cg_handle_children(struct node *tree){
    if (tree->first){
        cg_handle_token(tree->first);
    }

    if (tree->second){
        cg_handle_token(tree->second);
    }

    if (tree->third){
        cg_handle_token(tree->third);
    }

    if (tree->fourth){
        cg_handle_token(tree->fourth);
    }
}

char *freshVar(){
    char* res;
    asprintf(&res,"t%d",var_count++);
    return res;
}

char *freshLabel(){
    char* res;
    asprintf(&res,"L%d",label_count++);
    return res;
}

int should_cpy_code(char *token){
    return  strcmp(token, "") == 0 ||
            strcmp(token, "BLOCK") == 0 ||
            strcmp(token, "FUNCTION") == 0 ||
            strcmp(token, "GLOBAL") == 0 ||
            strcmp(token, "CODE") == 0 ||
            strcmp(token, "BODY") == 0;
}

void cpy_code(struct node *parent){
    struct node *f = parent->first, *s = parent->second, *t = parent->third, *fo = parent->fourth;
    char *tmp;

    // check that first/second exists and if they got code
    asprintf(&tmp,"%s%s%s%s",
        f && f->code ? f->code : "", 
        s && s->code ? s->code : "", 
        t && t->code ? t->code : "", 
        fo && fo->code ? fo->code : "");
    add_code(parent, tmp);
}

void write_code(struct node *tree){
    FILE *f = fopen("output.txt", "w");

    if (f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }
    
    printf("%s\n", tree->code);
    fprintf(f, "%s\n", tree->code);
}