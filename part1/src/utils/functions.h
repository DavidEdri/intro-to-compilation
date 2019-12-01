#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

typedef struct node{
    char *token;
    struct node *left;
    struct node *right;
} node;

node *mknode (char *token, node *left, node *right);
void printtree (node *tree, int tab); 

#endif 