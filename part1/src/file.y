%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.yy.c"
#include "utils/functions.c"

int yyerror(char *s);
%}

%token FUNCTION VOID INT REAL FOR VAR SEMICOLON IF ELSE WHILE ASSIGNMENT REALPTR GREATER PLUS LEFTBRACE RIGHTBRACE LEFTPAREN RIGHTPAREN ID INTEGER CHARACTER CHAR RETURN COMMA BOOL DO MAIN INTPTR CHARPTR DOUBLEPTR STRDECLARE BOOLTRUE BOOLFALSE CSNULL LEFTBRACKET RIGHTBRACKET PERCENT QUOTES DOUBLEQUOTES AND DIVISION EQUAL GREATEREQUAL LESS LESSEQUAL MINUS NOT NOTEQUAL OR MULTI ADDRESS DEREFERENCE ABSUOLUTE COLON HEX STR STRING MINUSMINUS

%left PLUS MINUS SEMICOLON
%left MULTI DIVISION

%%

s
: code_wrapper { printf("ok\n"); printtree($1, 0, 1); }
;

code_wrapper
    : global main   { $$ = mknode("CODE", $1, $2, NULL, NULL); }
    | main          { $$ = mknode("CODE", $1, NULL, NULL, NULL); }
    | global        { $$ = mknode("CODE", $1, NULL, NULL, NULL); }
    ;

global
    :statements { $$ = mknode("GLOBAL", $1, NULL, NULL, NULL); }
    ;

main
    : VOID MAIN RIGHTPAREN LEFTPAREN LEFTBRACKET RIGHTBRACKET 
        { $$ = mknode("MAIN", NULL, NULL, NULL, NULL); }
    ;

statements
    : statements statement  { $$ = mknode("", $1, $2, NULL, NULL); }
    | statement             { $$ = $1;}
    ;

statement
    : function_decleration      { $$ = $1; }
    | if                        { $$ = $1; }
    | if_else                   { $$ = $1; }
    | loops                     { $$ = $1; }
    | expression  SEMICOLON     { $$ = $1; }
    | return      SEMICOLON     { $$ = $1; }
    | declare_var SEMICOLON     { $$ = $1; }
    | assignment  SEMICOLON     { $$ = $1; }
    ;

code_block_wrapper
    : code_block    { $$ = mknode("BLOCK", $1, NULL, NULL, NULL); }
    ;

code_block
    : LEFTBRACE RIGHTBRACE              {$$ = NULL; }
    | LEFTBRACE statements RIGHTBRACE   {$$ = mknode("", $2, NULL, NULL, NULL); }
    ;

expression
    : expression GREATER expression         { $$ = mknode(">", $1, $3, NULL, NULL); }
    | expression PLUS expression            { $$ = mknode("+", $1, $3, NULL, NULL); }
    | expression MULTI expression           { $$ = mknode("*", $1, $3, NULL, NULL); }
    | expression DIVISION expression        { $$ = mknode("/", $1, $3, NULL, NULL); }
    | expression MINUS expression           { $$ = mknode("-", $1, $3, NULL, NULL); }
    | expression EQUAL expression           { $$ = mknode("==", $1, $3, NULL, NULL); }
    | expression GREATEREQUAL expression    { $$ = mknode(">=", $1, $3, NULL, NULL); }
    | expression LESS expression            { $$ = mknode("<", $1, $3, NULL, NULL); }
    | expression LESSEQUAL expression       { $$ = mknode("<=", $1, $3, NULL, NULL); }
    | expression NOTEQUAL expression        { $$ = mknode("!=", $1, $3, NULL, NULL); }
    | expression AND expression             { $$ = mknode("&&", $1, $3, NULL, NULL); }
    | expression OR expression              { $$ = mknode("||", $1, $3, NULL, NULL); }
    | NOT expression                        { $$ = mknode("NOT", $2, NULL, NULL, NULL); }
    | ADDRESS id                            { $$ = mknode("&", $2, NULL, NULL, NULL); }
    | id                                    { $$ = $1; }
    | number                                { $$ = $1; }
    | CHARACTER                             { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    | STRING                                { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

function_decleration
    : FUNCTION function_types id LEFTPAREN function_args_decleration_wrapper RIGHTPAREN code_block
        { $$ = mknode("FUNCTION", $3, $5, $2, mknode("BODY", $7, NULL, NULL, NULL)); }
    ;

function_args_decleration_wrapper
    : function_args_decleration SEMICOLON function_args_decleration 
        {$$ = mknode("ARGS", $1, $3, NULL, NULL); }
    | function_args_decleration {$$ = mknode("ARGS", $1, NULL, NULL, NULL);}
    ;

function_args_decleration
    : INT id function_args_decleration      { $$ = mknode("INT", $2, $3, NULL, NULL); }
    | REAL id function_args_decleration     { $$ = mknode("REAL", $2, $3, NULL, NULL); }
    | INTPTR id function_args_decleration   { $$ = mknode("INTPTR", $2, $3, NULL, NULL); }
    | VOID id function_args_decleration     { $$ = mknode("VOID", $2, $3, NULL, NULL); }
    | REALPTR id function_args_decleration  { $$ = mknode("REALPTR", $2, $3, NULL, NULL); }
    | CHAR id function_args_decleration     { $$ = mknode("CHAR", $2, $3, NULL, NULL); }
    | CHARPTR id function_args_decleration  { $$ = mknode("CHARPTR", $2, $3, NULL, NULL); }
    | BOOL id function_args_decleration     { $$ = mknode("BOOL", $2, $3, NULL, NULL); }
    | STR id function_args_decleration      { $$ = mknode("STR", $2, $3, NULL, NULL); }
    | COMMA id function_args_decleration    { $$ = mknode("", $2, $3, NULL, NULL); }
    | %empty                                { $$ = mknode("NONE", NULL, NULL, NULL, NULL); }
    ;

function_types
    : INT       { $$ = mknode("TYPE INT", NULL, NULL, NULL, NULL); }
    | INTPTR    { $$ = mknode("TYPE INTPTR", NULL, NULL, NULL, NULL); }
    | REAL      { $$ = mknode("TYPE REAL", NULL, NULL, NULL, NULL); }
    | REALPTR   { $$ = mknode("TYPE CHAR", NULL, NULL, NULL, NULL); }
    | CHAR      { $$ = mknode("TYPE CHAR", NULL, NULL, NULL, NULL); }
    | CHARPTR   { $$ = mknode("TYPE CHARPTR", NULL, NULL, NULL, NULL); }
    | BOOL      { $$ = mknode("TYPE BOOL", NULL, NULL, NULL, NULL); }
    | STR       { $$ = mknode("TYPE STR", NULL, NULL, NULL, NULL); }
    | VOID      { $$ = mknode("TYPE VOID", NULL, NULL, NULL, NULL); }
    ;

if_else
    : IF LEFTPAREN expression RIGHTPAREN code_block_wrapper ELSE code_block_wrapper
        { 
            $$ = mknode("IF-ELSE", $3, $5, $7, NULL); 
        }
    ;

if
    : IF LEFTPAREN expression RIGHTPAREN code_block_wrapper 
        { 
            $$ = mknode("IF", $3, $5, NULL, NULL); 
        }
    ;

loops
    : for       { $$ = $1; }
    | while     { $$ = $1; }
    | do_while  { $$ = $1; }
    ;

while
    : WHILE LEFTPAREN expression RIGHTPAREN code_block_wrapper 
        { 
            $$ = mknode("WHILE", $3, $5, NULL, NULL); 
        }
    ;

do_while
    : DO code_block_wrapper WHILE LEFTPAREN expression RIGHTPAREN SEMICOLON
        { 
            $$ = mknode("DO-WHILE", $5, $2, NULL, NULL); 
        }
    ;


for
    : FOR LEFTPAREN assignment SEMICOLON expression SEMICOLON assignment RIGHTPAREN code_block_wrapper
        { 
            $$ = mknode("FOR", $3, $5, $7, $9); 
        }
    | FOR LEFTPAREN declare_var SEMICOLON expression SEMICOLON assignment RIGHTPAREN code_block_wrapper
        { 
            $$ = mknode("FOR", $3, $5, $7, $9); 
        }
    ;

var_types
    : INT       { $$ = mknode("INT", NULL, NULL, NULL, NULL); }
    | INTPTR    { $$ = mknode("INTPTR", NULL, NULL, NULL, NULL); }
    | REAL      { $$ = mknode("REAL", NULL, NULL, NULL, NULL); }
    | REALPTR   { $$ = mknode("CHAR", NULL, NULL, NULL, NULL); }
    | CHAR      { $$ = mknode("CHAR", NULL, NULL, NULL, NULL); }
    | CHARPTR   { $$ = mknode("CHARPTR", NULL, NULL, NULL, NULL); }
    | BOOL      { $$ = mknode("BOOL", NULL, NULL, NULL, NULL); }
    | STR       { $$ = mknode("STR", NULL, NULL, NULL, NULL); }
    | VAR       { $$ = mknode("VAR", NULL, NULL, NULL, NULL); }
    ;

declare_var
    : var_types id  
        { $$ = mknode($1->token, $2, NULL, NULL, NULL); }                  
    | var_types id ASSIGNMENT expression  
        { $$ = mknode($1->token, mknode("=", NULL, NULL, NULL, NULL), $2, $4, NULL); }                  
    | var_types id COMMA declare_var
        { $$ = mknode($1->token, $2, $4, NULL, NULL); }
    | id  COMMA declare_var { $$ =mknode("",$1, $3, NULL, NULL ); }
    | id        { $$ =$1; }
    | ASSIGNMENT expression        { $$ = mknode("=", $2, NULL, NULL, NULL); }
    ;

assignment
    : id ASSIGNMENT expression  { $$ = mknode("=", $1, $3, NULL, NULL); }
    | id ASSIGNMENT CSNULL
        { $$ = mknode("=", $1, mknode("CSNULL", NULL, NULL, NULL, NULL), NULL, NULL); }
    ;

return
    : RETURN expression             { $$ = mknode("RET", $2, NULL, NULL, NULL); }
    | RETURN                        { $$ = mknode("RET", NULL, NULL, NULL, NULL); }

id
    : ID { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

number
    : INTEGER { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

%%

void main(){
    yyparse();
}

int yywrap(){
    return 1;
}

int yyerror(char* s){
    printf ("%s: found line:%d unexpected token: \"%s\"\n", s, yylineno, yytext);
    return 0;
}