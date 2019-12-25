typedef struct func{
    char *id;
    struct arg_arr *args;
    int args_count;
    int type;
} func;

func *new_func(char *id, int type){
    // TODO : add support for args
    func* res = (func*)malloc(sizeof(func));
    char* tmp_id = (char*)malloc(sizeof(id) + 1);
    
    strcpy(tmp_id, id);

    res->id = tmp_id;
    res->type = type;
    res->args_count = 0;
    res->args = NULL;

    return res;
}

void ast_to_func(struct node* tree){
    char *id, *type;
    
    id = tree->first->token;
    type = tree->third->token;
    
    printf("new func id: %s type: %s\n", id, type);
}


void print_func(struct func *f){
    // TODO : print args
    printf("id:%s\ttype:%d\n", f->id, f->type);
}