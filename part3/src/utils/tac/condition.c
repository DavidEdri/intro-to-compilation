void cg_condition(struct node *tree){
    struct node *f = tree->first, *s = tree->second;
    char *token = tree->token, *code;

    // inherit labels to children
    if(strcmp(token, "&&") == 0){
        add_false_label(f, tree->falseLabel);
        add_true_label(s, tree->trueLabel);
        add_false_label(s, tree->falseLabel);
    }else if(strcmp(token, "||") == 0){
        add_true_label(f, tree->trueLabel);
        add_true_label(s, tree->trueLabel);
        add_false_label(s, tree->falseLabel);
    }else if(strcmp(token, "NOT") == 0){
        add_true_label(f, tree->falseLabel);
        add_false_label(f, tree->trueLabel);
    }

    if(f){
        cg_condition(f);
    }

    if(s){
        cg_condition(s);
    }

    // codegen
    if(is_relop(token)){
        cg_relop(tree);
    }else if(strcmp(token, "&&") == 0){
        asprintf(&code, "%s%s:\t%s", f->code, f->trueLabel, s->code);
        add_code(tree, code);
    }else if(strcmp(token, "||") == 0){
        asprintf(&code, "%s%s:\t%s", f->code, f->falseLabel, s->code);
        add_code(tree, code);
    }else if(strcmp(token, "NOT") == 0){
        asprintf(&code, "%s", f->code);
        add_code(tree, code);
    }else if(strcmp(token, "TRUE") == 0){
        asprintf(&code, "\tgoto %s\n", tree->trueLabel);
        add_code(tree, code);
    }else if(strcmp(token, "FALSE") == 0){
        asprintf(&code, "\tgoto %s\n", tree->falseLabel);
        add_code(tree, code);
    }

}

void condition_lables(struct node *tree){
    char *token = tree->token;

    if(strcmp(token, "||") == 0 || strcmp(token, "&&") == 0){
        if(tree->first){
            condition_lables(tree->first);
        }

        if(tree->second){
            condition_lables(tree->second);
        }
    }

    if(strcmp(token, "||") == 0){
        add_false_label(tree->first, freshLabel());
    }else if(strcmp(token, "&&") == 0){
        add_true_label(tree->first, freshLabel());
    }

}

int is_relop(char *t){
    return  strcmp(t, "==") == 0 ||
            strcmp(t, "!=") == 0 ||
            strcmp(t, "<") == 0 ||
            strcmp(t, ">") == 0 ||
            strcmp(t, "<=") == 0 ||
            strcmp(t, ">=") == 0 ;
}

void cg_relop(struct node *tree){
    char *code;
    asprintf(&code,"\tif %s %s %s goto %s\n\tgoto %s\n",tree->first->token, tree->token, tree->second->token, tree->trueLabel, tree->falseLabel);

    add_code(tree, code);
}