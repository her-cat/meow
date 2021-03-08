#ifndef MEOW_ASM_CONTEXT_H
#define MEOW_ASM_CONTEXT_H

#include <stdint.h>
#include <stddef.h>

typedef void * meow_asm_context_t;

intptr_t meow_asm_context_jump(meow_asm_context_t *current, meow_asm_context_t *target, intptr_t vp, uint8_t preserve_fpu);
meow_asm_context_t meow_asm_context_make(void *sp, size_t size, void (*fn)(intptr_t));

#endif /* MEOW_ASM_CONTEXT_H */
