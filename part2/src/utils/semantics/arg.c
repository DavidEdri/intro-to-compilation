//core dumped when there are no args


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
        || strcmp(token, "BOOL") == 0
        || strcmp(token, "INTPTR") == 0
        || strcmp(token, "REALPTR") == 0
        || strcmp(token, "CHARPTR") == 0
    ){
        if(tree->first){
            add_arg_to_f(f, tree->first, type);
        }
        if(tree->second){
            add_arg_to_f(f, tree->second, type);
        }
    }else{
        if(arg_is_declared(f->args, tree->token)){
            print_line(tree);
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
    }else if(is_arg_type(token)){
        add_arg_to_f(f,tree, arg_type_to_int(token));
    }else{
        print_line(tree);
        printf("ast_to_args error\n");    
    }
}

void args_to_st(struct func *f){
    struct arg_arr *a = f->args;
    struct sym_el *s_tmp = NULL;

    if(!a->data){
        return;
    }

    while(a){
        s_tmp = new_sym_el();
        se_add_var(s_tmp, new_var(a->data->id, a->data->type, ""));
        st_add_item(main_stack->top, s_tmp);
        a = a->next;
    }
}

void print_arg(arg* a) {
    if(!a) return;
    printf("id:%s,type:%s\n", a->id, type_to_str(a->type));
}

void print_arg_arr(arg_arr* a) {
    struct arg_arr *tmp = a;

    while(tmp){
        if(!tmp) return;
        print_arg(tmp->data);
        tmp = tmp->next;
    }
}

arg *get_arg_by_index(struct arg_arr *arr, int index){
    int current = 0;

    while(arr && current != index){
        arr = arr->next;
        current++;
    }

    return arr->data;
}

int num_of_args(struct arg_arr* args){
    int count= 0;
    struct arg_arr* tmp = args;

    if(!tmp->data){
        return 0;
    }

    while(tmp){
        count++;
        tmp=tmp->next;
    }
  
    return count;
}

int count_tree_args(struct node* tree){   
    char *token = tree->token;
    int x = 0;   
    
    if(strcmp(token, "") != 0 && strcmp(token, "NONE") != 0 && strcmp(token, "ARGS") != 0){
        x++;
    }

    if(!is_operator(token)){
        if(tree->first){
            x += count_tree_args(tree->first);
        }
        if(tree->second){
            x += count_tree_args(tree->second);
        }
    }
    return x;
}

void func_call_to_args(struct arg_arr *arr, struct node *tree){
    char *token = tree->token;

    if(strcmp(token, "") != 0 && strcmp(token, "ARGS") != 0 && strcmp(token, "NONE") != 0){
        if(is_operator(token)){
            arg_arr_add(arr, new_arg("dummy", get_expression_type(tree)));
        }else{
            arg_arr_add(arr, new_arg("dummy", get_operand_type(tree)));
        }
    }

    if(!is_operator(token)){
        if(tree->first){
            func_call_to_args(arr, tree->first);
        }
        
        if(tree->second){
            func_call_to_args(arr, tree->second);
        }
    }
}

int comapre_args(struct arg_arr *a1, struct arg_arr *a2){
    int x = 1;
    
    if(!a1->data) return 0;

    while(a1){
        if( a1->data->type != a2->data->type && 
            !(a1->data->type == TYPE_INT && a2->data->type == TYPE_REAL) 
            // if func arg (a2) is real and got int
        ){
            return x;
        }
        a1 = a1->next;
        a2 = a2->next;
        x++;
    }

    return 0;
}




