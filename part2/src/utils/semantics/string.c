void validate_str_declare(struct node *tree){
    char *token = tree->token;
    
    if(strcmp(token, "STR") == 0 || strcmp(token, "") == 0 ){
        if(tree->first){
            validate_str_declare(tree->first);
        }
        if(tree->second){
            validate_str_declare(tree->second);
        }
    }else{
        if(strcmp(token, "=") == 0){
            validate_str_assingment(tree);
        }else{
            validate_str(tree);
        }
    }
}

void validate_str(struct node *tree){
    char *token = tree->token;
    struct sym_el *el = st_find(main_stack->top, token);
    int size_type;

    if(el){
        printf("%s is already declared\n", token);
        exit(1);
    }

    size_type = get_expression_type(tree->first);
    if(size_type != TYPE_INT){
        printf("string size can be integer only but %s got size type of %s\n", token, type_to_str(size_type));
        exit(1);
    }

    el = new_sym_el();
    se_add_var(el, new_var(token, TYPE_STR, ""));
    st_add_item(main_stack->top, el);
}

void validate_str_assingment(struct node *tree){
    struct node *adjusted = mknode(tree->first->first->token, tree->first->second,NULL,NULL,NULL); // converting from tree with strchar to tree of declertation
    int assingment_type = get_expression_type(tree->second);
    if(assingment_type != TYPE_STR){
        printf("string can only be assinged to string but %s was assigned to %s\n", tree->first->token, type_to_str(assingment_type));
        exit(1);
    }
    validate_str(adjusted);
}

int validate_str_char(struct node *tree){
    char *id = tree->first->token;
    struct sym_el *el = cs_find(main_stack, id);
    int el_type, index_type;

    if(!el){
        printf("%s is undefined\n", id);
        exit(1);
    }

    el_type = se_get_type(el);
    if(el_type != TYPE_STR){
        printf("operator: [] can be used only on strings, but used on: %s with type: %s\nb", id, type_to_str(el_type));
        exit(1);
    }

    index_type = get_expression_type(tree->second);
    if(index_type != TYPE_INT){
        printf("operator: [] can contain only int, but id: %s got type: %s\nb", id, type_to_str(index_type));
        exit(1);
    }

    return TYPE_CHAR;
}