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
#define ERROR(str) do{ fprintf(stderr,str);exit(-1);} while(0)
static GHashTable* globalvars;
static GList* functions = NULL;
static GHashTable* func_table;
static GHashTable* func_vars;
GList* instructions = NULL;
typedef struct{
	enum asm_op op;
	uint16_t data;
	int flags;
}instruction;
typedef struct{
	GList* list;
	GHashTable* vars;
	int frame_pointer;
}f_data;
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
	f_data* data = (f_data*)d;
	switch (n_data->type) {
	  	case T_INT:
			add_inst(Push, *(int*)(n_data->data), data->list);
			break;
		case T_ADD:
			add_inst(Add, 0, data->list);
			break;
		case T_SUBTRACT:
			add_inst(Sub, 0, data->list);
			break;
		case T_AND:
			add_inst(And, 0, data->list);
			break;
		case T_OR:
			add_inst(Or,0,data->list);
			break;
		case T_XOR:
			add_inst(Xor, 0, data->list);
			break;
		case T_EQ:
			add_inst(Eq,0,data->list);
			break;
		case T_NE:
			ERROR("Unsupported instruction NE\n");
			break;
		case T_LE:
			ERROR("Unsupported instruction LE\n");
			break;
		case T_GE:
			ERROR("Unsupported instruction GE\n");
			break;
		case T_GT:
			add_inst(Gt,0,data->list);
			break;
		case T_LT:
			add_inst(Lt,0,data->list);
			break;
		case T_PRINT:
			add_inst(Out,0,data->list);
			break;
		case T_VAR:{
			Node* id_node = g_node_first_child(tree);
			char* id = get_node_data(id_node)->data;
			if(id==NULL)ERROR("Null pointer when referencing id!\n");
			int offset = GPOINTER_TO_INT( g_hash_table_lookup(data->vars, id));
			add_inst(Local, offset, data->list);
			break;
		}
		case T_ASSIGN:{
			char* id = get_node_data(g_node_first_child(tree))->data;
			int offset = GPOINTER_TO_INT( g_hash_table_lookup(data->vars, id));
			add_inst(Slocal, offset, data->list);
			break;
		}
	  default:
			break;
	}
	return FALSE;
}
gboolean extract_vardecs(Node* tree, gpointer d){
	node_data* n_data = tree->data;
	f_data* data = (f_data*)d;
	if(n_data->type == T_VARDEC){
		Node* id_node = g_node_nth_child(tree, 1);
		char* id = get_node_data(id_node)->data;
		printf("Vardec: %s\n",id);
		g_hash_table_insert(data->vars, id, GINT_TO_POINTER(data->frame_pointer++));
	}
	return FALSE;
}
gboolean generate(Node* tree, gpointer d){
	node_data* n_data = tree->data;
	if(n_data->type == T_ROOT)return FALSE;
	if(n_data->type == T_FUNCDEF){
		char* id =((node_data*)(g_node_first_child(tree)->data))->data;
		functions = g_list_prepend(functions, id);
		f_data data;
		data.list = NULL;
		data.vars = g_hash_table_new(g_str_hash, g_str_equal);
		data.frame_pointer = 0;
		g_node_traverse(tree, G_POST_ORDER, G_TRAVERSE_ALL, -1, &extract_vardecs, &data);
		add_inst(Frame, data.frame_pointer, data.list);
		g_node_traverse(tree, G_POST_ORDER, G_TRAVERSE_ALL, -1, &generate_function, &data);
		add_inst(Leave, 0, data.list);
		add_inst(Ret, 0, data.list);
		g_hash_table_insert(func_table, id, data.list);
		g_hash_table_insert(func_vars,id,data.vars);
	}else{
		generate_global(tree, NULL);
	}
	return FALSE;
}
void primary_codegen(Node* tree){
	globalvars = g_hash_table_new(g_str_hash, g_str_equal);
	func_table = g_hash_table_new(g_str_hash, g_str_equal);
	func_vars = g_hash_table_new(g_str_hash, g_str_equal);
	g_node_traverse(tree, G_POST_ORDER, G_TRAVERSE_ALL, 2, &generate, NULL);
	instructions = g_list_reverse(instructions);
	g_hash_table_destroy(globalvars);
}
void item_print(gpointer data, gpointer d){
	instruction* inst = (instruction*) data;
	enum asm_op op = inst->op;
	if(op == Push||op == Local||op==Slocal||op==Frame){
		printf("\t%s %d\n",asm_op_name_table[inst->op],inst->data);
	}else{
		printf("\t%s\n",asm_op_name_table[inst->op]);
	}
}
void func_print(gpointer data, gpointer d){
	char* id = data;
	GList* func = g_hash_table_lookup(func_table, id);
	func = g_list_reverse(func);
	printf("%s:\n",id);
	g_list_foreach(func, &item_print, NULL);
}
void print_code(){
	g_list_foreach(instructions, &item_print, NULL);
	g_list_foreach(functions, &func_print, NULL);
}
void delete_inst(gpointer data, gpointer d){
	free(data);
}
void delete_func(gpointer data, gpointer d){
	g_list_foreach(g_hash_table_lookup(func_table, data), &delete_inst, NULL);
	
	
}
void delete_code(){
	g_list_foreach(functions, &delete_func, NULL);
}