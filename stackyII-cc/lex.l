%{
	#include <stdio.h>
	#include "y.tab.h"
	extern int yylex();
%}
%option header-file="lex.yy.h"
%option noyywrap



%%
[ \t\n]          ;
[0-9]+\.[0-9]+   { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+           { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+     { yylval.sval = strdup(yytext); return STRING;}
\+ {return PLUS; };
%%
