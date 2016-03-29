%{
#include <stdio.h>
#include <stdlib.h>
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
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING
%token PLUS ASSIGN
%token INT
%%
start:
	start INT {printf("int: %d\n",$2);node_append(tree_head,new_tail_node(INTT,&$2,sizeof(int)));}

	| INT {printf("int: %d\n",$2);node_append(tree_head,new_tail_node(INTT,&$1,sizeof(int)));}
;

%%

void yyerror(const char* s){
	fprintf(stderr,"Parse error: %s\n",s);
	exit(-1);
}

