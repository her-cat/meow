#ifndef MEOW_CONTEXT_H
#define MEOW_CONTEXT_H

#include "meow_log.h"
#include "meow_type.h"
#include "meow_asm_context.h"

#define MEOW_CONTEXT_DEFAULT_STACK_SIZE (2 * 1024 * 1024)

typedef void (*meow_context_func_t)(void *);

typedef struct meow_context_s {
    void *data;
    char *stack;
    meow_bool_t end;
    uint32_t stack_size;
    meow_asm_context_t ctx;
    meow_asm_context_t swap_ctx;
    meow_context_func_t func;
} meow_context_t;

meow_context_t *meow_context_create(meow_context_func_t func, void *data);
meow_context_t *meow_context_create_ex(meow_context_func_t func, void *data, uint32_t stack_size);
void meow_context_swap_in(meow_context_t *context);
void meow_context_swap_out(meow_context_t *context);
meow_bool_t meow_context_is_end(meow_context_t *context);
void meow_context_free(meow_context_t *context);

#endif /* MEOW_CONTEXT_H */
