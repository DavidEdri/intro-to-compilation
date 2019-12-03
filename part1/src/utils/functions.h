#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

typedef struct node{
    char *token;
    struct node *first;
    struct node *second;
    struct node *third;
    struct node *fourth;
} node;

node *mknode (char *token, node *first, node *second, node *third, node *fourth);
void printtree (node *tree, int tab, int one_line); 

#endif 