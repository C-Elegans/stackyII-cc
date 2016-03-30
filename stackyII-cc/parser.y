%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "ast.h"
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);
	
%}
%union{
	int ival;
	float fval;
	char* sval;
	struct node* tree;
}
%token <ival> INT expression
%token <fval> FLOAT
%token <sval> IDENTIFIER
%token PLUS ASSIGN EQUALS
%token INTDEC
%token SEMICOLON
%type<tree> assign int identifier vardec statement
%start start
%%
start:
	statement {process_tree($1);}
;
statement:
	 statement assign {$$=makeNode(HEAD,NULL,0,$1,$2,NULL);}
	| statement vardec {$$=makeNode(HEAD,NULL,0,$1,$2,NULL);}
	| assign
	| vardec
;
vardec:
	INTDEC identifier {$$=makeNode(VARDECT,NULL,0,$2,NULL);}
;
assign:
	identifier EQUALS int {$$=makeNode(ASSIGNT,NULL,0,$1,$3,NULL);}
;
identifier:
	IDENTIFIER {$$=makeNode(IDENTIFIERT,$1,strlen($1)+1,NULL,NULL);}
;
int:
	INT {$$=makeNode(INTT,&$1,sizeof(int),NULL,NULL);}
;

%%

void yyerror(const char* s){
	fprintf(stderr,"Parse error: %s\n",s);
	exit(-1);
}

