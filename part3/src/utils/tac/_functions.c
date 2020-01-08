void codegen(struct node *tree){
    // printtree(tree,0,1);
    cg_handle_token(tree);
    //print_cs(main_stack);
}

void cg_handle_token(struct node *tree){
    char *token = tree->token;
    if (strcmp(token, "IF-ELSE") == 0){
        cg_if_else(tree);
    }else if(strcmp(token, "=") == 0){
        cg_assignment(tree);
    }else{
        // show unhandled tokens
        if (strcmp(token, "") != 0){
            // printf("unsuported token : %s\n", token);
        }

        cg_handle_children(tree);

        if(should_cpy_code(token)){
            cpy_code(tree);
            if(strcmp(token, "BODY") == 0){
                printf("%s\n", tree->code);
            }
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

char *freshLabel(){
    char* res;
    asprintf(&res,"L%d",label_count++);
    return res;
}

int should_cpy_code(char *token){
    return  strcmp(token, "") == 0 ||
            strcmp(token, "BLOCK") == 0 ||
            strcmp(token, "BODY") == 0;
}

void cpy_code(struct node *parent){
    struct node *f = parent->first, *s = parent->second;
    char *tmp;

    // check that first/second exists and if they got code
    asprintf(&tmp,"%s%s",f && f->code ? f->code : "", s && s->code ? s->code : "");
    add_code(parent, tmp);
}