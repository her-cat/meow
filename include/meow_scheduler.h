#ifndef MEOW_SCHEDULER_H
#define MEOW_SCHEDULER_H

#include <uv.h>
#include "meow_coroutine.h"

typedef enum {
    UV_CLOCK_PRECISE = 0,
    UV_CLOCK_FAST = 1
} uv_clocktype_t;

void uv__run_timers(uv_loop_t *loop);
uint64_t uv__hrtime(uv_clocktype_t type);
int uv__next_timeout(const uv_loop_t *loop);

meow_coroutine_t *meow_coroutine_scheduler_module_init();
void meow_coroutine_scheduler_run();

#endif /* MEOW_SCHEDULER_H */
