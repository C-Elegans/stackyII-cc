%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);
	
%}
%union{
	int ival;
	float fval;
	char* sval;
}
%token <ival> INT expression
%token <fval> FLOAT
%token <sval> IDENTIFIER
%token PLUS ASSIGN EQUALS
%token INTDEC

%%
start:
	assign
	|vardec
;
vardec:
	INTDEC IDENTIFIER
	;
assign:
	IDENTIFIER EQUALS INT {printf("%s = %d\n",$1,$3);node_append(tree_head,new_nodev(ASSIGNT,NULL,0,new_tail_node(IDENTIFIERT,$1,strlen($1)+1),
	new_tail_node(INTT,&$3,sizeof(int))));}
;


%%

void yyerror(const char* s){
	fprintf(stderr,"Parse error: %s\n",s);
	exit(-1);
}

