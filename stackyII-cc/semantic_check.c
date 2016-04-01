//
//  semantic_check.c
//  stackyII-cc
//
//  Created by Michael Nolan on 3/30/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#include "semantic_check.h"
#include <stdlib.h>
int intdec = 1;

GHashTable* vars;
GHashTable* functions;
typedef enum{TYPE_INT,TYPE_VOID} return_type;
typedef struct{
	int numParams;
	return_type ret;
}func_entry;
gboolean check_vars(Node* node,gpointer d){
	node_data *n_data = (node_data*) node->data;
	if(n_data->type == T_IDENTIFIER){
		char* id = (char*)n_data->data;
		printf("id: %s\n",id);
	}
	if(n_data->type == T_VARDEC){
		
		Node*  id_node = g_node_nth_child(node,1);
		char* id = "";
		if(id_node && id_node->data != NULL){
			id = (char*)((node_data*)(id_node->data))->data;
		}
		g_hash_table_insert(vars, id, &intdec);
	}
	if(n_data->type == T_VAR){
		Node* id_node = g_node_first_child(node);
		char* id = (char*)((node_data*)(id_node->data))->data;
		if(!g_hash_table_contains(vars, id)){
			printf("Variable %s used before definition!\n",id);
			exit(-1);
		}
	}
	
	return FALSE;
}
void check_semantics(Node* root){
	vars = g_hash_table_new(g_str_hash, g_str_equal);
	functions = g_hash_table_new(g_str_hash, g_str_equal);
	g_node_traverse(root, G_PRE_ORDER, G_TRAVERSE_ALL, -1, &check_vars, NULL);
	g_hash_table_destroy(vars);
	g_hash_table_destroy(functions);
}
