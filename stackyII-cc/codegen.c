//
//  codegen.c
//  stackyII-cc
//
//  Created by Michael Nolan on 3/31/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#include "codegen.h"
#include <stdlib.h>
#undef ENUM_BEGIN
#undef ENUM
#undef ENUM_END
#define ENUM_BEGIN(typ) const char * typ ## _name_table [] = {
#define ENUM(nam) #nam
#define ENUM_END(typ) };
#undef ENUM_DEFINED_ASM_OP
#include "codegen.h"
#define add_inst(opcode,instruction_data,list) do{  \
instruction* i = malloc(sizeof(instruction)); \
i->op = opcode;		\
i->data = instruction_data; \
list = g_list_prepend(list,i); \
}while(0)
static GHashTable* globalvars;
static GList* functions = NULL;
static GHashTable* func_table;

GList* instructions = NULL;
typedef struct{
	enum asm_op op;
	uint16_t data;
	int flags;
}instruction;
int frame = 0;
void add_global_variable(char* var){
	frame++;
	
	g_hash_table_insert(globalvars, var, GINT_TO_POINTER(frame) );
}
gboolean generate_global(Node* tree,gpointer d){
	node_data* n_data = tree->data;
	if(n_data->type == T_HEAD){
		Node* assign = g_node_nth_child(tree, 1);
		Node* id_node = g_node_first_child(assign);
		char* id = ((node_data*)id_node->data)->data;
		add_global_variable(id);
	}
	return FALSE;
}
gboolean generate_function(Node* tree, gpointer d){
	node_data* n_data = tree->data;
	GList** f_list = (GList**)d;
	switch (n_data->type) {
	  case T_INT:
			add_inst(Push, *(int*)(n_data->data), *f_list);
			break;
			
	  default:
			break;
	}
	return FALSE;
}
gboolean generate(Node* tree, gpointer d){
	node_data* n_data = tree->data;
	if(n_data->type == T_ROOT)return FALSE;
	if(n_data->type == T_FUNCDEF){
		char* id =((node_data*)(g_node_first_child(tree)->data))->data;
		functions = g_list_prepend(functions, id);
		GList* f_list = NULL;
		
		g_node_traverse(tree, G_POST_ORDER, G_TRAVERSE_ALL, -1, &generate_function, &f_list);
		g_hash_table_insert(func_table, id, f_list);
	}else{
		generate_global(tree, NULL);
	}
	return FALSE;
}
void primary_codegen(Node* tree){
	globalvars = g_hash_table_new(g_str_hash, g_str_equal);
	func_table = g_hash_table_new(g_str_hash, g_str_equal);
	g_node_traverse(tree, G_POST_ORDER, G_TRAVERSE_ALL, 2, &generate, NULL);
	instructions = g_list_reverse(instructions);
	g_hash_table_destroy(globalvars);
}
void item_print(gpointer data, gpointer d){
	instruction* inst = (instruction*) data;
	enum asm_op op = inst->op;
	if(op == Push||op == Local||op==Slocal){
		printf("%s %d\n",asm_op_name_table[inst->op],inst->data);
	}else{
		printf("%s\n",asm_op_name_table[inst->op]);
	}
}
void func_print(gpointer data, gpointer d){
	char* id = data;
	GList* func = g_hash_table_lookup(func_table, id);
	printf("Func: %s\n",id);
	g_list_foreach(func, &item_print, NULL);
}
void print_code(){
	g_list_foreach(instructions, &item_print, NULL);
	g_list_foreach(functions, &func_print, NULL);
}