//
//  main.c
//  stackyII-cc
//
//  Created by Michael Nolan on 3/29/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "y.tab.h"
AstNode* tree_head;
extern int yyparse();
extern FILE* yyin;
int main(int argc,char** argv){
	
	tree_head = new_tail_node(HEADT, NULL, 0);
	if(argc != 2){
		fprintf(stderr, "Usage stackyII-cc [file]\n");
		exit(-1);
	}
	FILE* f = fopen(argv[1], "r");
	if(f == NULL){
		fprintf(stderr, "Error opening file %s\n",argv[1]);
		exit(-1);
	}
	yyin = f;
	do {
		yyparse();
	} while (!feof(yyin));
	
	print_node(tree_head,0);
}
