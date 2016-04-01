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
#define add_inst(opcode,instruction_data) do{  \
instruction* i = malloc(sizeof(instruction)); \
i->op = opcode;		\
i->data = instruction_data; \
instructions = g_list_prepend(instructions,i); \
}while(0)
static GHashTable* vars;
GList* instructions = NULL;
typedef struct{
	enum asm_op op;
	uint16_t data;
	int flags;
}instruction;
int frame = 0;
void add_variable(char* var){
	frame++;
	
	g_hash_table_insert(vars, var, GINT_TO_POINTER(frame) );
}
gboolean generate_node(Node* tree,gpointer d){
	node_data* n_data = (node_data*)tree->data;
	switch (n_data->type) {
		
		case INTT:
			add_inst(Push, *(uint16_t*)(n_data->data));
			break;
		case VARDECT:{
				Node* id = g_node_nth_child(tree, 1);
				node_data* id_data = (node_data*)id->data;
				add_variable((char*)id_data->data);
			break;
			}
		case VAR:{
				Node * id = g_node_first_child(tree);
				node_data* id_data = (node_data*)id->data;
				int offset = GPOINTER_TO_INT( g_hash_table_lookup(vars, (char*)id_data->data));
				add_inst(Local, offset);
			break;
			}
		case ADD:
			add_inst(Add, 0);
			break;
		case SUBTRACT:
			add_inst(Sub, 0);
			break;
		case MULTIPLY:
			add_inst(Mul, 0);
			break;
		case AND:
			add_inst(And,0);break;
		case OR:
			add_inst(Or, 0);break;
		case XOR:
			add_inst(Xor,0);break;
		case ASSIGNT:{
			Node * id = g_node_first_child(tree);
			node_data* id_data = (node_data*)id->data;
			int offset = GPOINTER_TO_INT( g_hash_table_lookup(vars, (char*)id_data->data));
			add_inst(Slocal, offset);
			break;
		}
		default:
			break;
	}
	return FALSE;
}
void primary_codegen(Node* tree){
	vars = g_hash_table_new(g_str_hash, g_str_equal);
	g_node_traverse(tree, G_POST_ORDER, G_TRAVERSE_ALL, -1, &generate_node, NULL);
	instructions = g_list_reverse(instructions);
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
void print_code(){
	g_list_foreach(instructions, &item_print, NULL);
}