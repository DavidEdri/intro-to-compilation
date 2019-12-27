var *new_var(char *id, int type, char *val){
    var* res = (var*)malloc(sizeof(var));
    char* tmp_id = (char*)malloc(sizeof(id) + 1);
    char* tmp_val = (char*)malloc(sizeof(val) + 1);
    
    strcpy(tmp_id, id);
    strcpy(tmp_val, val);

    res->id = tmp_id;
    res->val = tmp_val;
    res->type = type;

    return res;
}

void validate_var_decleration(struct node *tree, int type){
    char *token = tree->token;

    if( strcmp(token, "") == 0 || is_arg_type(token)){
        if(tree->first){
            validate_var_decleration(tree->first, type);
        }
        if(tree->second){
            validate_var_decleration(tree->second, type);
        }
    }else{
        if(strcmp(token, "=") == 0){
            add_ass_var(tree, type);
        }else{
            add_empty_var(token, type);
        }
    }
}

void add_empty_var(char *token, int type){
    struct sym_el *tmp = NULL;

    if(st_is_declared(main_stack->top, token)){
        printf("%s is already declared\n", token);
        exit(1);
    }
    tmp = new_sym_el();
    se_add_var(tmp, new_var(token, type, ""));
    st_add_item(main_stack->top, tmp);
}

void add_ass_var(struct node *tree, int type){
    validate_assignment(tree, 1, type);
    add_empty_var(tree->first->token, type);
}

int validate_var_ptr(struct node *tree){
    char *id = tree->first->token;
    struct sym_el *el = cs_find(main_stack, id);
    int el_type;

    if(!el){
        printf("%s is undefined\n", id);
        exit(1);
    }

    el_type = se_get_type(el);

    switch(el_type){
        case TYPE_INT:
            return TYPE_INTPTR;
        case TYPE_REAL:
            return TYPE_REALPTR;
        case TYPE_CHAR:
            return TYPE_CHARPTR;
        default:
            printf("& operator can only be used on char,real,int or string[index] but used on: %s with type: %s\n", id, type_to_str(el_type));
            exit(1);
    }
}

void print_var(var *v){
    int is_empty = strcmp(v->val, "") == 0;
    printf("id:%s\ttype:%s\tval:%s\n", v->id, type_to_str(v->type), is_empty ? "empty" : v->val);
}