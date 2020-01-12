void codegen(struct node *tree){
    // printtree(tree,0,1);
    cg_handle_token(tree);
    // print_cs(main_stack);
    write_code(tree);
}

void cg_handle_token(struct node *tree){
    char *token = tree->token;
    if (strcmp(token, "IF-ELSE") == 0 || strcmp(token, "IF") == 0){
        cg_if_else(tree);
    }else if(strcmp(token, "FUNCTION") == 0){
        cg_function(tree);
        var_count = 0;
    }else if(strcmp(token, "MAIN") == 0){
        cg_main(tree);
    }else if(strcmp(token, "WHILE") == 0){
        cg_while(tree);
    }else if(strcmp(token, "DO-WHILE") == 0){
        cg_do_while(tree);
    }else if(strcmp(token, "FOR") == 0){
        cg_for(tree);
    }else if(strcmp(token, "FUNCTION-CALL") == 0){
        cg_func_call(tree, NULL);
    }else if(strcmp(token, "=") == 0){
        cg_assignment(tree);
    }else if(strcmp(token, "RET") == 0){
        cg_return(tree);
    }else if(strcmp(token, "STRCHAR") == 0){
        cg_expression(tree);
    }else if(is_operator(token) || strcmp(token, "TRUE") == 0 || strcmp(token, "FALSE") == 0){
        cg_expression(tree);
    }else if(is_arg_type(token) || strcmp(token, "STR") == 0){
        // check for assignments
        check_for_assignment(tree);
    }else{
        cg_handle_children(tree);

        if(should_cpy_code(token)){
            cpy_code(tree);
        }else{
            // printf("unsuported token : %s\n", token);
            // add useless statment to code
            char *tmp;
            asprintf(&tmp, "\t%s\n", token);
            add_code(tree, tmp);
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

int is_var(char *token){
    return  isalpha(token[0]) != 0 &&
            strcmp(token, "TRUE") != 0 &&
            strcmp(token, "FALSE") != 0 &&
            strcmp(token, "FUNCTION-CALL") != 0 &&
            strcmp(token, "STRCHAR") != 0 &&
            strcmp(token, "CSNULL") != 0 &&
            strcmp(token, "DREF") != 0 ;
}

int should_cpy_code(char *token){
    return  strcmp(token, "") == 0 ||
            strcmp(token, "BLOCK") == 0 ||
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

void strip_extra_tabs(char* str) {
  int i, x;
  for(i=x=0; str[i]; ++i)
    if(str[i] != '\t' || (i > 0 && str[i-1] != '\t'))
      str[x++] = str[i];
  str[x] = '\0';
}

void write_code(struct node *tree){
    int flag = 0;
    char *code = tree->code;
    FILE *f = fopen("output.txt", "w");

    if (f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    strip_extra_tabs(code);

    printf("%s", !is_test ? code : "");
    fprintf(f, "%s", code);
}

char* my_str_cat(char *a, char *b){
    char *res = (char*)malloc(strlen(a) + strlen(b) + 1);

    strcpy(res, a);
    strcat(res, b);

    return res;
}