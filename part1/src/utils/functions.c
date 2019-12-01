#include "functions.h"

void printtree (node *tree, int tab){
    int i; 
    for (i = 0; i< tab; i++)
        printf ("\t");
    char* token = tree->token;
        printf ("(%s\n", token);
    if (tree -> left)
        printtree (tree-> left, tab + 1);  
    if (tree -> right)
        printtree (tree-> right, tab + 1); 
    for (i = 0; i< tab; i++)
        printf ("\t");
    printf(")\n");
}

node *mknode    (char *token, node *left, node *right){
    node *newnode = (node*)malloc (sizeof(node));
    char    *newstr = (char*)malloc (sizeof(token)+1);
    strcpy (newstr, token);
    newnode -> left = left;
    newnode -> right = right;
    newnode -> token = newstr;
    return newnode;
}