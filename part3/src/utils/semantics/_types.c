#define TYPE_INT        0
#define TYPE_REAL       1
#define TYPE_BOOL       2
#define TYPE_CHAR       3
#define TYPE_STR        4
#define TYPE_VOID       5
#define TYPE_VAR        6
#define TYPE_FUNC       7
#define TYPE_ID         8
#define TYPE_INTPTR     9
#define TYPE_REALPTR    10
#define TYPE_CHARPTR    11
#define TYPE_NULL       12
#define TYPE_ADDRESS    13

int var_type_to_int(char *type){
    if(strcmp(type, "BOOL") == 0){
        return TYPE_BOOL;
    }else if(strcmp(type, "INT") == 0){
        return TYPE_INT;
    }else if(strcmp(type, "REAL") == 0){
        return TYPE_REAL;
    }else if(strcmp(type, "CHAR") == 0){
        return TYPE_CHAR;
    }else if(strcmp(type, "CHARPTR") == 0){
        return TYPE_CHARPTR;
    }else if(strcmp(type, "REALPTR") == 0){
        return TYPE_REALPTR;
    }else if(strcmp(type, "INTPTR") == 0){
        return TYPE_INTPTR;
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
    else if(strcmp(type, "TYPE INTPTR") == 0){
        return TYPE_INTPTR;
    }
    else if(strcmp(type, "TYPE REALPTR") == 0){
        return TYPE_REALPTR;
    }
    else if(strcmp(type, "TYPE CHARPTR") == 0){
        return TYPE_CHARPTR;
    }

    printf("unexpected type: %s in func_type_to_int\n", type);
    exit(1);

    return -1;
}

int is_arg_type(char *token){
    return
        strcmp(token, "INT") == 0 ||
        strcmp(token, "REAL") == 0 ||
        strcmp(token, "CHAR") == 0 ||
        strcmp(token, "BOOL") == 0 ||
        strcmp(token, "INTPTR") == 0 ||
        strcmp(token, "REALPTR") == 0 ||
        strcmp(token, "CHARPTR") == 0 ;
}

int arg_type_to_int(char *token){
    if(strcmp(token, "INT") == 0){
        return TYPE_INT;
    }else if(strcmp(token, "REAL") == 0){
        return TYPE_REAL;
    }else if(strcmp(token, "CHAR") == 0){
        return TYPE_CHAR;
    }else if(strcmp(token, "BOOL") == 0){
        return TYPE_BOOL;
    }else if(strcmp(token, "INTPTR") == 0){
        return TYPE_INTPTR;
    }else if(strcmp(token, "CHARPTR") == 0){
        return TYPE_CHARPTR;
    }else if(strcmp(token, "REALPTR") == 0){
        return TYPE_REALPTR;
    }else{
        printf("incompetibale arg type %s\n", token); 
        exit(1);
    }
}

int is_ptr(int type){
    return type == TYPE_REALPTR || type == TYPE_CHARPTR || type == TYPE_INTPTR;
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
        case 9:
            return "int ptr";
        case 10:
            return "real ptr";
        case 11:
            return "char ptr";
        case 12:
            return "null";

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

    if(strcmp(token , "FUNCTION-CALL") == 0){
        return validate_func_call(tree);
    }

    if(strcmp(token, "STRCHAR") == 0){
        return validate_str_char(tree);
    }

    if(strcmp(token , "DREF") == 0){
        return validate_dref(tree);
    }

    if(strcmp(token , "CSNULL") == 0){
        return TYPE_NULL;
    }

    if(strcmp(token , "TRUE") == 0 || strcmp(token, "FALSE") == 0){
        return TYPE_BOOL;
    }

    if(strcmp(token , "UMINUS") == 0){
        return TYPE_INT;
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
            print_line(tree);
            printf("%s is undefined\n", token);
            exit(1);
        }

        if(id->type == TYPE_FUNC){
            print_line(tree);
            printf("%s is a function and cannot be used in expression\n", token);
            exit(1);
        }

        return se_get_type(id);
    }

    return TYPE_INT;
}