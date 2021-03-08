#include "meow_context.h"

meow_context_t *context;

void task()
{
    printf("task before\n");

    meow_context_swap_out(context);

    printf("task after\n");
}

int main()
{
    context = meow_context_create(task, NULL);

    printf("main before\n");

    meow_context_swap_in(context);

    printf("main after\n");

    meow_context_swap_in(context);

    printf("end\n");

    meow_context_free(context);

    return 1;
}