//
//  tree_optimize.c
//  stackyII-cc
//
//  Created by Michael Nolan on 4/1/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#include "tree_optimize.h"
gboolean constexpr_elim(Node* tree, gpointer d){
	node_data* parent_data = (node_data*)tree->data;
	Node* child1 = g_node_first_child(tree);
	Node* child2 = g_node_nth_child(tree, 1);
	if(child1 != NULL && child2 != NULL){
		node_data* child1_data = (node_data*)child1->data;
		node_data* child2_data = (node_data*)child2->data;
		if(child1_data != NULL && child2_data != NULL && child1_data->type == T_INT && child2_data->type == T_INT){
			switch (parent_data->type) {
				case T_ADD:{
					int newVal = *(int*)(child1_data->data) + *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
					}
				case T_SUBTRACT:{
					int newVal = *(int*)(child1_data->data) - *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_MULTIPLY:{
					int newVal = *(int*)(child1_data->data) * *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_DIVIDE:{
					int newVal = *(int*)(child1_data->data) / *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_AND:{
					int newVal = *(int*)(child1_data->data) & *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_OR:{
					int newVal = *(int*)(child1_data->data) | *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_XOR:{
					int newVal = *(int*)(child1_data->data) ^ *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_EQ:{
					int newVal = *(int*)(child1_data->data) == *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_NE:{
					int newVal = *(int*)(child1_data->data) != *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_GT:{
					int newVal = *(int*)(child1_data->data) > *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_LT:{
					int newVal = *(int*)(child1_data->data) < *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_GE:{
					int newVal = *(int*)(child1_data->data) >= *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case T_LE:{
					int newVal = *(int*)(child1_data->data) <= *(int*)(child2_data->data);
					Node* newNode = makeNode(T_INT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				default:
					break;
			}
		}
	}
	return FALSE;
}
void optimize_tree(Node* tree){
	g_node_traverse(tree, G_POST_ORDER, G_TRAVERSE_ALL, -1, &constexpr_elim, NULL);
}