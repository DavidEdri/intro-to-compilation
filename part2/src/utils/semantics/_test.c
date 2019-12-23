void test(){
    struct arg *a = new_arg("arg", 1);
    struct var *v = new_var("var", TYPE_BOOL, "true");
    struct func *f = new_func("func", TYPE_CHAR);
    struct sym_el *se1 = new_sym_el(), *se2 = new_sym_el(), *se3 = new_sym_el(), *se4 = new_sym_el();
    struct sym_table *st1 = new_sym_table(), *st2 = new_sym_table();
    struct code_stack *cs = new_code_stack();

    se_add_var(se1, new_var("var2", TYPE_BOOL, ""));
    se_add_func(se2, new_func("func2", TYPE_INT));
    se_add_var(se3, new_var("var3", TYPE_BOOL, ""));
    se_add_func(se4, new_func("func3", TYPE_INT));

    st_add_item(st1, se1);
    st_add_item(st1, se2);

    st_add_item(st2, se3);
    st_add_item(st2, se4);

    cs_push(cs, st1);
    cs_push(cs, st2);


    // print_arg(a);
    // print_var(v);
    // print_func(f);
    // print_se(se1);
    // print_se(se2);
    // print_st(st1);
    print_cs(cs);
}


    // switch(type){
    //     case TYPE_FUNC:
    //         el.f = 
    //         res->data = el;
    //         res->type = type;
    //         return type;
    //         break;

    //     case TYPE_VAR:

    //         break;
        
    //     default:
    //         printf("unexpected type: %d in new_sym_el func\n", type);
    //         return NULL;
    // }