arg *new_arg(char* id, int type) {
    arg* res = (arg*)malloc(sizeof(arg));
    char* tmp_id = (char*)malloc(sizeof(id) + 1);
    
    strcpy(tmp_id, id);

    res->id = tmp_id;
    res->type = type;

    return res;
}

arg_arr *new_arg_arr(){
    struct arg_arr *res = (struct arg_arr*)malloc(sizeof(struct arg_arr));

    res->data = NULL;
    res->next = NULL;

    return res;
}

void arg_arr_add(struct arg_arr *a, struct arg *data){
    struct arg_arr *tmp = new_arg_arr();
    struct arg_arr *current = a;

    if(!a->data){
        a->data = data;
        return;
    }

    
    while(current->next){
        current = current->next;
    }

    tmp->data = data;
    tmp->next = NULL;
    current->next = tmp;
}

int arg_is_declared(struct arg_arr *a, char *id){
    struct arg_arr *tmp = a;
    struct arg *t = NULL;

    while(tmp){
        t = tmp->data;

        if(!t){
            // empty array
            return 0;
        }

        if(strcmp(t->id, id) == 0){
            return 1;
        }

        tmp = tmp->next;
    }

    return 0;
}

void add_arg_to_f(struct func *f, struct node *tree, int type){
    char *token = tree->token;
    if(strcmp(token, "") == 0 
        || strcmp(token, "INT") == 0
        || strcmp(token, "REAL") == 0
        || strcmp(token, "CHAR") == 0
    ){
        if(tree->first){
            add_arg_to_f(f, tree->first, type);
        }
        if(tree->second){
            add_arg_to_f(f, tree->second, type);
        }
    }else{
        if(arg_is_declared(f->args, tree->token)){
            printf("arg: [%s] already declared in func: [%s]\n",tree->token ,f->id );
            exit(1);
        }else{
            arg_arr_add(f->args, new_arg(tree->token, type));
        }
        
    }
}

void ast_to_args(struct func *f, struct node *tree){
    char *token = tree->token;

    if(strcmp(token, "NONE") == 0){
        return;
    }else if(strcmp(token, "") == 0 || strcmp(token, "ARGS") == 0){
        if(tree->first){
            ast_to_args(f, tree->first);
        }
        if(tree->second){
            ast_to_args(f, tree->second);
        }
    }else if(strcmp(token, "INT") == 0){
        add_arg_to_f(f,tree, TYPE_INT);
    }else if(strcmp(token, "REAL") == 0){
        add_arg_to_f(f,tree, TYPE_REAL);
    }else if(strcmp(token, "CHAR") == 0){
        add_arg_to_f(f,tree, TYPE_CHAR);
    }else{
        printf("ast_to_args error\n");    
    }
}

void print_arg(arg* a) {
    if(!a) return;
    printf("id:%s,type:%d\n", a->id, a->type);
}

void print_arg_arr(arg_arr* a) {
    struct arg_arr *tmp = a;

    while(tmp){
        if(!tmp) return;
        print_arg(tmp->data);
        tmp = tmp->next;
    }
}
