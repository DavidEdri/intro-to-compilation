%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lex.yy.c" 
#include "utils/semantics/_structs.h"
#include "utils/semantics/_declerations.c"

struct code_stack *main_stack = NULL;
#include "utils/index.c"

int yylex();
int yyerror(char *s);
%}

%token FUNCTION VOID INT REAL FOR VAR SEMICOLON IF ELSE WHILE ASSIGNMENT REALPTR GREATER PLUS LEFTBRACE RIGHTBRACE LEFTPAREN RIGHTPAREN ID INTEGER CHARACTER CHAR RETURN COMMA BOOL DO MAIN INTPTR CHARPTR DOUBLEPTR STRDECLARE BOOLTRUE BOOLFALSE CSNULL LEFTBRACKET RIGHTBRACKET QUOTES DOUBLEQUOTES AND DIVISION EQUAL GREATEREQUAL LESS LESSEQUAL MINUS NOT NOTEQUAL OR MULTI ADDRESS HEX STR STRING REALVALUE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left PLUS MINUS SEMICOLON
%left MULTI DIVISION
%left NOT
%%

s
: code_wrapper {  apply_semantics($1); } // apply_semantics($1);   printtree($1,0,0);
;

code_wrapper
    : global        { $$ = mknode("CODE", $1, NULL, NULL, NULL, yylineno); }
    | %empty        { $$ = NULL; exit(0); }
    ;

global
    : declerations main { $$ = mknode("GLOBAL", $1, $2, NULL, NULL, yylineno); }
    | declerations      { $$ = $1; }
    | main              { $$ = $1; }
    ;

main
    : FUNCTION VOID MAIN LEFTPAREN RIGHTPAREN code_block 
        { $$ = mknode("MAIN",mknode("BODY", $6, NULL, NULL, NULL, yylineno) , NULL, NULL, NULL, yylineno); }
    ;

statements
    : statements statement  { $$ = mknode("", $1, $2, NULL, NULL, yylineno); }
    | statement             { $$ = $1;}
    ;

statement
    : if_else                   { $$ = $1; }
    | loops                     { $$ = $1; }
    | expression  SEMICOLON     { $$ = $1; }
    | return      SEMICOLON     { $$ = $1; }
    | assignment  SEMICOLON     { $$ = $1; }
    | code_block_wrapper        { $$ = $1; }
    ;

declerations
    : declerations decleration  { $$ = mknode("", $1, $2, NULL, NULL, yylineno); }
    | decleration               { $$ = $1;}
    ;

decleration
    : function_decleration      { $$ = $1; }
    | declare_VAR SEMICOLON     { $$ = $1; }
    | declare_STR SEMICOLON     { $$ = $1; }
    ;

code_block_wrapper
    : code_block    { $$ = mknode("BLOCK", $1, NULL, NULL, NULL, yylineno); }
    ;

code_block
    : LEFTBRACE RIGHTBRACE                         { $$ = NULL; }
    | LEFTBRACE declerations statements RIGHTBRACE { $$ = mknode("", $2, $3, NULL, NULL, yylineno); }
    | LEFTBRACE declerations RIGHTBRACE            { $$ = mknode("", $2, NULL, NULL, NULL, yylineno); }
    | LEFTBRACE statements RIGHTBRACE              { $$ = mknode("", $2, NULL, NULL, NULL, yylineno); }
    ;

code_block_return
    : LEFTBRACE return SEMICOLON RIGHTBRACE          { $$ = $2; }
    | LEFTBRACE declerations statements return SEMICOLON RIGHTBRACE 
        { $$ = mknode("", $2, $3, $4, NULL, yylineno); }
    | LEFTBRACE declerations return SEMICOLON RIGHTBRACE              
        { $$ = mknode("", $2, $3, NULL, NULL, yylineno); }
    | LEFTBRACE statements return SEMICOLON RIGHTBRACE
        { $$ = mknode("", $2, $3, NULL, NULL, yylineno); } 
    ;

expression
    : additive_expression {$$=$1;}
    | ADDRESS id LEFTBRACKET additive_expression RIGHTBRACKET                   
            { $$ = mknode("&", $2, $4, NULL, NULL, yylineno); }
    | ADDRESS id          { $$ = mknode("&", $2, NULL, NULL, NULL, yylineno); }
    ;

relop
    : GREATER       { $$ = mknode(">", NULL, NULL, NULL, NULL, yylineno); }
    | EQUAL         { $$ = mknode("==", NULL, NULL, NULL, NULL, yylineno); }
    | GREATEREQUAL  { $$ = mknode(">=", NULL, NULL, NULL, NULL, yylineno); }
    | LESS          { $$ = mknode("<", NULL, NULL, NULL, NULL, yylineno); }
    | LESSEQUAL     { $$ = mknode("<=", NULL, NULL, NULL, NULL, yylineno); }  
    | NOTEQUAL      { $$ = mknode("!=", NULL, NULL, NULL, NULL, yylineno); } 
    | AND           { $$ = mknode("&&", NULL, NULL, NULL, NULL, yylineno); } 
    | OR            { $$ = mknode("||", NULL, NULL, NULL, NULL, yylineno); }
    | MINUS         { $$ = mknode("-", NULL, NULL, NULL, NULL, yylineno); }
    | PLUS          { $$ = mknode("+", NULL, NULL, NULL, NULL, yylineno); }
    ;

additive_expression
    : term                            { $$ = $1; }
    | additive_expression relop term  { $$ = mknode($2 -> token, $1, $3, NULL, NULL, yylineno); }
    ;

term
    : factor                { $$ = $1; }
    | term term_op factor   { $$ = mknode($2 -> token, $1, $3, NULL, NULL, yylineno); }
    ;

term_op
    : DIVISION  { $$ = mknode("/", NULL, NULL, NULL, NULL, yylineno);}
    | MULTI     { $$ = mknode("*", NULL, NULL, NULL, NULL, yylineno);}
    ;

factor
    : dref                                      { $$ = $1; }
    | id                                        { $$ = $1; }
    | number                                    { $$ = $1; }
    | function_call                             { $$ = $1; }
    | BOOLTRUE                                  { $$ = mknode("TRUE", NULL, NULL, NULL, NULL, yylineno); }
    | BOOLFALSE                                 { $$ = mknode("FALSE", NULL, NULL, NULL, NULL, yylineno); }
    | CHARACTER                                 { $$ = mknode(yytext, NULL, NULL, NULL, NULL, yylineno); }
    | STRING                                    { $$ = mknode(yytext, NULL, NULL, NULL, NULL, yylineno); }
    | '|' id '|'                                { $$ = mknode("STRLEN", $2, NULL, NULL, NULL, yylineno); }
    | id LEFTBRACKET expression RIGHTBRACKET    { $$ = mknode("STRCHAR", $1, $3, NULL, NULL, yylineno); }
    | LEFTPAREN expression RIGHTPAREN           { $$=$2; }
    | not                                       { $$ = $1;}
    ;

not
    : NOT factor                        { $$ = mknode("NOT", $2, NULL, NULL, NULL, yylineno); }
    ;


dref
    : MULTI id                                  { $$ = mknode("DREF", $2, NULL, NULL, NULL, yylineno); }
    | MULTI LEFTPAREN expression RIGHTPAREN     { $$ = mknode("DREF", $3, NULL, NULL, NULL, yylineno); }
    ;


function_types
    : INT       { $$ = mknode("TYPE INT", NULL, NULL, NULL, NULL, yylineno); }
    | INTPTR    { $$ = mknode("TYPE INTPTR", NULL, NULL, NULL, NULL, yylineno); }
    | REAL      { $$ = mknode("TYPE REAL", NULL, NULL, NULL, NULL, yylineno); }
    | REALPTR   { $$ = mknode("TYPE REALPTR", NULL, NULL, NULL, NULL, yylineno); }
    | CHAR      { $$ = mknode("TYPE CHAR", NULL, NULL, NULL, NULL, yylineno); }
    | CHARPTR   { $$ = mknode("TYPE CHARPTR", NULL, NULL, NULL, NULL, yylineno); }
    | BOOL      { $$ = mknode("TYPE BOOL", NULL, NULL, NULL, NULL, yylineno); }
    | STR       { $$ = mknode("TYPE STR", NULL, NULL, NULL, NULL, yylineno); }
    ;

function_decleration
    : func_void     { $$ = $1; }
    | func_not_void { $$ = $1; }
    ;

func_not_void
    : FUNCTION function_types id LEFTPAREN function_args_decleration_wrapper RIGHTPAREN code_block_return
        { $$ = mknode("FUNCTION", $3, $5, $2, mknode("BODY", $7, NULL, NULL, NULL, yylineno), yylineno); }
    | FUNCTION function_types id LEFTPAREN  RIGHTPAREN code_block_return
        { $$ = mknode("FUNCTION", $3, mknode("ARGS",mknode("NONE",NULL,NULL,NULL,NULL, yylineno),NULL,NULL,NULL, yylineno), $2, mknode("BODY", $6, NULL, NULL, NULL, yylineno), yylineno); }
    ;

func_void
    : FUNCTION VOID id LEFTPAREN function_args_decleration_wrapper RIGHTPAREN code_block
        { $$ = mknode("FUNCTION", $3, $5, mknode("TYPE VOID", NULL, NULL, NULL, NULL, yylineno), mknode("BODY", $7, NULL, NULL, NULL, yylineno), yylineno); }
    | FUNCTION VOID id LEFTPAREN  RIGHTPAREN code_block
        { $$ = mknode("FUNCTION", $3, mknode("ARGS",mknode("NONE",NULL,NULL,NULL,NULL, yylineno),NULL,NULL,NULL, yylineno), mknode("TYPE VOID", NULL, NULL, NULL, NULL, yylineno), mknode("BODY", $6, NULL, NULL, NULL, yylineno), yylineno); }
    ;

function_args_decleration_wrapper
    : function_args_group
        { $$ = mknode("ARGS", $1, NULL, NULL, NULL, yylineno); }
    ;

function_args_group
    : function_args_group SEMICOLON function_args_decleration 
        { $$ = mknode("", $1, $3, NULL, NULL, yylineno); }
    | function_args_decleration { $$ = mknode("", $1, NULL, NULL, NULL, yylineno); }
    ;

function_args_decleration
    : var_types id function_args_decleration    { $$ = mknode($1->token, $2, $3, NULL, NULL, yylineno); }
    | var_types id                              { $$ = mknode($1->token, $2, NULL, NULL, NULL, yylineno); }
    | COMMA id function_args_decleration        { $$ = mknode("", $2, $3, NULL, NULL, yylineno); }
    | COMMA id                                  { $$ = mknode("", $2, NULL, NULL, NULL, yylineno); }
    ;

function_call
    : id LEFTPAREN function_call_args_wrapper RIGHTPAREN 
        { $$ = mknode("FUNCTION-CALL", $1, $3, NULL, NULL, yylineno); }
    ;

function_call_args_wrapper
    : function_call_args        { $$ = mknode("ARGS", $1, NULL, NULL, NULL, yylineno); }
    | %empty                    { $$ = mknode("ARGS", mknode("NONE", NULL, NULL, NULL, NULL, yylineno), NULL, NULL, NULL, yylineno); }
    ;

function_call_args
    : expression COMMA function_call_args   { $$ = mknode("", $1, $3, NULL, NULL, yylineno); } 
    | expression                            { $$ = mknode("", $1, NULL, NULL, NULL, yylineno); }
    ;

if_else
    : IF LEFTPAREN expression RIGHTPAREN statement ELSE statement
        { 
            $$ = mknode("IF-ELSE", $3, $5, $7, NULL, yylineno); 
        }
     |IF LEFTPAREN expression RIGHTPAREN statement  %prec LOWER_THAN_ELSE
        { 
            $$ = mknode("IF", $3, $5, NULL, NULL, yylineno); 
        }
        
    ;

loops
    : for       { $$ = $1; }
    | while     { $$ = $1; }
    | do_while  { $$ = $1; }
    ;

while
    : WHILE LEFTPAREN expression RIGHTPAREN statement 
        { $$ = mknode("WHILE", $3, $5, NULL, NULL, yylineno); }
    ;

do_while
    : DO code_block_wrapper WHILE LEFTPAREN expression RIGHTPAREN SEMICOLON
        { $$ = mknode("DO-WHILE", $5, $2, NULL, NULL, yylineno); }
    ;

for
    : FOR LEFTPAREN declare_VAR SEMICOLON expression SEMICOLON assignment RIGHTPAREN statement
        { $$ = mknode("FOR", $3, $5, $7, $9, yylineno); }
    | FOR LEFTPAREN assignment SEMICOLON expression SEMICOLON assignment RIGHTPAREN statement
        { $$ = mknode("FOR", $3, $5, $7, $9, yylineno); }
    ;

var_types
    : INT           { $$ = mknode("INT", NULL, NULL, NULL, NULL, yylineno); }
    | INTPTR        { $$ = mknode("INTPTR", NULL, NULL, NULL, NULL, yylineno); }
    | REAL          { $$ = mknode("REAL", NULL, NULL, NULL, NULL, yylineno); }
    | REALPTR       { $$ = mknode("REALPTR", NULL, NULL, NULL, NULL, yylineno); }
    | CHAR          { $$ = mknode("CHAR", NULL, NULL, NULL, NULL, yylineno); }
    | CHARPTR       { $$ = mknode("CHARPTR", NULL, NULL, NULL, NULL, yylineno); }
    | BOOL          { $$ = mknode("BOOL", NULL, NULL, NULL, NULL, yylineno); }
    ;

declare_VAR
    : VAR declare_var { $$ = $2; }
    ;

declare_var
    : var_types var                     { $$ = mknode($1->token, $2, NULL, NULL, NULL, yylineno); }    
    | var_types var COMMA declare_var   { $$ = mknode($1->token, $2, $4, NULL, NULL, yylineno); }  
    | var COMMA declare_var             { $$ = mknode("", $1, $3, NULL, NULL, yylineno); }
    | var                               { $$ = $1; }
    ;

var
    : id            { $$ = $1; }
    | assignment    { $$ = $1; }
    ;

declare_STR
    : STRDECLARE declare_str    { $$ = mknode("STR", $2, NULL, NULL, NULL, yylineno); } 
    ;

declare_str
    : str COMMA declare_str { $$ = mknode("", $1, $3, NULL, NULL, yylineno); } 
    | str                   { $$ = $1; } 
    ;

str
    : id LEFTBRACKET expression RIGHTBRACKET    { $$ = mknode($1->token, $3, NULL, NULL, NULL, yylineno); }
    | assignment                                { $$ = $1; }
    ;

assignment
    : id ASSIGNMENT expression  
        { $$ = mknode("=", $1, $3, NULL, NULL, yylineno); }
    | id ASSIGNMENT csnull
        { $$ = mknode("=", $1, $3, NULL, NULL, yylineno); }
    | id LEFTBRACKET expression RIGHTBRACKET ASSIGNMENT expression 
        { $$ = mknode("=", mknode("STRCHAR", $1, $3, NULL, NULL, yylineno), $6, NULL, NULL, yylineno); }
    | MULTI id ASSIGNMENT expression       
        { $$ = mknode("=", mknode("DREF", $2, NULL, NULL, NULL, yylineno), $4, NULL, NULL, yylineno); }
    ;

return
    : RETURN expression             { $$ = mknode("RET", $2, NULL, NULL, NULL, yylineno); }
    | RETURN csnull                 { $$ = mknode("RET", $2, NULL, NULL, NULL, yylineno); }
    ;

id
    : ID { $$ = mknode(yytext, NULL, NULL, NULL, NULL, yylineno); }
    ;

number
    : INTEGER   { $$ = mknode(yytext, NULL, NULL, NULL, NULL, yylineno); }
    | HEX       { $$ = mknode(yytext, NULL, NULL, NULL, NULL, yylineno); }
    | REALVALUE { $$ = mknode(yytext, NULL, NULL, NULL, NULL, yylineno); }
    ;

csnull
    : CSNULL { $$ = mknode("CSNULL", NULL, NULL, NULL, NULL, yylineno); }
    ;

%%

int main(){
   return yyparse();
}

int yyerror(char* s){
    printf ("%s: found line:%d unexpected token: \"%s\"\n", s, yylineno, yytext);
    return 0;
}