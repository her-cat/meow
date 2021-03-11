#include <assert.h>
#include "meow_coroutine.h"
#include "meow_scheduler.h"

static void task1(void *data)
{
    printf("task1 before\n");

    meow_coroutine_sleep(2);

    printf("task1 after\n");
}

static void task2(void *data)
{
    printf("task2 before\n");

    meow_coroutine_sleep(1);

    printf("task2 after\n");
}

static void task3(void *data)
{
    printf("task3\n");
}

int main()
{
    meow_coroutine_module_init();

    assert(MEOW_COROUTINE_G(current) != NULL);

    meow_coroutine_scheduler_module_init();

    meow_coroutine_run(task1, NULL);
    meow_coroutine_run(task2, NULL);
    meow_coroutine_run(task3, NULL);

    meow_coroutine_scheduler_run();

    return 0;
}
