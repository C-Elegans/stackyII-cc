%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "ast.h"
#define EXPRESSION(type) $$=makeNode(type,0,NULL,$1,$3);
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
%token INTDEC VOIDDEC
%token SEMICOLON
%type<tree> assign int identifier vardec statement expr stmt_list functype function_variables funcdef block

%left '&' '|' '^'
%left '+' '-'
%left '*' '/'

%start start
%%
start:
	stmt_list {process_tree($1);}
;
stmt_list:
	stmt_list statement SEMICOLON {$$=$1;append_node($1,$2);}
	|stmt_list funcdef {$$=$1;append_node($1,$2);}
	| {$$=makeNode(ROOT,NULL,0,NULL);}
	;
statement:
	 assign
	 | vardec

;
vardec:
	INTDEC identifier {$$=makeNode(VARDECT,NULL,0,$2,NULL);}
;
assign:
	identifier EQUALS expr {$$=makeNode(ASSIGNT,NULL,0,$1,$3,NULL);}
	|INTDEC identifier EQUALS expr {$$=makeNode(HEAD,NULL,0,makeNode(VARDECT,NULL,0,$2,NULL),makeNode(ASSIGNT,NULL,0,$2,$4),NULL);}
;
identifier:
	IDENTIFIER {$$=makeNode(IDENTIFIERT,$1,strlen($1)+1,NULL,NULL);}
;
int:
	INT {$$=makeNode(INTT,&$1,sizeof(int),NULL,NULL);}
;
expr:
	int
	|identifier
	|expr '+' expr {$$=makeNode(ADD,NULL,0,$1,$3,NULL);}
	|expr '-' expr {$$=makeNode(SUBTRACT,NULL,0,$1,$3,NULL);}
	|expr '*' expr {$$=makeNode(MULTIPLY,NULL,0,$1,$3,NULL);}
	|expr '/' expr {$$=makeNode(DIVIDE,NULL,0,$1,$3,NULL);}
	|expr '&' expr {$$=makeNode(AND,NULL,0,$1,$3,NULL);}
	|expr '|' expr {$$=makeNode(OR,NULL,0,$1,$3,NULL);}
	|expr '^' expr {$$=makeNode(XOR,NULL,0,$1,$3,NULL);}
	|expr '<''<' expr {$$=makeNode(SHL,NULL,0,$1,$4,NULL);}
	|expr '>''>' expr {$$=makeNode(SHR,NULL,0,$1,$4,NULL);}
	|expr EQUALS EQUALS expr {$$=makeNode(EQUALST,NULL,0,$1,$4,NULL);}
	|'(' expr ')' {$$=$2;}
;
functype: VOIDDEC {$$=makeNode(VOIDDECT,NULL,0,NULL);}
	|INTDEC {$$=makeNode(INTDECT,NULL,0,NULL);}
	;
function_variables:
	function_variables ',' vardec {$$=$1;append_node($$,$3);}
	|vardec {$$=makeNode(FUNCVARS,NULL,0,$1);}
	| {$$=makeNode(FUNCVARS,NULL,0,NULL);}
;
funcdef:
	functype identifier '('  ')' '{' block '}' {$$=makeNode(FUNCDEF,NULL,0,$2,$1,$6,NULL);}
;
block:
	block statement SEMICOLON {$$=$1;append_node($1,$2);}
	| {$$=makeNode(BLOCK,NULL,0,NULL);}
;
%%

void yyerror(const char* s){
	fprintf(stderr,"Parse error: %s\n",s);
	exit(-1);
}

