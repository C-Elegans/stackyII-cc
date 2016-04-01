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
%token LE GE LT GT EQ NE
%token PLUS ASSIGN EQUALS
%token INTDEC VOIDDEC
%token SEMICOLON
%token FOR IF
%type<tree> assign int identifier vardec statement expr stmt_list functype function_variables funcdef block funccall parameters funcdecl var
%type<tree> for_loop
%left LE GE LT GT EQ NE
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
	|stmt_list for_loop {$$=$1;g_node_append($$,$2);}
	| {$$=makeNode(T_ROOT,NULL,0,NULL);}
	;
statement:
	 assign
	 | vardec
	 | expr

;
for_loop:
	FOR '(' statement SEMICOLON expr SEMICOLON statement ')' '{' block '}' {$$=makeNode(T_FOR,NULL,0,$3,$5,$10,$7,NULL);}
	//FOR '(' statement ';' ')' '{'  '}' {$$=makeNode(T_FOR,NULL,0,$3,NULL);}
	//FOR
;
vardec:
	INTDEC var {$$=makeNode(T_VARDEC,NULL,0,makeNode(T_INTDEC,NULL,0,NULL,NULL),$2,NULL);}
;
assign:
	INTDEC identifier EQUALS expr {Node* id = $2;$$=makeNode(T_HEAD,NULL,0,makeNode(T_VARDEC,NULL,0,makeNode(T_INTDEC,NULL,0,NULL),g_node_copy(id),NULL),
														  makeNode(T_ASSIGN,NULL,0,id,$4,NULL,NULL),NULL);}
	|identifier EQUALS expr {$$=makeNode(T_ASSIGN,NULL,0,$1,$3,NULL);}
	|identifier '+''+' {int t=1;$$=makeNode(T_ASSIGN,NULL,0,g_node_copy($1),makeNode(T_ADD,NULL,0,makeNode(T_VAR,NULL,0,$1,NULL),makeNode(T_INT,&t,sizeof(int),NULL),NULL),NULL);}
	|identifier '-''-' {int t=1;$$=makeNode(T_ASSIGN,NULL,0,g_node_copy($1),makeNode(T_SUBTRACT,NULL,0,makeNode(T_VAR,NULL,0,$1,NULL),makeNode(T_INT,&t,sizeof(int),NULL),NULL),NULL);}
;
identifier:
	IDENTIFIER {$$=makeNode(T_IDENTIFIER,$1,strlen($1)+1,NULL,NULL);free($1);}
;
int:
	INT {$$=makeNode(T_INT,&$1,sizeof(int),NULL,NULL);}
;
var:
	identifier {$$=makeNode(T_VAR,NULL,0,$1,NULL);}
;
expr:
	int
	|var
	|funccall
	|expr EQ expr {$$=makeNode(T_EQ,NULL,0,$1,$3,NULL);}
	|expr NE expr {$$=makeNode(T_NE,NULL,0,$1,$3,NULL);}
	|expr LT expr {$$=makeNode(T_LT,NULL,0,$1,$3,NULL);}
	|expr GT expr {$$=makeNode(T_GT,NULL,0,$1,$3,NULL);}
	|expr LE expr {$$=makeNode(T_LE,NULL,0,$1,$3,NULL);}
	|expr GE expr {$$=makeNode(T_GE,NULL,0,$1,$3,NULL);}
	|expr '+' expr {$$=makeNode(T_ADD,NULL,0,$1,$3,NULL);}
	|expr '-' expr {$$=makeNode(T_SUBTRACT,NULL,0,$1,$3,NULL);}
	|expr '*' expr {$$=makeNode(T_MULTIPLY,NULL,0,$1,$3,NULL);}
	|expr '/' expr {$$=makeNode(T_DIVIDE,NULL,0,$1,$3,NULL);}
	|expr '&' expr {$$=makeNode(T_AND,NULL,0,$1,$3,NULL);}
	|expr '|' expr {$$=makeNode(T_OR,NULL,0,$1,$3,NULL);}
	|expr '^' expr {$$=makeNode(T_XOR,NULL,0,$1,$3,NULL);}
	|expr '<''<' expr {$$=makeNode(T_SHL,NULL,0,$1,$4,NULL);}
	|expr '>''>' expr {$$=makeNode(T_SHR,NULL,0,$1,$4,NULL);}
	|expr EQUALS EQUALS expr {$$=makeNode(T_EQUALS,NULL,0,$1,$4,NULL);}
	|'(' expr ')' {$$=$2;}

;
functype: VOIDDEC {$$=makeNode(T_VOIDDEC,NULL,0,NULL);}
	|INTDEC {$$=makeNode(T_INTDEC,NULL,0,NULL);}
	;
function_variables:
	function_variables ',' vardec {$$=$1;g_node_append($$,$3);}
	|vardec {$$=makeNode(T_FUNCVARS,NULL,0,$1,NULL);}
	| {$$=makeNode(T_FUNCVARS,NULL,0,NULL);}
;
funcdecl:
	functype identifier '(' function_variables  ')' SEMICOLON {$$=makeNode(T_FUNCDECL,NULL,0,$2,$1,$4,NULL);}
funcdef:
	functype identifier '(' function_variables  ')' '{' block '}' {$$=makeNode(T_FUNCDEF,NULL,0,$2,$1,$4,$7,NULL);}
;
block:
	block statement SEMICOLON {$$=$1;g_node_append($$,$2);}
	|block for_loop {$$=$1;g_node_append($$,$2);}
	| {$$=makeNode(T_BLOCK,NULL,0,NULL);}
;
parameters:
	parameters ',' expr {$$=$1;g_node_append($1,$3);}
	| expr {$$=makeNode(T_FUNCPARS,NULL,0,$1,NULL);}
	| {$$=makeNode(T_FUNCPARS,NULL,0,NULL);}
;
funccall:
	identifier '(' parameters ')' {$$=makeNode(T_FUNCCALL,NULL,0,$1,$3,NULL);}
	;
%%

void yyerror(const char* s){
	fprintf(stderr,"Parse error: %s\n",s);
	exit(-1);
}

