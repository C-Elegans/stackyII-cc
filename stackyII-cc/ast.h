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
typedef enum{ROOT=0,HEAD,ASSIGNT,VARDECT,EXPRT,IDENTIFIERT,INTT,ADD,SUBTRACT,MULTIPLY,DIVIDE,AND,OR,XOR,EQUALST
,SHL,SHR,FUNCDEF,FUNCVARS,VOIDDECT,INTDECT,BLOCK,FUNCCALL,FUNCPARS} nodetype;
typedef struct node{
	nodetype type;
	void* data;
	int capacity;
	int numchildren;
	struct node** children;
} Node;
Node* makeNode(nodetype type,void* data, size_t datasize,Node* child,...);
void print_node(Node* tree,int depth);
void append_node(Node* tree,Node* child);
void append_nodes(Node* root,Node** children);
void collapse_tree(Node* tree);
void delete_node(Node* tree, Node* delete);
void free_node(Node* node);
#endif /* ast_h */
