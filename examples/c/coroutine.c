#include "meow_coroutine.h"

static void task(void *data)
{
    char *id = (char *) data;

    printf("task%s before\n", id);

    meow_coroutine_yield();

    printf("task%s after\n", id);
}

int main()
{
    meow_coroutine_t *coroutine1, *coroutine2;

    meow_coroutine_module_init();

    coroutine1 = meow_coroutine_run(task, "1");
    coroutine2 = meow_coroutine_run(task, "2");

    meow_coroutine_resume(coroutine1);
    meow_coroutine_resume(coroutine2);

    return 0;
}
