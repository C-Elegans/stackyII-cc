//
//  ast.c
//  stackyII-cc
//
//  Created by Michael Nolan on 3/29/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
char* names[] = {"HEAD","INT","ASSIGN","IDENTIFIER"};
AstNode* new_tail_node(node_type type, void* data,size_t datasize){
	AstNode* node = malloc(sizeof(AstNode));
	node->type = type;
	node->data = malloc(datasize);
	memcpy(node->data, data, datasize);
	node->children = NULL;
	node->parent = NULL;
	node->numchildren = 0;
	node->childrensize = 0;
	return node;
}
AstNode* new_nodev(node_type type,void* data,size_t datasize, AstNode* child, ...){
	AstNode* node = new_tail_node(type, data,datasize );
	node_append(node, child);
	va_list argp;
	va_start(argp,child);
	AstNode* p;
	while((p = va_arg(argp, AstNode*)) != NULL)
		node_append(node, p);
	
	va_end(argp);
	
	return node;
}
AstNode* new_node(node_type type,void* data,size_t datasize, AstNode** children){
	AstNode* node = new_tail_node(type, data,datasize );
	node->children = children;
	int numchildren = node_count_children(node);
	node->numchildren = numchildren;
	if(numchildren <4)numchildren = 4; //extra space
	node->children = malloc(sizeof(AstNode*)*numchildren);
	memcpy(node->children, children, sizeof(AstNode*)*numchildren);
	node->childrensize = numchildren;
	
	return node;
}
void node_append(AstNode* node,AstNode* new_child){
	if(node->children == NULL){
		node->children =calloc(4*sizeof(AstNode*),sizeof(AstNode*));
		node->childrensize = 4;
		node->numchildren = 0;
	}
	if(node->numchildren <= node->childrensize -1){
		AstNode** children = realloc(node->children, 2*node->childrensize*sizeof(AstNode*));
		if(children == NULL)exit(-1);
		node->children = children;
		node->childrensize *=2;
	}
	*(node->children + node->numchildren) = new_child;
	*(node->children + node->numchildren + 1) = NULL;
	node->numchildren ++;
}
int node_count_children(AstNode* node){
	if (node->children == NULL) {
		return 0;
	}
	AstNode** children = node->children;
	int count = 0;
	while (*children!= NULL) {
		count++;
		children++;
	}
	return count;
}
void node_free(AstNode* node){
	free(node->data);
	AstNode** child = node->children;
	if(child != NULL){
		while (*child != NULL) {
			node_free(*child);
			child++;
		}
		free(node->children);
	}
	free(node);
}
void print_node(AstNode* node,int depth){
	for(int i=0;i<depth;i++)printf("\t");
	printf("type: %s, children: %d ",names[node->type],node->numchildren);
	switch (node->type) {
		case HEADT:
			break;
		case INTT:
			printf("int: %d",*(int*)(node->data));
			break;
		case IDENTIFIERT:
			printf("id: %s",(char*)node->data);
			break;
		default:
			break;
	}
	printf("\n");
	
	AstNode** child = node->children;
	if(child != NULL){
		while (*child != NULL) {
			print_node(*child,depth+1);
			child++;
		}
	}
}