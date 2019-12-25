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

void print_var(var *v){
    int is_empty = strcmp(v->val, "") == 0;
    printf("id:%s\ttype:%d\tval:%s\n", v->id, v->type, is_empty ? "empty" : v->val);
}