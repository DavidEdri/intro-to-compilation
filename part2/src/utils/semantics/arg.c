typedef struct arg {
    char *id;
    int type;
} arg;

typedef struct arg_arr {
    struct arg *data;
    struct arg_arr *next;
} arg_arr;

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


void print_arg(arg* a) {
    printf("id:%s\ttype:%d\n", a->id, a->type);
}

void print_arg_arr(arg_arr* a) {
    struct arg_arr *tmp = a;

    while(tmp){
        print_arg(tmp->data);
        tmp = tmp->next;
    }
}
