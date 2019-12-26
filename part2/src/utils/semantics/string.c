void validate_str_declare(struct node *tree){
    printtree(tree,0,1);
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
        printf("operator: [] can contain only iny, but id: %s got type: %s\nb", id, type_to_str(index_type));
        exit(1);
    }

    return TYPE_CHAR;
}