#ifndef MEOW_BUILTIN_H
#define MEOW_BUILTIN_H

#define MEOW_GLOBALS_STRUCT(name)           name##_globals_s
#define MEOW_GLOBALS_TYPE(name)             name##_globals_t
#define MEOW_GLOBALS(name)                  name##_globals

#define MEOW_GLOBALS_STRUCT_BEGIN(name)     typedef struct MEOW_GLOBALS_STRUCT(name) {
#define MEOW_GLOBALS_STRUCT_END(name)       } MEOW_GLOBALS_TYPE(name);

#define MEOW_GLOBALS_DECLARE(name)          MEOW_GLOBALS_TYPE(name) MEOW_GLOBALS(name);
#define MEOW_GLOBALS_GET(name, value)       MEOW_GLOBALS(name).value

#endif /* MEOW_BUILTIN_H */
