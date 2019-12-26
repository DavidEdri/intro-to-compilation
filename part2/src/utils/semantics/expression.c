int is_operator(char *token){
    return
        strcmp(token, "+") == 0 ||
        strcmp(token, "-") == 0 ||
        strcmp(token, "*") == 0 ||
        strcmp(token, "/") == 0 ||
        strcmp(token, "||") == 0 ||
        strcmp(token, "&&") == 0 ||
        strcmp(token, ">") == 0 ||
        strcmp(token, "<") == 0 ||
        strcmp(token, ">=") == 0 ||
        strcmp(token, "<=") == 0 ||
        strcmp(token, "!=") == 0 ||
        strcmp(token, "==") == 0 ||
        strcmp(token, "STRLEN") == 0 ||
        strcmp(token, "NOT") == 0 
        ;

}

int is_singel_op(char *token){
    return strcmp(token, "STRLEN") == 0 || strcmp(token, "NOT") == 0;
}

void print_exp_error(char *op, int type1, int type2){
    printf("cannot %s between %s and %s\n", op, type_to_str(type1), type_to_str(type2));
    exit(1);
}

int is_numbers(int type1, int type2){
    return  (type1 == TYPE_INT || type1 == TYPE_REAL) && 
            (type2 == TYPE_INT || type2 == TYPE_REAL);
}

int eq_is_legal(int type1, int type2){
    return type1 == type2 && type1 != TYPE_STR && type2 != TYPE_STR;
}

int is_additive_op(char *op){
    return 
        strcmp(op, "+") == 0 ||
        strcmp(op, "-") == 0 ||
        strcmp(op, "*") == 0 ||
        strcmp(op, "/") == 0 ;
}

int is_bool_op(char *op){
    return strcmp(op, "||") == 0 || strcmp(op, "&&") == 0;
}

int is_rel_op(char *op){
    return  strcmp(op, "<=") == 0 || 
            strcmp(op, ">=") == 0 ||
            strcmp(op, "<") == 0 ||
            strcmp(op, ">") == 0;
}

int is_eq_op(char *op){
    return strcmp(op, "==") == 0 || strcmp(op, "!=") == 0;
}

int match_exp_types(struct node *first, struct node *second, char *op){
    char *f_token = first->token, *s_token = second->token;
    int type1, type2;

    if(is_operator(f_token)){
        type1 = validate_expression_type(first);
    }else{
        type1 = get_operand_type(first);
    }

    if(is_operator(s_token)){
        type2 = validate_expression_type(second);
    }else{
        type2 = get_operand_type(second);
    }

    if(is_additive_op(op)){
        if(!is_numbers(type1, type2)){
            print_exp_error(op, type1, type2);
        }

        if(type1 == TYPE_REAL || type2 == TYPE_REAL){
            return TYPE_REAL;
        }

        return TYPE_INT;
    }

    if(is_bool_op(op)){
        if(type1 != TYPE_BOOL || type2 != TYPE_BOOL){
            print_exp_error(op, type1, type2);
        }

        return TYPE_BOOL;
    }

    if(is_rel_op(op)){
        if(!is_numbers(type1, type2)){
            print_exp_error(op, type1, type2);
        }

        return TYPE_BOOL;
    }

    if(is_eq_op(op)){
        if(!eq_is_legal(type1, type2)){
            print_exp_error(op, type1, type2);
        }

        return TYPE_BOOL;
    }

    printf("error in match_exp_types op:%s\n", op);
    exit(1);

    return -1;
}

int match_singal_type(struct node *first, char *op){
    char *token = first->token;
    int type;

    if(is_operator(token)){
        type = validate_expression_type(first);
    }else{
        type = get_operand_type(first);
    }

    if(strcmp(op, "NOT") == 0){
        if(type != TYPE_BOOL){
            printf("cannot ! on %s\n", type_to_str(type));
        }

        return TYPE_BOOL;
    }

    if(strcmp(op, "STRLEN") == 0){
        if(type != TYPE_STR){
            printf("cannot get len of %s\n", type_to_str(type));
        }

        return TYPE_INT;
    }

    printf("error in match_singal_type op:%s\n", op);
    exit(1);

    return -1;
}

int validate_expression_type(struct node *tree){
    char *token = tree->token;
    if(is_singel_op(token)) return match_singal_type(tree->first, token);
    return match_exp_types(tree->first, tree->second, token);
}