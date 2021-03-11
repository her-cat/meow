#ifndef MEOW_SCHEDULER_H
#define MEOW_SCHEDULER_H

#include <uv.h>
#include "meow_coroutine.h"

meow_coroutine_t *meow_coroutine_scheduler_module_init();
void meow_coroutine_scheduler_run();

#endif /* MEOW_SCHEDULER_H */
