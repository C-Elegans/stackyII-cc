//
//  ast.c
//  stackyII-cc
//
//  Created by Michael Nolan on 3/29/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#include "ast.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
char* names[] = {"HEAD","ASSIGN","VARDEC","EXPR","IDENTIFIER","INT","ADD","SUBTRACT","MULTIPLY"};
Node* makeNode(nodetype type,void* data, size_t datasize,Node* child,...){
	Node* node = malloc(sizeof(Node));
	Node* child_bak = child;
	//printf("Node:%p Initial child %p %s\n",node,child,names[type]);
	va_list argp;
	node->type = type;
	if(datasize != 0 && data != NULL){
		node->data = malloc(datasize);
		memcpy(node->data,data,datasize);
	}
	if(child == NULL){
		node->children = NULL;
		//printf("children: %d\n",0);
		return node;
	}
	
	va_start(argp, child);
	int numchildren = 1;
	Node* p ;
	while((p = va_arg(argp, Node*)) != NULL)
		numchildren++;
	va_end(argp);
	//printf("children: %d\n",numchildren);
	Node** children = calloc(numchildren+1, sizeof(Node*));
	node->children = children;
	*children = child_bak;
	children++;
	va_start(argp, child);
	while((p = va_arg(argp, Node*)) != NULL){
		*children = p;
		children++;
	}
	return node;
}
void print_node(Node* tree,int depth){
	for(int i=0;i<depth;i++)printf("\t");
	printf("%s: data: ",names[tree->type]);
	switch (tree->type) {
  		case INTT:
			printf("%d",*(int*)tree->data);
			break;
		case IDENTIFIERT:
			printf("%s",(char*)tree->data);
			
  		default:
			break;
	}
	printf("\n");
	if(tree->children != NULL){
		Node** child = tree->children;
		while(*child != NULL){
			print_node(*child, depth+1);
			child++;
		}
	}
	
}