%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.yy.c" 
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
: code_wrapper { printf("ok\n"); printtree($1, 0, 1); }
;

code_wrapper
    : global        { $$ = mknode("CODE", $1, NULL, NULL, NULL); }
    | %empty        { $$ = NULL; exit(0); }
    ;

global
    : declerations main { $$ = mknode("GLOBAL", $1, $2, NULL, NULL); }
    | declerations      { $$ = $1; }
    | main              { $$ = $1; }
    ;

main
    : FUNCTION VOID MAIN LEFTPAREN RIGHTPAREN code_block 
        { $$ = mknode("MAIN",mknode("BODY", $6, NULL, NULL, NULL) , NULL, NULL, NULL); }
    ;

statements
    : statements statement  { $$ = mknode("", $1, $2, NULL, NULL); }
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
    : declerations decleration  { $$ = mknode("", $1, $2, NULL, NULL); }
    | decleration               { $$ = $1;}
    ;

decleration
    : function_decleration      { $$ = $1; }
    | declare_VAR SEMICOLON     { $$ = $1; }
    | declare_STR SEMICOLON     { $$ = $1; }
    ;

code_block_wrapper
    : code_block    { $$ = mknode("BLOCK", $1, NULL, NULL, NULL); }
    ;

code_block
    : LEFTBRACE RIGHTBRACE                           { $$ = NULL; }
    | LEFTBRACE declerations statements RIGHTBRACE   { $$ = mknode("", $2, $3, NULL, NULL); }
    | LEFTBRACE declerations RIGHTBRACE              { $$ = mknode("", $2, $3, NULL, NULL); }
    | LEFTBRACE statements RIGHTBRACE                { $$ = mknode("", $2, $3, NULL, NULL); }
    ;

expression
    : additive_expression {$$=$1;}
    | ADDRESS id LEFTBRACKET additive_expression RIGHTBRACKET                   
            { $$ = mknode("&", mknode($2->token, $4, NULL, NULL, NULL), NULL, NULL, NULL); }
    ;

relop
    : GREATER       { $$ = mknode(">", NULL, NULL, NULL, NULL); }
    | EQUAL         { $$ = mknode("==", NULL, NULL, NULL, NULL); }
    | GREATEREQUAL  { $$ = mknode(">=", NULL, NULL, NULL, NULL); }
    | LESS          { $$ = mknode("<", NULL, NULL, NULL, NULL); }
    | LESSEQUAL     { $$ = mknode("<=", NULL, NULL, NULL, NULL); }  
    | NOTEQUAL      { $$ = mknode("!=", NULL, NULL, NULL, NULL); } 
    | AND           { $$ = mknode("&&", NULL, NULL, NULL, NULL); } 
    | OR            { $$ = mknode("||", NULL, NULL, NULL, NULL); }
    | MINUS         { $$ = mknode("-", NULL, NULL, NULL, NULL); }
    | PLUS          { $$ = mknode("+", NULL, NULL, NULL, NULL); }
    ;

additive_expression
    : term                              { $$ = $1; }
    | additive_expression relop term    { $$ = mknode($2 -> token, $1, $3, NULL, NULL); }
    ;

term
    : factor                { $$ = $1; }
    | term term_op factor   { $$ = mknode($2 -> token, $1, $3, NULL, NULL); }
    ;

term_op
    : DIVISION  { $$ = mknode("/", NULL, NULL, NULL, NULL);}
    | MULTI     { $$ = mknode("*", NULL, NULL, NULL, NULL);}
    ;

factor
    : dref                                      { $$ = $1; }
    | id                                        { $$ = $1; }
    | number                                    { $$ = $1; }
    | function_call                             { $$ = $1; }
    | BOOLTRUE                                  { $$ = mknode("TRUE", NULL, NULL, NULL, NULL); }
    | BOOLFALSE                                 { $$ = mknode("FALSE", NULL, NULL, NULL, NULL); }
    | CHARACTER                                 { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    | STRING                                    { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    | ADDRESS id                                { $$ = mknode("&", $2, NULL, NULL, NULL); }
    | '|' id '|'                                { $$ = mknode("STRLEN", $2, NULL, NULL, NULL); }
    | id LEFTBRACKET expression RIGHTBRACKET    { $$ = mknode("STRCHAR", $1, $3, NULL, NULL); }
    | LEFTPAREN expression RIGHTPAREN           { $$=$2; }
    | not                                       { $$ = $1;}
    ;

not
    : NOT factor                        { $$ = mknode("NOT", $2, NULL, NULL, NULL); }
    ;


dref
    : MULTI id                                  { $$ = mknode("DREF", $2, NULL, NULL, NULL); }
    | MULTI LEFTPAREN expression RIGHTPAREN     { $$ = mknode("DREF", $3, NULL, NULL, NULL); }
    ;


function_types
    : INT       { $$ = mknode("TYPE INT", NULL, NULL, NULL, NULL); }
    | INTPTR    { $$ = mknode("TYPE INTPTR", NULL, NULL, NULL, NULL); }
    | REAL      { $$ = mknode("TYPE REAL", NULL, NULL, NULL, NULL); }
    | REALPTR   { $$ = mknode("TYPE REALPTR", NULL, NULL, NULL, NULL); }
    | CHAR      { $$ = mknode("TYPE CHAR", NULL, NULL, NULL, NULL); }
    | CHARPTR   { $$ = mknode("TYPE CHARPTR", NULL, NULL, NULL, NULL); }
    | BOOL      { $$ = mknode("TYPE BOOL", NULL, NULL, NULL, NULL); }
    | STR       { $$ = mknode("TYPE STR", NULL, NULL, NULL, NULL); }
    | VOID      { $$ = mknode("TYPE VOID", NULL, NULL, NULL, NULL); }
    ;

function_decleration
    : FUNCTION function_types id LEFTPAREN function_args_decleration_wrapper RIGHTPAREN code_block
        { $$ = mknode("FUNCTION", $3, $5, $2, mknode("BODY", $7, NULL, NULL, NULL)); }
    | FUNCTION function_types id LEFTPAREN  RIGHTPAREN code_block
        { $$ = mknode("FUNCTION", $3, mknode("ARGS",mknode("NONE",NULL,NULL,NULL,NULL),NULL,NULL,NULL), $2, mknode("BODY", $6, NULL, NULL, NULL)); }
    ;

function_args_decleration_wrapper
    : function_args_group
        { $$ = mknode("ARGS", $1, NULL, NULL, NULL); }
    ;

function_args_group
    : function_args_group SEMICOLON function_args_decleration 
        { $$ = mknode("", $1, $3, NULL, NULL); }
    | function_args_decleration { $$ = mknode("", $1, NULL, NULL, NULL); }
    ;

function_args_decleration
    : var_types id function_args_decleration    { $$ = mknode($1->token, $2, $3, NULL, NULL); }
    | var_types id                              { $$ = mknode($1->token, $2, NULL, NULL, NULL); }
    | COMMA id function_args_decleration        { $$ = mknode("", $2, $3, NULL, NULL); }
    | COMMA id                                  { $$ = mknode("", $2, NULL, NULL, NULL); }
    ;

function_call
    : id LEFTPAREN function_call_args_wrapper RIGHTPAREN 
        { $$ = mknode("FUNCTION-CALL", $1, $3, NULL, NULL); }
    ;

function_call_args_wrapper
    : function_call_args        { $$ = mknode("ARGS", $1, NULL, NULL, NULL); }
    | %empty                    { $$ = mknode("ARGS", mknode("NONE", NULL, NULL, NULL, NULL), NULL, NULL, NULL); }
    ;

function_call_args
    : expression COMMA function_call_args   { $$ = mknode("", $1, $3, NULL, NULL); } 
    | expression                            { $$ = mknode("", $1, NULL, NULL, NULL); }
    ;

if_else
    : IF LEFTPAREN expression RIGHTPAREN statement ELSE statement
        { 
            $$ = mknode("IF-ELSE", $3, $5, $7, NULL); 
        }
     |IF LEFTPAREN expression RIGHTPAREN statement  %prec LOWER_THAN_ELSE
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
    : WHILE LEFTPAREN expression RIGHTPAREN statement 
        { $$ = mknode("WHILE", $3, $5, NULL, NULL); }
    ;

do_while
    : DO code_block_wrapper WHILE LEFTPAREN expression RIGHTPAREN SEMICOLON
        { $$ = mknode("DO-WHILE", $5, $2, NULL, NULL); }
    ;

for
    : FOR LEFTPAREN assignment SEMICOLON expression SEMICOLON assignment RIGHTPAREN statement
        { $$ = mknode("FOR", $3, $5, $7, $9); }
    | FOR LEFTPAREN declare_VAR SEMICOLON expression SEMICOLON assignment RIGHTPAREN statement
        { $$ = mknode("FOR", $3, $5, $7, $9); }
    ;

var_types
    : INT           { $$ = mknode("INT", NULL, NULL, NULL, NULL); }
    | INTPTR        { $$ = mknode("INTPTR", NULL, NULL, NULL, NULL); }
    | REAL          { $$ = mknode("REAL", NULL, NULL, NULL, NULL); }
    | REALPTR       { $$ = mknode("REALPTR", NULL, NULL, NULL, NULL); }
    | CHAR          { $$ = mknode("CHAR", NULL, NULL, NULL, NULL); }
    | CHARPTR       { $$ = mknode("CHARPTR", NULL, NULL, NULL, NULL); }
    | BOOL          { $$ = mknode("BOOL", NULL, NULL, NULL, NULL); }
    ;

declare_VAR
    : VAR declare_var { $$ = $2; }
    ;

declare_var
    : var_types var                     { $$ = mknode($1->token, $2, NULL, NULL, NULL); }    
    | var_types var COMMA declare_var   { $$ = mknode($1->token, $2, $4, NULL, NULL); }  
    | var COMMA declare_var             { $$ = mknode("", $1, $3, NULL, NULL); }
    | var                               { $$ = $1; }
    ;

var
    : id            { $$ = $1; }
    | assignment    { $$ = $1; }
    ;

declare_STR
    : STRDECLARE declare_str    { $$ = mknode("STR", $2, NULL, NULL, NULL); } 
    ;

declare_str
    : str COMMA declare_str { $$ = mknode("", $1, $3, NULL, NULL); } 
    | str                   { $$ = $1; } 
    ;

str
    : id LEFTBRACKET expression RIGHTBRACKET    { $$ = mknode($1->token, $3, NULL, NULL, NULL); }
    | assignment                                { $$ = $1; }
    ;

assignment
    : id ASSIGNMENT expression  
        { $$ = mknode("=", $1, $3, NULL, NULL); }
    | id ASSIGNMENT csnull
        { $$ = mknode("=", $1, $3, NULL, NULL); }
    | id LEFTBRACKET expression RIGHTBRACKET ASSIGNMENT expression 
        { $$ = mknode("=", mknode($1->token, $3, NULL, NULL, NULL), $6, NULL, NULL); }
    | id LEFTBRACKET expression RIGHTBRACKET ASSIGNMENT csnull
        { $$ = mknode("=", mknode($1->token, $3, NULL, NULL, NULL), $6, NULL, NULL); }
    | MULTI id ASSIGNMENT expression       
        { $$ = mknode("=", mknode("ADDRESS", $2, NULL, NULL, NULL), $4, NULL, NULL); }
    ;

return
    : RETURN expression             { $$ = mknode("RET", $2, NULL, NULL, NULL); }
    | RETURN                        { $$ = mknode("RET", NULL, NULL, NULL, NULL); }

id
    : ID { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

number
    : INTEGER   { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    | HEX       { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    | REALVALUE { $$ = mknode(yytext, NULL, NULL, NULL, NULL); }
    ;

csnull
    : CSNULL { $$ = mknode("CSNULL", NULL, NULL, NULL, NULL); }
    ;

%%

int main(){
   return yyparse();
}

int yyerror(char* s){
    printf ("%s: found line:%d unexpected token: \"%s\"\n", s, yylineno, yytext);
    return 0;
}