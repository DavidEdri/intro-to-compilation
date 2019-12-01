%{
    #include <string.h> 
    #define YYSTYPE struct node *
    #include "y.tab.h"
    int counter=1;
%}

%%

"function" return FUNCTION;
"return" return RETURN;
"void" return VOID;
"int" return INT;
"real" return REAL;
"char" return CHAR;
";" return SEMICOLON;
"," return COMMA;
"if" return IF;
"else" return ELSE;
"=" return ASSIGNMENT;
">" return GREATER;
"+" return PLUS;
"{" return LEFTBRACE;
"}" return RIGHTBRACE;
"(" return LEFTPAREN;
")" return RIGHTPAREN;
[a-zA-Z][a-zA-Z0-9_]* return ID;
[1-9][0-9]*|0 return INTEGER;
\n {counter++;}
[ \t] ;
. return yytext[0]; ;

%%