%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.yy.c"
#include "utils/functions.c"
%}

%token FUNCTION VOID INT REAL SEMICOLON IF ELSE ASSIGNMENT GREATER PLUS LEFTBRACE RIGHTBRACE LEFTPAREN RIGHTPAREN ID INTEGER CHAR RETURN COMMA

%left MINUS PLUS

%%

s
: code {printf("ok\n"); printtree($1, 0);};

code
    : function_decleration {$$ = mknode("CODE", $1, NULL);}
    | %empty {$$ = mknode("CODE", NULL, NULL);};

function_decleration
    : FUNCTION type id LEFTPAREN params_decleration RIGHTPAREN code_block {
        $$ = mknode("FUNCTION", $3,NULL);
        };

id
    : ID {$$ = mknode(yytext, NULL, NULL);};

type
    : VOID
    | INT    
    | REAL;

code_block
    : LEFTBRACE RIGHTBRACE;

params_decleration
    : type params SEMICOLON
    | %empty;

params
    : ID COMMA params
    | ID;

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
