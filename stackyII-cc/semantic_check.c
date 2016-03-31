//
//  semantic_check.c
//  stackyII-cc
//
//  Created by Michael Nolan on 3/30/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#include "semantic_check.h"
#include <glib.h>
int intdec = 1;

GHashTable* vars;
GHashTable* functions;
typedef enum{TYPE_INT,TYPE_VOID} return_type;
typedef struct{
	int numParams;
	return_type ret;
}func_entry;
void check_vars(Node* root){
	if(root->type == VARDECT){
		printf("def: %s\n",(*((*(root->children+1))->children))->data);
		g_hash_table_insert(vars, (*((*(root->children+1))->children))->data, &intdec);
	}
	if(root->type == FUNCDEF){
		if(!g_hash_table_contains(functions, (*(root->children))->data)){
			Node* funcvars = *(root->children+2);
			func_entry* entry = malloc(sizeof(func_entry));
			entry->numParams = funcvars->numchildren;
			nodetype t = (*(root->children+1))->type;
			entry->ret = t-VOIDDECT;
			printf("funcdef: %s\n",(*(root->children))->data);
			g_hash_table_insert(functions, (*(root->children))->data, entry);
		}
	}
	if(root->type == FUNCDECL){
		printf("funcdec: %s\n",(*(root->children))->data);
		if(!g_hash_table_contains(functions, (*(root->children))->data)){
			g_hash_table_insert(functions, (*(root->children))->data, &intdec);
		}
	}
	if(root->children != NULL){
		Node** child = root->children;
		while (*child != NULL) {
			check_vars(*child);
			child++;
		}
	}
	
	if(root->type == VAR){
		printf("%s\n",(char*)(*(root->children))->data);
		if(!g_hash_table_contains(vars, (*(root->children))->data)){
			printf("Variable %s  used before definition!\n",(*(root->children))->data);
		}
	}
	if(root->type == FUNCCALL){
		char* id = (*(root->children))->data;
		if(!g_hash_table_contains(functions, id)){
			printf("Function %s used before definition!\n",(*(root->children))->data);
		}
		func_entry* entry = g_hash_table_lookup(functions, id);
		int params = (*(root->children+1))->numchildren;
		if(entry->numParams != params){
			printf("Function %s expected %d params, found %d\n",id,entry->numParams,params);
		}
		printf("call: %s\n",id);
	}
	
	
}
void check_semantics(Node* root){
	vars = g_hash_table_new(g_str_hash, g_str_equal);
	functions = g_hash_table_new(g_str_hash, g_str_equal);
	check_vars(root);
}
