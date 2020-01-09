#include "ast.h"
#define NUM_OF_STRINGS 2

int should_print(char *str)
{
    char ignore_list[NUM_OF_STRINGS][10] = {
        "",
        "GLOBAL"};

    for (int i = 0; i < NUM_OF_STRINGS; i++)
    {
        if (strcmp(str, ignore_list[i]) == 0)
        {
            return 0;
        }
    }

    return 1;
}

void printtree(node *tree, int tab, int one_line)
{
    int i;
    char *token = tree->token;
    int print_value = 1;//should_print(token);

    if(is_test){
        return;
    }

    if (print_value)
    {
        for (i = 0; i < tab; i++)
        {
            printf("\t");
        }

        if (one_line)
        {
            printf("(");
        }

        printf("%s\t", token);

        if(tree->next){
            printf("next: %s, ", tree->next);
        }
        if(tree->trueLabel){
            printf("trueLabel: %s, ", tree->trueLabel);
        }
        if(tree->falseLabel){
            printf("falseLabel: %s", tree->falseLabel);
        }
        if(tree->code){
            printf("code: %ld", strlen(tree->code));
        }
        printf("\n");
    }

    if (tree->first)
    {
        printtree(tree->first, tab + print_value, strcmp(token, "FUNCTION"));
    }

    if (tree->second)
    {
        printtree(tree->second, tab + print_value, 1);
    }

    if (tree->third)
    {
        printtree(tree->third, tab + print_value, 1);
    }

    if (tree->fourth)
    {
        printtree(tree->fourth, tab + print_value, 1);
    }

    if (print_value && one_line)
    {
        for (i = 0; i < tab; i++)
        {
            printf("\t");
        }

        printf(")\n");
    }
}

node *mknode(char *token, node *first, node *second, node *third, node *fourth, int line)
{
    node *newnode = (node *)malloc(sizeof(node));
    char *newstr = (char *)malloc(sizeof(token) + 1);
    strcpy(newstr, token);
    newnode->first = first;
    newnode->second = second;
    newnode->third = third;
    newnode->fourth = fourth;
    newnode->token = newstr;
    newnode->line = line;
    newnode->trueLabel = NULL;
    newnode->falseLabel = NULL;
    newnode->next = NULL;
    return newnode;
}

void add_true_label(struct node *n, char *l){
    n->trueLabel = l;
}

void add_false_label(struct node *n, char *l){
    n->falseLabel = l;
}

void add_next(struct node *n, char *l){
    n->next = l;
}

void add_code(struct node *n, char *c){
    n->code = c;
}

void add_var(struct node *n, char *v){
    n->var = v;
}