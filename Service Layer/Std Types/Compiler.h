#ifndef COMPILER_H
#define COMPILER_H


#define NULL_PTR  ((void*)0)

/* used to inline assembly code with a c program */
#define ASM		  __asm

#define DISABLE_INTERRUPT() 	ASM("CPSID i;");
#define ENABLE_INTERRUPT()	ASM("CPSIE	i;");


#endif