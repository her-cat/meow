#include <stdlib.h>
#include "meow_context.h"

static void meow_context_func(void *data)
{
    meow_context_t *context = (meow_context_t *) data;

    context->func(context->data);
    context->finished = meow_true;

    meow_context_swap_out(context);
}

meow_context_t *meow_context_create(meow_context_func_t func, void *data)
{
    return meow_context_create_ex(func, data, 0);
}

meow_context_t *meow_context_create_ex(meow_context_func_t func, void *data, uint32_t stack_size)
{
    void *sp;
    meow_context_t *context;

    context = (meow_context_t *) malloc(sizeof(meow_context_t));
    if (context == NULL) {
        meow_warn("Create context failed: (errno %d) %s", errno, strerror(errno))
        return NULL;
    }

    if (stack_size == 0) {
        stack_size = MEOW_CONTEXT_DEFAULT_STACK_SIZE;
    }

    context->data = data;
    context->func = func;
    context->finished = meow_false;
    context->stack_size = stack_size;
    context->stack = (char *) malloc(stack_size);

    if (context->stack == NULL) {
        free(context);
        meow_warn("Create stack failed: (errno %d) %s", errno, strerror(errno))
        return NULL;
    }

    /* 将指针移动到堆栈底部 */
    sp = (void *) ((char *) context->stack + context->stack_size);

    context->ctx = meow_asm_context_make(sp, context->stack_size, (void (*)(intptr_t)) meow_context_func);

    return context;
}

void meow_context_swap_in(meow_context_t *context)
{
    meow_asm_context_jump(&context->swap_ctx, context->ctx, (intptr_t) context, meow_true);
}

void meow_context_swap_out(meow_context_t *context)
{
    meow_asm_context_jump(&context->ctx, context->swap_ctx, (intptr_t) context, meow_true);
}

meow_bool_t meow_context_is_finished(meow_context_t *context)
{
    return context->finished;
}

void meow_context_free(meow_context_t *context)
{
    if (!meow_context_is_finished(context)) {
        return;
    }

    free(context->stack);
    free(context);
}
