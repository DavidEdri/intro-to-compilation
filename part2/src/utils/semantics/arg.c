typedef struct arg {
    char *id;
    int type;
} arg;

arg *new_arg(char* id, int type) {
    arg* res = (arg*)malloc(sizeof(arg));
    char* tmp_id = (char*)malloc(sizeof(id) + 1);
    
    strcpy(tmp_id, id);

    res->id = tmp_id;
    res->type = type;

    return res;
}

void print_arg(arg* a) {
    printf("id:%s\ttype:%d\n", a->id, a->type);
}
