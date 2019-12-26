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
    struct sym_el *tmp = NULL;
    char *token = tree->token;

    if( strcmp(token, "") == 0
        || strcmp(token, "INT") == 0
        || strcmp(token, "REAL") == 0
        || strcmp(token, "CHAR") == 0
        || strcmp(token, "BOOL") == 0
    ){
        if(tree->first){
            validate_var_decleration(tree->first, type);
        }
        if(tree->second){
            validate_var_decleration(tree->second, type);
        }
    }else{
        if(st_is_declared(main_stack->top, token)){
            printf("%s is already declared\n", token);
            exit(1);
        }

        tmp = new_sym_el();
        se_add_var(tmp, new_var(token, type, ""));
        st_add_item(main_stack->top, tmp);
    }
}


void print_var(var *v){
    int is_empty = strcmp(v->val, "") == 0;
    printf("id:%s\ttype:%d\tval:%s\n", v->id, v->type, is_empty ? "empty" : v->val);
}