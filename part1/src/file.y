%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.yy.c"
#include "utils/functions.c"
%}

%token FUNCTION VOID INT REAL SEMICOLON IF ELSE ASSIGNMENT GREATER PLUS LEFTBRACE RIGHTBRACE LEFTPAREN RIGHTPAREN ID INTEGER CHAR RETURN COMMA BOOL MAIN INTPTR CHARPTR DOUBLEPTR STRDECLARE BOOLTRUE BOOLFALSE CSNULL LEFTBRACKET RIGHTBRACKET PERCENT QUOTES DOUBLEQUOTES AND DIVISION EQUAL GREATEREQUAL LESS LESSEQUAL MINUS NOT NOTEQUAL OR MULTI ADDRESS DEREFERENCE ABSUOLUTE COLON HEX STR
  
%left MINUS PLUS
%right ASSIGNMENT SEMICOLON
%start s

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
    | statement             { $$ = mknode("", $1, NULL, NULL, NULL); }
    ;

statement
    : function_decleration
    | if_else
    ;

expression
    : expression PLUS expression
    | expression MULTI expression
    | id
    | number
    ;

function_decleration
    : FUNCTION type id LEFTPAREN function_args_decleration_wrapper RIGHTPAREN code_block_wrapper
        { $$ = mknode("FUNCTION", $3, $5, $2, $7); }
    ;

function_args_decleration_wrapper
    : function_args_decleration SEMICOLON function_args_decleration 
        {$$ = mknode("ARGS", $1, $3, NULL, NULL);}
    | function_args_decleration {$$ = mknode("ARGS", $1, NULL, NULL, NULL);}
    ;

function_args_decleration
    : INT id function_args_decleration      { $$ = mknode("INT", $2, $3, NULL, NULL); }
    | REAL id function_args_decleration     { $$ = mknode("REAL", $2, $3, NULL, NULL); }
    | COMMA id function_args_decleration    { $$ = mknode("", $2, $3, NULL, NULL); }
    | %empty                                { $$ = NULL; }
    ;


if_else
    : IF ELSE
    ;

id
    : ID { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

number
    : INTEGER { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

type
    : VOID  { $$ = mknode("VOID", NULL, NULL, NULL, NULL); }
    | INT   { $$ = mknode("INT", NULL, NULL, NULL, NULL); }
    | REAL  { $$ = mknode("REAL", NULL, NULL, NULL, NULL); }
    | CHAR  { $$ = mknode("CHAR", NULL, NULL, NULL, NULL); }
    ;

code_block_wrapper
    : LEFTBRACE RIGHTBRACE { $$ = NULL; }
    | LEFTBRACE statements RIGHTBRACE { $$ = mknode("", NULL, NULL, NULL, NULL); }
    ;

%%

int main(){
   return yyparse();
}

int yyerror(char* s){
    printf ("%s: found line:%d token [%s]\n", s, yylineno, yytext);
    return 0;
}

int yywrap(){
    return 0;
}
