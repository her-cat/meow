#include <stdlib.h>
#include "meow_coroutine.h"

MEOW_GLOBALS_DECLARE(meow_coroutine);

void meow_coroutine_module_init()
{
    MEOW_COROUTINE_G(last_id) = 0;
    MEOW_COROUTINE_G(stack_size) = MEOW_CONTEXT_DEFAULT_STACK_SIZE;

    meow_coroutine_t *main_coroutine = &MEOW_COROUTINE_G(_main);
    main_coroutine->id = MEOW_COROUTINE_MAIN_ID;
    main_coroutine->state = MEOW_COROUTINE_STATE_READY;
    main_coroutine->previous = NULL;
    main_coroutine->context = NULL;

    MEOW_COROUTINE_G(main) = main_coroutine;
    MEOW_COROUTINE_G(current) = main_coroutine;
    MEOW_COROUTINE_G(last_id) = main_coroutine->id + 1;
    MEOW_COROUTINE_G(scheduler) = NULL;
}

meow_coroutine_t *meow_coroutine_create(meow_coroutine_func_t func, void *data)
{
    return meow_coroutine_create_ex(func, data, 0);
}

meow_coroutine_t *meow_coroutine_create_ex(meow_coroutine_func_t func, void *data, uint32_t stack_size)
{
    meow_coroutine_t *coroutine;

    coroutine = (meow_coroutine_t *) malloc(sizeof(meow_coroutine_t));
    if (coroutine == NULL) {
        meow_warn("Create coroutine failed: (errno %d) %s", errno, strerror(errno))
        return NULL;
    }

    coroutine->previous = NULL;
    coroutine->id = MEOW_COROUTINE_G(last_id)++;
    coroutine->state = MEOW_COROUTINE_STATE_READY;

    if (stack_size == 0) {
        stack_size = MEOW_COROUTINE_G(stack_size);
    }

    coroutine->context = meow_context_create_ex((meow_context_func_t) func, data, stack_size);
    if (coroutine->context == NULL) {
        free(coroutine);
        return NULL;
    }

    return coroutine;
}

meow_coroutine_t *meow_coroutine_get_current()
{
    return MEOW_COROUTINE_G(current);
}

meow_coroutine_t *meow_coroutine_get_root()
{
    return meow_coroutine_get_by_index(0);
}

meow_coroutine_t *meow_coroutine_get_by_index(uint32_t index)
{
    uint32_t count = 0;
    meow_coroutine_t *coroutine  = MEOW_COROUTINE_G(current);

    while (!coroutine->previous) {
        count++;
        coroutine = coroutine->previous;
    }

    if (index == 0) {
        return coroutine;
    } else if (index > count) {
        return NULL;
    }

    coroutine = MEOW_COROUTINE_G(current);
    count -= index;

    while (count--) {
        coroutine = coroutine->previous;
    }

    return coroutine;
}

meow_bool_t meow_coroutine_is_alive(meow_coroutine_t *coroutine)
{
    return coroutine->state > MEOW_COROUTINE_STATE_READY && coroutine->state < MEOW_COROUTINE_STATE_FINISHED;
}

meow_bool_t meow_coroutine_is_resumable(meow_coroutine_t *coroutine)
{
    meow_coroutine_t *current = MEOW_COROUTINE_G(current);

    if (current->previous == coroutine) {
        return meow_true;
    }

    switch (coroutine->state) {
        case MEOW_COROUTINE_STATE_READY:
        case MEOW_COROUTINE_STATE_WAITING:
            return meow_true;
        case MEOW_COROUTINE_STATE_RUNNING:
        case MEOW_COROUTINE_STATE_DEAD:
        default:
            return meow_false;
    }
}

meow_bool_t meow_coroutine_resume(meow_coroutine_t *coroutine)
{
    if (!meow_coroutine_is_resumable(coroutine)) {
        meow_warn("Coroutine can not resumable")
        return meow_false;
    }

    coroutine->state = MEOW_COROUTINE_STATE_RUNNING;
    coroutine->previous = MEOW_COROUTINE_G(current);
    MEOW_COROUTINE_G(current) = coroutine;

    meow_context_swap_in(coroutine->context);

    /* TODO: yield current coroutine? The scheduler has not been implemented yet. Now yield will have problems. */

    if (meow_context_is_finished(coroutine->context)) {
        MEOW_COROUTINE_G(current) = coroutine->previous;
        coroutine->state = MEOW_COROUTINE_STATE_FINISHED;
        meow_coroutine_close(coroutine);
    }

    return meow_true;
}

meow_bool_t meow_coroutine_yield()
{
    meow_coroutine_t *coroutine = MEOW_COROUTINE_G(current);

    coroutine->state = MEOW_COROUTINE_STATE_WAITING;
    MEOW_COROUTINE_G(current) = coroutine->previous;
    meow_context_swap_out(coroutine->context);

    return meow_true;
}

meow_bool_t meow_coroutine_close(meow_coroutine_t *coroutine)
{
    if (!meow_context_is_finished(coroutine->context) || meow_coroutine_is_alive(coroutine)) {
        meow_warn("Coroutine[%d] is still alive! Unable to close", coroutine->id)
        return meow_false;
    }

    meow_context_free(coroutine->context);
    free(coroutine);

    return meow_true;
}

meow_coroutine_t *meow_coroutine_run(meow_coroutine_func_t func, void *data)
{
    meow_coroutine_t *coroutine;

    coroutine = meow_coroutine_create(func, data);
    if (coroutine == NULL) {
        return NULL;
    }

    if (!meow_coroutine_resume(coroutine)) {
        meow_coroutine_close(coroutine);
        return NULL;
    }

    return coroutine;
}
