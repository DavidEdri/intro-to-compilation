%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.yy.c"

typedef struct node{
    char *token;
    struct node *left;
    struct node *right;
} node;

node *mknode (char *token, node *left, node *right);
void printtree (node *tree, int tab);

%}

%token FUNCTION VOID INT REAL SEMICOLON IF ELSE ASSIGNMENT GREATER PLUS LEFTBRACE RIGHTBRACE LEFTPAREN RIGHTPAREN ID INTEGER CHAR RETURN COMMA

%left MINUS PLUS

%%

s: code {
        printf("ok\n");
        printtree($1, 0);
    };
code:
    function_decleration {$$ = mknode("CODE", $1, NULL);}
    |%empty {$$ = mknode("CODE", NULL, NULL);};

function_decleration:
    FUNCTION type ID LEFTPAREN params_decleration RIGHTPAREN code_block;

type:
    VOID
    |INT    
    |REAL;

code_block:
    LEFTBRACE RIGHTBRACE;

params_decleration:
    type params SEMICOLON
    |%empty;

params:
    ID COMMA params
    |ID;

%%

int main(){
   return yyparse();
}

int yyerror(char* s){
    printf ("%s: found line:%d token [%s]\n",  s,counter, yytext);
    return 0;
}

int yywrap(){
    return 0;
}

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