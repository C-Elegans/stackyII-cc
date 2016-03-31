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
"SHIFT LEFT","SHIFT RIGHT","FUNCDEF","FUNCVARS","void","int","BLOCK","FUNCCALL","FUNCPARS","FUNCDECL","VAR"};
Node* makeNode(nodetype type,void* data, size_t datasize,Node* child,...){
	
	//printf("Node:%p Initial child %p %s\n",node,child,names[type]);
	va_list argp;
	
	node_data* n_data = malloc(sizeof(node_data));
	n_data->type = type;
	if(datasize != 0 && data != NULL){
		n_data->data = malloc(datasize);
		memcpy(n_data->data,data,datasize);
	}
	Node* node = g_node_new(n_data);
	if(child == NULL){
		
		return node;
	}
	g_node_append(node, child);
	va_start(argp, child);
	
	Node* p ;
	while((p = va_arg(argp, Node*)) != NULL)
		g_node_append(node, p);
	va_end(argp);
	
	return node;
}
gboolean print_traverse_func(GNode* node,gpointer data){
	GNode* root = (GNode*)data;
	int depth = g_node_depth(node);
	for(int i=0;i<depth-1;i++)printf("\t");
	node_data* n_data = (node_data*)node->data;
	printf("%s ",names[n_data->type]);
	if(n_data->type == INTT)printf("data: %d",*(int*)(n_data->data));
	if(n_data->type == IDENTIFIERT)printf("data: %s",n_data->data);
	printf("\n");
	return FALSE;
}
void print_node(Node* tree,int depth){
	g_node_traverse(tree, G_PRE_ORDER, G_TRAVERSE_ALL, -1, &print_traverse_func, tree);
	
	
}

/*void collapse_tree(Node* tree){
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
/*void delete_node(Node* tree, Node* delete){
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
	if(tree->capacity <=tree->numchildren+1){
		printf("Reallocating tree\n");
		tree->children = realloc(tree->children, 2*tree->capacity + 1);
		tree->capacity =2*tree->capacity;
	}
	*(tree->children + tree->numchildren) = child;
	*(tree->children + tree->numchildren+1) = NULL;
	tree->numchildren ++;
}*/