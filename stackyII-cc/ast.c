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
"SHIFT LEFT","SHIFT RIGHT","FUNCDEF","FUNCVARS","void","int","BLOCK","FUNCCALL","FUNCPARS","FUNCDECL","VAR",
"==","!=","<=",">=","<",">"

};
Node* makeNode(nodetype type,void* data, size_t datasize,Node* child,...){
	
	//printf("Node:%p Initial child %p %s\n",node,child,names[type]);
	va_list argp;
	
	node_data* n_data = malloc(sizeof(node_data));
	n_data->type = type;
	n_data->data = NULL;
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
gboolean delete_traverse_func(Node* node,gpointer d){
	node_data* data = (node_data*)node->data;
	if(data->data != NULL){
		free(data->data);
		data->data = NULL;
	}
	free(data);
	node->data = NULL;
	return FALSE;
}
void delete_node_recursive(Node* node){
	g_node_traverse(node, G_POST_ORDER, G_TRAVERSE_ALL, -1, &delete_traverse_func , NULL);
	g_node_destroy(node);
}
void replace_node(Node* initial,Node* replacement){
	Node* parent = initial->parent;
	int position = g_node_child_position(parent, initial);
	delete_node_recursive(initial);
	g_node_insert(parent, position, replacement);
}
void print_node(Node* tree,int depth){
	g_node_traverse(tree, G_PRE_ORDER, G_TRAVERSE_ALL, -1, &print_traverse_func, tree);
	
	
}


