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
#define INITIAL_CAPACITY 4
char* names[] = {"ROOT","HEAD","ASSIGN","VARDEC","EXPR","IDENTIFIER","INT","ADD","SUBTRACT","MULTIPLY","DIVIDE","AND","OR","XOR","EQUALS",
"SHIFT LEFT","SHIFT RIGHT","FUNCDEF","FUNCVARS","void","int","BLOCK","FUNCCALL","FUNCPARS","FUNCDECL"};
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
		node->capacity = 0;
		node->numchildren = 0;
		//printf("children: %d\n",0);
		return node;
	}
	
	va_start(argp, child);
	int numchildren = 1;
	Node* p ;
	while((p = va_arg(argp, Node*)) != NULL)
		numchildren++;
	va_end(argp);
	node->capacity = numchildren;
	node->numchildren = numchildren;
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
	printf("%s:  data: ",names[tree->type]);
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
void collapse_tree(Node* tree){
	Node* rootnode = tree;
	Node** child = tree->children;
	if(child != NULL){
		while (*child != NULL) {
			if((*child)->type == HEAD){
				collapse_tree(*child);
				append_nodes(rootnode,(*child)->children);
				delete_node(rootnode, *child);
				
			}
			child++;
		}
		
	}
}
void free_node(Node* node){
	free(node->data);
	free(node->children);
	free(node);
}
void delete_node(Node* tree, Node* delete){
	if(tree->children != NULL){
		Node** child = tree->children;
		Node** new_children = child;
		int oldnumchildren = tree->numchildren;
		while(*child != NULL){
			if(*child == delete){
				tree->numchildren--;
				free(*child);
			}else{
				*new_children = *child;
				new_children++;
			}
			child++;
		}
		for(int i=tree->numchildren;i<oldnumchildren;i++){
			*(tree->children + i) = NULL;
		}
		
	}

}
void append_nodes(Node* root,Node** children){
	if(children == NULL){
		return;
	}
	Node** child = children;
	int count = 0;
	while (*child != NULL) {
		count++;
		child++;
	}
	if(root->capacity <= (root->numchildren+count)){
		root->children = realloc(root->children, root->numchildren+count + 1);
	}
	child = children;
	while (*child != NULL) {
		*(root->children+root->numchildren) = *child;
		root->numchildren ++;
		child++;
	}
	*(root->children+root->numchildren) = NULL;
}
void append_node(Node* tree,Node* child){
	if(tree->children == NULL || tree->capacity == 0 ){
		tree->children = calloc(INITIAL_CAPACITY+1, sizeof(Node*));
		tree->capacity = INITIAL_CAPACITY;
	}
	if(tree->capacity <=tree->numchildren){
		tree->children = realloc(tree->children, 2*tree->capacity + 1);
		tree->capacity =2*tree->capacity;
	}
	*(tree->children + tree->numchildren) = child;
	*(tree->children + tree->numchildren+1) = NULL;
	tree->numchildren ++;
}