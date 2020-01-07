#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

typedef struct node{
    char *token;
    struct node *first;
    struct node *second;
    struct node *third;
    struct node *fourth;
    int line;
    char *trueLabel;
    char *falseLabel;
    char *next;
    char *code;
} node;

node *mknode (char *token, node *first, node *second, node *third, node *fourth, int line);
void printtree (node *tree, int tab, int one_line); 
void add_next(struct node *n, char *l);
void add_false_label(struct node *n, char *l);
void add_true_label(struct node *n, char *l);
void add_code(struct node *n, char *c);

#endif 