%{
	#include <stdio.h>
	#include "y.tab.h"
	extern int yylex();
%}
%option header-file="lex.yy.h"
%option noyywrap



%%
[ \t\n]          ;
[0-9]+           { yylval.ival = atoi(yytext); return INT; }
int {return INTDEC;};
void {return VOIDDEC;};
[a-zA-Z][a-zA-Z0-9]*     { yylval.sval = strdup(yytext); return IDENTIFIER;}
\; {return SEMICOLON;}
= {return EQUALS;};
. {return yytext[0];}
%%
