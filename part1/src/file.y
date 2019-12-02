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
: global {printf("ok\n"); printtree($1, 0);}
;

global
    : actions {$$ = mknode("CODE", $1, NULL);}
    ;

actions
    : actions action {$$ = mknode("", $1, $2);}
    | action {$$ = mknode("", $1, NULL);}
    ;

action
    : function_decleration {$$ = mknode("FUNCTION", $1, NULL);}
    | expression {$$ = mknode("EXP", $1, NULL);}
    ;

function_decleration
    : FUNCTION type id LEFTPAREN func_args RIGHTPAREN code_block
        {
        $$ = mknode("FUNCTION", $3,NULL);
        }
    ;
expression
    : expression PLUS expression {$$ = mknode("+", $1, $2);}
    | INTEGER {$$ = mknode("yytext", NULL, NULL);}
    ;
id
    : ID {$$ = mknode(yytext, NULL, NULL);}
    ;

type
    : VOID
    | INT    
    | REAL
    | CHAR
    ;

code_block
    : LEFTBRACE RETURN RIGHTBRACE
    |LEFTBRACE RIGHTBRACE
    ;

func_args
    : params_decleration SEMICOLON
    | func_args params_decleration
    | %empty
    ;

params_decleration
    : type params 
    ;

params
    : ID COMMA params
    | ID
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
