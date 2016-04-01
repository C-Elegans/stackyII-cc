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
#include "semantic_check.h"
#include "codegen.h"
extern int yyparse();
extern FILE* yyin;
int main(int argc,char** argv){
	
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
	
	
}
void process_tree(Node* tree){
	//print_node(tree, 0);
	//collapse_tree(tree);
	printf("Processing tree\n");
	print_node(tree, 0);
	check_semantics(tree);
	primary_codegen(tree);
	print_code();
}