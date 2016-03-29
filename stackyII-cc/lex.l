%{
	#include <stdio.h>
	#include "y.tab.h"
	extern int yylex();
%}
%option header-file="lex.yy.h"
%option noyywrap



%%
[ \t\n]          ;
[0-9]+\.[0-9]+   { yylval.fval = atof(yytext);printf("float: %s\n",yytext); return FLOAT; }
[0-9]+           { yylval.ival = atoi(yytext);printf("int: %s\n",yytext); return INT; }
[a-zA-Z0-9]+     { yylval.sval = strdup(yytext);printf("id: %s\n",yytext); return IDENTIFIER;}
int {return INTDEC;};
\+ {return PLUS; };
= {return EQUALS;};
%%
