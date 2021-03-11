#include <assert.h>
#include "meow_scheduler.h"

static void meow_coroutine_scheduler_func(void *data)
{
    meow_coroutine_t *coroutine = MEOW_COROUTINE_G(current);

    MEOW_COROUTINE_G(scheduler) = coroutine;

    meow_coroutine_yield();

    meow_coroutine_scheduler_run();

    MEOW_COROUTINE_G(scheduler) = NULL;
}

meow_coroutine_t *meow_coroutine_scheduler_module_init()
{
    uint32_t last_id;
    meow_coroutine_t *coroutine;

    assert(MEOW_COROUTINE_G(scheduler) == NULL);

    last_id = MEOW_COROUTINE_G(last_id);
    MEOW_COROUTINE_G(last_id) = 0;

    coroutine = meow_coroutine_create(meow_coroutine_scheduler_func, NULL);

    MEOW_COROUTINE_G(last_id) = last_id;

    meow_coroutine_resume(coroutine);

    assert(MEOW_COROUTINE_G(scheduler) != NULL);

    meow_coroutine_get_root()->previous = coroutine;

    return coroutine;
}

void meow_coroutine_scheduler_run()
{
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
