//
//  codegen.h
//  stackyII-cc
//
//  Created by Michael Nolan on 3/31/16.
//  Copyright © 2016 Michael Nolan. All rights reserved.
//

#ifndef codegen_h
#define codegen_h

#include <stdio.h>
#include "ast.h"

#define ENUM_BEGIN(typ) enum typ {
#define ENUM(nam) nam
#define ENUM_END(typ) };



void print_code();
void primary_codegen(Node* tree);
typedef enum{F_VOLATILE=1} var_flags;
#endif /* codegen_h */
ENUM_BEGIN( asm_op )
#ifndef ENUM_DEFINED_ASM_OP
#define ENUM_DEFINED_ASM_OP
#define ASM_OP(ENUM)
ENUM(Nop),
ENUM(Add),
ENUM(Sub),
ENUM(Out),
ENUM(In),
ENUM(Dup),
ENUM(Drop),
ENUM(Swap),
ENUM(Rot),
ENUM(Ret),
ENUM(Lt),
ENUM(Eq),
ENUM(Gt),
ENUM(Neg),
ENUM(Leave),
ENUM(And),
ENUM(Or),
ENUM(Xor),
ENUM(Lshift),
ENUM(Rshift),
ENUM(Mul),
ENUM(Fetch),
ENUM(Store),
ENUM(Frame),
ENUM(Local),
ENUM(Slocal),
ENUM(Jump),
ENUM(Call),
ENUM(CJump),
ENUM(Push)
ENUM_END( Numbers )
#endif