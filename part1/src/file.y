%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.yy.c"
#include "utils/functions.c"
%}

%token FUNCTION VOID INT REAL SEMICOLON IF ELSE ASSIGNMENT GREATER PLUS LEFTBRACE RIGHTBRACE LEFTPAREN RIGHTPAREN ID INTEGER CHARACTER CHAR RETURN COMMA BOOL MAIN INTPTR CHARPTR DOUBLEPTR STRDECLARE BOOLTRUE BOOLFALSE CSNULL LEFTBRACKET RIGHTBRACKET PERCENT QUOTES DOUBLEQUOTES AND DIVISION EQUAL GREATEREQUAL LESS LESSEQUAL MINUS NOT NOTEQUAL OR MULTI ADDRESS DEREFERENCE ABSUOLUTE COLON HEX STR
  
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
    | if_else                   { $$ = $1; }
    | expression                { $$ = $1; }
    | return                    { $$ = $1; }
    ;

expression
    : expression GREATER expression { $$ = mknode(">", $1, $3, NULL, NULL); }
    | expression PLUS expression { $$ = mknode("+", $1, $3, NULL, NULL); }
    | expression MULTI expression
    | expression ASSIGNMENT expression { $$ = mknode("=", $1, $3, NULL, NULL); }
    | id
    | number
    ;

function_decleration
    : FUNCTION decleration_type id LEFTPAREN function_args_decleration_wrapper RIGHTPAREN code_block
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
    | COMMA id function_args_decleration    { $$ = mknode("", $2, $3, NULL, NULL); }
    | %empty                                { $$ = NULL; }
    ;


if_else
    : IF LEFTPAREN expression RIGHTPAREN code_block ELSE code_block
        { $$ = mknode("IF-ELSE", $3, $5, $7, NULL); }
    ;

return
    : RETURN id {$$ = mknode("RET", $2, NULL, NULL, NULL); }
    | RETURN type { $$=mknode("RET", mknode(yytext, NULL, NULL, NULL, NULL), NULL, NULL, NULL); }

code_block
    : LEFTBRACE RIGHTBRACE {$$ = mknode("BLOCK", NULL, NULL, NULL, NULL); }
    | LEFTBRACE statements RIGHTBRACE {$$ = mknode("BLOCK", $2, NULL, NULL, NULL); }
    ;

id
    : ID { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

number
    : INTEGER { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

decleration_type
    : VOID  { $$ = mknode("TYPE VOID", NULL, NULL, NULL, NULL); }
    | INT   { $$ = mknode("TYPE INT", NULL, NULL, NULL, NULL); }
    | REAL  { $$ = mknode("TYPE REAL", NULL, NULL, NULL, NULL); }
    | CHAR  { $$ = mknode("TYPE CHAR", NULL, NULL, NULL, NULL); }
    ;

type
    : CHARACTER { }
    ;

%%

void main(){
    yyparse();
}

int yyerror(char* s){
    printf ("%s: found line:%d token [%s]\n", s, yylineno, yytext);
    return 0;
}

int yywrap(){
    return 0;
}
