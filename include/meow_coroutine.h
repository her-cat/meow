#ifndef MEOW_COROUTINE_H
#define MEOW_COROUTINE_H

#include <stdint.h>
#include "meow_queue.h"
#include "meow_builtin.h"
#include "meow_context.h"

#define MEOW_COROUTINE_MAIN_ID      1
#define MEOW_COROUTINE_G(field)     MEOW_GLOBALS_GET(meow_coroutine, field)

typedef void (*meow_coroutine_func_t)(void *);
typedef struct meow_coroutine_s meow_coroutine_t;

#define MEOW_COROUTINE_STATE_MAP(XX) \
    XX(READY, 0) \
    XX(RUNNING, 1) \
    XX(WAITING, 2) \
    XX(FINISHED, 3) \
    XX(DEAD, 4) \

typedef enum {
#define MEOW_COROUTINE_STATE_GEN(name, value) MEOW_COROUTINE_STATE_##name = value,
    MEOW_COROUTINE_STATE_MAP(MEOW_COROUTINE_STATE_GEN)
#undef MEOW_COROUTINE_STATE_GEN
} meow_coroutine_state_t;

/* TODO: defer function */

struct meow_coroutine_s {
    uint32_t id;
    meow_queue_t node;
    meow_context_t *context;
    meow_coroutine_t *previous;
    meow_coroutine_state_t state;
};

MEOW_GLOBALS_STRUCT_BEGIN(meow_coroutine)
    uint32_t last_id;
    uint32_t stack_size;
    meow_coroutine_t *current;
    meow_coroutine_t *main;
    meow_coroutine_t _main;
    meow_coroutine_t *scheduler;
MEOW_GLOBALS_STRUCT_END(meow_coroutine)

extern MEOW_GLOBALS_DECLARE(meow_coroutine)

void meow_coroutine_module_init();

meow_coroutine_t *meow_coroutine_create(meow_coroutine_func_t func, void *data);
meow_coroutine_t *meow_coroutine_create_ex(meow_coroutine_func_t func, void *data, uint32_t stack_size);

meow_coroutine_t *meow_coroutine_get_root();
meow_coroutine_t *meow_coroutine_get_current();
meow_coroutine_t *meow_coroutine_get_by_index(uint32_t index);

meow_bool_t meow_coroutine_is_alive(meow_coroutine_t *coroutine);
meow_bool_t meow_coroutine_is_resumable(meow_coroutine_t *coroutine);

meow_bool_t meow_coroutine_yield();
meow_bool_t meow_coroutine_resume(meow_coroutine_t *coroutine);
meow_bool_t meow_coroutine_close(meow_coroutine_t *coroutine);

meow_coroutine_t *meow_coroutine_run(meow_coroutine_func_t func, void *data);

#endif /* MEOW_COROUTINE_H */
