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
		if(child1_data != NULL && child2_data != NULL && child1_data->type == INTT && child2_data->type == INTT){
			switch (parent_data->type) {
				case ADD:{
					int newVal = *(int*)(child1_data->data) + *(int*)(child2_data->data);
					Node* newNode = makeNode(INTT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
					}
				case SUBTRACT:{
					int newVal = *(int*)(child1_data->data) - *(int*)(child2_data->data);
					Node* newNode = makeNode(INTT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case MULTIPLY:{
					int newVal = *(int*)(child1_data->data) * *(int*)(child2_data->data);
					Node* newNode = makeNode(INTT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case AND:{
					int newVal = *(int*)(child1_data->data) & *(int*)(child2_data->data);
					Node* newNode = makeNode(INTT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case OR:{
					int newVal = *(int*)(child1_data->data) | *(int*)(child2_data->data);
					Node* newNode = makeNode(INTT, &newVal, sizeof(int), NULL);
					replace_node(tree, newNode);
					break;
				}
				case XOR:{
					int newVal = *(int*)(child1_data->data) ^ *(int*)(child2_data->data);
					Node* newNode = makeNode(INTT, &newVal, sizeof(int), NULL);
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