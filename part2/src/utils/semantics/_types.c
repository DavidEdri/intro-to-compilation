#define TYPE_INT     0
#define TYPE_REAL    1
#define TYPE_BOOL    2
#define TYPE_CHAR    3
#define TYPE_STR     4
#define TYPE_VOID    5
#define TYPE_VAR     6
#define TYPE_FUNC    7
#define TYPE_ID      8

int var_type_to_int(char *type){
    if(strcmp(type, "BOOL") == 0){
        return TYPE_BOOL;
    }else if(strcmp(type, "INT") == 0){
        return TYPE_BOOL;
    }else if(strcmp(type, "REAL") == 0){
        return TYPE_BOOL;
    }else if(strcmp(type, "CHAR") == 0){
        return TYPE_BOOL;
    }

    printf("unexpected type: %s in var_type_to_int\n", type);
    exit(1);

    return -1;

}

int func_type_to_int(char *type){
    if(strcmp(type, "TYPE BOOL") == 0){
        return TYPE_BOOL;
    }
    else if(strcmp(type, "TYPE VOID") == 0){
        return TYPE_VOID;
    }
    else if(strcmp(type, "TYPE REAL") == 0){
        return TYPE_REAL;
    }
    else if(strcmp(type, "TYPE INT") == 0){
        return TYPE_INT;
    }
    else if(strcmp(type, "TYPE CHAR") == 0){
        return TYPE_CHAR;
    }

    printf("unexpected type: %s in func_type_to_int\n", type);
    exit(1);

    return -1;
}

char *type_to_str(int type){
    switch(type){
        case 0:
            return "integer";
        case 1:
            return "real";
        case 2:
            return "boolean";
        case 3:
            return "char";
        case 4:
            return "string";
        case 5:
            return "void";
        case 6:
            return "var";
        case 7:
            return "function";
        case 8:
            return "identifier";

        default:
            return "undefined type";
    }
}

int get_operand_type(struct node *tree){
    char *token = tree->token;
    char f = token[0];
    int len = strlen(token);
    int check_real = f == '+' || f == '-' || f == '.' || isdigit(f);
    int check_id = isalpha(f);

    if(strcmp(token , "TRUE") == 0 || strcmp(token, "FALSE") == 0){
        return TYPE_BOOL;
    }

    if(token[0] == '\''){
        return TYPE_CHAR;
    }

    if(token[0] == '"'){
        return TYPE_STR;
    }

    if(check_real){
        for(int i = 0; i < len; i++){
            if(token[i] == '.' || token[i] == 'E' || token[i] == 'e'){
                return TYPE_REAL;
            }
        }
    }
    
    if(check_id){
        struct sym_el *id = cs_find(main_stack, token);
        if(!id){
            printf("%s is undefined\n", token);
            exit(1);
        }

        return se_get_type(id);
    }

    return TYPE_INT;
}