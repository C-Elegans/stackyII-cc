%{
	#include <stdio.h>
	#include "y.tab.h"
	extern int yylex();
%}
%option header-file="lex.yy.h"
%option noyywrap



%%
[ \t\n]          ;
for {return FOR;}
if {return IF;}
[0-9]+           { yylval.ival = atoi(yytext); return INT; }
int {return INTDEC;};
void {return VOIDDEC;};
[a-zA-Z][a-zA-Z0-9]*     { yylval.sval = strdup(yytext); return IDENTIFIER;}
\; {return SEMICOLON;}
= {return EQUALS;};
\=\= {return EQ;}
\< {return LT;}
\> {return GT;}
\<\= {return LE;}
\>\= {return GE;}
!= {return NE;}

. {return yytext[0];}
%%
