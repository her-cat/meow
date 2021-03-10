#include <assert.h>
#include "meow_coroutine.h"

static void defer_task(void *data)
{
    printf("defer_task%s\n", (char *) data);
}

static void task(void *data)
{
    printf("task before\n");

    meow_coroutine_defer(defer_task, "1");

    printf("task after\n");

    meow_coroutine_defer(defer_task, "2");
}

int main()
{
    meow_coroutine_t *coroutine;

    meow_coroutine_module_init();

    coroutine = meow_coroutine_run(task, NULL);

    assert(MEOW_QUEUE_EMPTY(&coroutine->defer_tasks));

    return 0;
}
