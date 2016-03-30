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
typedef enum{HEAD=0,ASSIGNT,VARDECT,EXPRT,IDENTIFIERT,INTT,ADD,SUBTRACT,MULTIPLY} nodetype;
typedef struct node{
	nodetype type;
	void* data;
	struct node** children;
} Node;
Node* makeNode(nodetype type,void* data, size_t datasize,Node* child,...);
void print_node(Node* tree,int depth);
#endif /* ast_h */
