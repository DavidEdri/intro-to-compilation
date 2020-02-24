#include "functions.h"
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
    return newnode;
}



