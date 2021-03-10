cmake_minimum_required(VERSION 3.5)
project(meow C ASM)

set(MEOW_VERSION 0.0.1)
set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

file(GLOB_RECURSE MEOW_INCLUDES FOLLOW_SYMLINKS include/*.h)
file(GLOB_RECURSE MEOW_SOURCES FOLLOW_SYMLINKS src/*.c deps/context/combined.S)

include_directories(BEFORE ./include ./ext-src ./)

#message(STATUS "source=${MEOW_SOURCES}")
#message(STATUS "includes=${MEOW_INCLUDES}")

execute_process(COMMAND php-config --includes OUTPUT_VARIABLE PHP_INCLUDES OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND php-config --extension-dir OUTPUT_VARIABLE PHP_EXTENSION_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PHP_INCLUDES}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PHP_INCLUDES}")

message(STATUS "php_extension_dir=${PHP_EXTENSION_DIR}")

link_directories(${LIBRARY_OUTPUT_PATH})
add_library(lib-meow SHARED ${MEOW_SOURCES})
set_target_properties(lib-meow PROPERTIES OUTPUT_NAME "meow" VERSION ${MEOW_VERSION})

# context
add_executable(context examples/c/context.c)
target_link_libraries(context lib-meow)

# coroutine
add_executable(coroutine examples/c/coroutine.c)
target_link_libraries(coroutine lib-meow)

# coroutine_defer
add_executable(coroutine_defer examples/c/coroutine_defer.c)
target_link_libraries(coroutine_defer lib-meow)
