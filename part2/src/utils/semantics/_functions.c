#define NUM_OF_IGNOORES 3
void handle_children(struct node* tree);

int sem_ignore(char *str){
    char ignore_list[NUM_OF_IGNOORES][10] = {
        "",
        "GLOBAL",
        "CODE"
        };

    for (int i = 0; i < NUM_OF_IGNOORES; i++)
    {
        if (strcmp(str, ignore_list[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void handle_token(struct node* tree){
    char *token = tree->token;

    if(strcmp(token, "FUNCTION") == 0){
        ast_to_func(tree);
    }else if(strcmp(token, "BLOCK") == 0){
        // handle new block
    }else{
        printf("unsuported token : %s\n", token);
        handle_children(tree);
    }
}

void handle_children(struct node* tree){
if (tree->first)
    {
        handle_token(tree->first);
    }

    if (tree->second)
    {
        handle_token(tree->second);
    }

    if (tree->third)
    {
        handle_token(tree->third);
    }

    if (tree->fourth)
    {
        handle_token(tree->fourth);
    }
}

void apply_semantics(struct node* tree){
    handle_token(tree);
}