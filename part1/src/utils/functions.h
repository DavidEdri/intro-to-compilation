#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

typedef struct node{
    char *token;
    struct node *first;
    struct node *second;
    struct node *third;
    struct node *fourth;
    int line;
} node;

node *mknode (char *token, node *first, node *second, node *third, node *fourth, int line);
void printtree (node *tree, int tab, int one_line); 

#endif 