//
//  ast.h
//  stackyII-cc
//
//  Created by Michael Nolan on 3/29/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#ifndef ast_h
#define ast_h
typedef enum type {HEADT=0,INTT,ASSIGNT,IDENTIFIERT,VARDECT} node_type;
#include <stdio.h>
typedef struct ast_node{
	node_type type;
	void* data;
	struct ast_node *parent;
	struct ast_node **children;
	int childrensize;
	int numchildren;
} AstNode;
AstNode* new_tail_node(node_type type, void* data,size_t datasize);
AstNode* new_node(node_type type,void* data,size_t datasize, AstNode** children);
AstNode* new_nodev(node_type type,void* data,size_t datasize, AstNode* child, ...);
void node_append(AstNode* node,AstNode* new_child);
int node_count_children(AstNode* node);
void node_free(AstNode* node);
void print_node(AstNode* node,int depth);
#endif /* ast_h */
