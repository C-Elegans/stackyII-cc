//
//  ast.h
//  stackyII-cc
//
//  Created by Michael Nolan on 3/29/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#ifndef ast_h
#define ast_h

#include <stdio.h>
#include <glib.h>
typedef enum{ROOT=0,HEAD,ASSIGNT,VARDECT,EXPRT,IDENTIFIERT,INTT,ADD,SUBTRACT,MULTIPLY,DIVIDE,AND,OR,XOR,EQUALST
,SHL,SHR,FUNCDEF,FUNCVARS,VOIDDECT,INTDECT,BLOCK,FUNCCALL,FUNCPARS,FUNCDECL,VAR,
EQT,NET,LET,GET,LTT,GTT,
  _ENUM_END} nodetype;
typedef GNode Node;
typedef struct{
	nodetype type;
	void* data;
} node_data;
Node* makeNode(nodetype type,void* data, size_t datasize,Node* child,...);
void print_node(Node* tree,int depth);
void replace_node(Node* initial,Node* replacement);
void delete_node_recursive(Node* node);
#endif /* ast_h */
