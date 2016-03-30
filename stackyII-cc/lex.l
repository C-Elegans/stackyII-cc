%{
	#include <stdio.h>
	#include "y.tab.h"
	extern int yylex();
%}
%option header-file="lex.yy.h"
%option noyywrap



%%
[ \t\n]          ;
[0-9]+           { yylval.ival = atoi(yytext);printf("int: %s\n",yytext); return INT; }
int {printf("int\n");return INTDEC;};
[a-zA-Z0-9]+     { yylval.sval = strdup(yytext);printf("id: %s\n",yytext); return IDENTIFIER;}
\; {return SEMICOLON;}
= {return EQUALS;};
. {return yytext[0];}
%%
