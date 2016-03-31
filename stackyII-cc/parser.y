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
	struct _GNode* tree;
}

%token <ival> INT expression
%token <fval> FLOAT
%token <sval> IDENTIFIER
%token PLUS ASSIGN EQUALS
%token INTDEC VOIDDEC
%token SEMICOLON
%type<tree> assign int identifier vardec statement expr stmt_list functype function_variables funcdef block funccall parameters funcdecl var

%left '&' '|' '^'
%left '+' '-'
%left '*' '/'

%start start
%%
start:
	stmt_list {process_tree($1);}
;
stmt_list:
	stmt_list statement SEMICOLON {$$=$1;g_node_append($$,$2);}
	|stmt_list funcdef {$$=$1;g_node_append($$,$2);}
	|stmt_list funcdecl {$$=$1;g_node_append($$,$2);}
	| {$$=makeNode(ROOT,NULL,0,NULL);}
	;
statement:
	 assign
	 | vardec
	 | expr

;
vardec:
	INTDEC var {$$=makeNode(VARDECT,NULL,0,makeNode(INTDECT,NULL,0,NULL,NULL),$2,NULL);}
;
assign:
	var EQUALS expr {$$=makeNode(ASSIGNT,NULL,0,$1,$3,NULL);}
	|INTDEC var EQUALS expr {$$=makeNode(HEAD,NULL,0,makeNode(VARDECT,NULL,0,makeNode(INTDECT,NULL,0,NULL),$2,NULL),makeNode(ASSIGNT,NULL,0,$2,$4,NULL,NULL),NULL);}
;
identifier:
	IDENTIFIER {$$=makeNode(IDENTIFIERT,$1,strlen($1)+1,NULL,NULL);}
;
int:
	INT {$$=makeNode(INTT,&$1,sizeof(int),NULL,NULL);}
;
var:
	identifier {$$=makeNode(VAR,NULL,0,$1,NULL);}
;
expr:
	int
	|var
	|funccall
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
	function_variables ',' vardec {$$=$1;g_node_append($$,$3);}
	|vardec {$$=makeNode(FUNCVARS,NULL,0,$1,NULL);}
	| {$$=makeNode(FUNCVARS,NULL,0,NULL);}
;
funcdecl:
	functype identifier '(' function_variables  ')' SEMICOLON {$$=makeNode(FUNCDECL,NULL,0,$2,$1,$4,NULL);}
funcdef:
	functype identifier '(' function_variables  ')' '{' block '}' {$$=makeNode(FUNCDEF,NULL,0,$2,$1,$4,$7,NULL);}
;
block:
	block statement SEMICOLON {$$=$1;g_node_append($$,$2);}
	| {$$=makeNode(BLOCK,NULL,0,NULL);}
;
parameters:
	parameters ',' expr {$$=$1;g_node_append($1,$3);}
	| expr {$$=makeNode(FUNCPARS,NULL,0,$1,NULL);}
	| {$$=makeNode(FUNCPARS,NULL,0,NULL);}
;
funccall:
	identifier '(' parameters ')' {$$=makeNode(FUNCCALL,NULL,0,$1,$3,NULL);}
	;
%%

void yyerror(const char* s){
	fprintf(stderr,"Parse error: %s\n",s);
	exit(-1);
}

