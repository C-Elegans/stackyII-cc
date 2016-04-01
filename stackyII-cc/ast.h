//
//  ast.h
//  stackyII-cc
//
//  Created by Michael Nolan on 3/29/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//
#ifndef ast_h
#define ENUM_BEGIN(typ) enum typ {
#define ENUM(nam) T_ ## nam
#define ENUM_END(typ) };
#endif
#ifndef ENUM_GENERATED
#define ENUM_GENERATED
ENUM_BEGIN(n_type)
ENUM(ROOT),
ENUM(HEAD),
ENUM(ASSIGN),
ENUM(VARDEC),
ENUM(EXPR),
ENUM(IDENTIFIER),
ENUM(INT),
ENUM(ADD),
ENUM(SUBTRACT),
ENUM(MULTIPLY),
ENUM(DIVIDE),
ENUM(AND),
ENUM(OR),
ENUM(XOR),
ENUM(EQUALS),
ENUM(SHL),
ENUM(SHR),
ENUM(FUNCDEF),
ENUM(FUNCVARS),
ENUM(VOIDDEC),
ENUM(INTDEC),
ENUM(BLOCK),
ENUM(FUNCCALL),
ENUM(FUNCPARS),
ENUM(FUNCDECL),
ENUM(VAR),
ENUM(EQ),
ENUM(NE),
ENUM(LT),
ENUM(GT),
ENUM(LE),
ENUM(GE),
ENUM(FOR),
ENUM(ENUM_END),
ENUM_END(n_type)
#endif
#ifndef ast_h
#define ast_h

#include <stdio.h>
#include <glib.h>

typedef enum n_type nodetype;

typedef GNode Node;
typedef struct{
	nodetype type;
	void* data;
} node_data;
Node* makeNode(nodetype type,void* data, size_t datasize,Node* child,...);
void print_node(Node* tree,int depth);
void replace_node(Node* initial,Node* replacement);
void delete_node_recursive(Node* node);
#endif /* ast_h */
