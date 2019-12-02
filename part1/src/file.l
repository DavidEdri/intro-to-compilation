%{
    #include <string.h> 
    #define YYSTYPE struct node *
    #include "y.tab.h"
    int yylineno;
%}

%%

"function" return FUNCTION;
"return" return RETURN;
"void" return VOID;
"boolean" return BOOL;
"main" return MAIN;
"int" return INT;
"int*" return INTPTR;
"char*" return CHARPTR;
"double*" return DOUBLEPTR;
"return" return RETURN;
"string" return STRDECLARE;
"real" return REAL;
"char" return CHAR;
"true" return BOOLTRUE;
"false" return BOOLFALSE;
"null" return CSNULL;
";" return SEMICOLON;
"," return COMMA;
"if" return IF;
"else" return ELSE;
"{" return LEFTBRACE;
"}" return RIGHTBRACE;
"(" return LEFTPAREN;
")" return RIGHTPAREN;
"[" return LEFTBRACKET;
"]" return RIGHTBRACKET;
"%" return PERCENT;
"'"  return QUOTES;
"\""  return DOUBLEQUOTES;
"=" return ASSIGNMENT;
"&&" return AND;
\/ return DIVISION;
"==" return EQUAL;
">" return GREATER;
">=" return GREATEREQUAL;
"<" return LESS;
"<=" return LESSEQUAL;
"-" return MINUS;
"!" return NOT;
"!=" return NOTEQUAL;
"||" return OR;
"+" return PLUS;
"*" return MULTI;
"&" return ADDRESS;
"^" return DEREFERENCE;
"|" return ABSUOLUTE;
":" return COLON;
[a-zA-Z][a-zA-Z0-9_]* return ID;
[-?][1-9][0-9]*|0 return INTEGER;
\n {yylineno++;}
0(x|X)0[0-9A-F] return HEX;
"\""[a-zA-z0-9][A-z]"\"" return STR; 
<comment>[^\n] ;
<comment>\n     ;/*counter++;*/
<comment>"*/" BEGIN 0;
[ \t\n] ;




%%