/* Copyright (c) 2013, Ben Noordhuis <info@bnoordhuis.nl>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef MEOW_MEOW_QUEUE_H
#define MEOW_MEOW_QUEUE_H

#include <stddef.h>

typedef void *meow_queue_t[2];
typedef void *meow_queue_node_t[2];

/* Private macros. */
#define MEOW_QUEUE_NEXT(q)       (*(meow_queue_t **) &((*(q))[0]))
#define MEOW_QUEUE_PREV(q)       (*(meow_queue_t **) &((*(q))[1]))
#define MEOW_QUEUE_PREV_NEXT(q)  (MEOW_QUEUE_NEXT(MEOW_QUEUE_PREV(q)))
#define MEOW_QUEUE_NEXT_PREV(q)  (MEOW_QUEUE_PREV(MEOW_QUEUE_NEXT(q)))

/* Public macros. */
#define MEOW_QUEUE_DATA(ptr, type, field)                                          \
  ((type *) ((char *) (ptr) - offsetof(type, field)))

/* Important note: mutating the list while MEOW_QUEUE_FOREACH is
 * iterating over its elements results in undefined behavior.
 */
#define MEOW_QUEUE_FOREACH(q, h)                                                   \
  for ((q) = MEOW_QUEUE_NEXT(h); (q) != (h); (q) = MEOW_QUEUE_NEXT(q))

#define MEOW_QUEUE_EMPTY(q)                                                        \
  ((const meow_queue_t *) (q) == (const meow_queue_t *) MEOW_QUEUE_NEXT(q))

#define MEOW_QUEUE_HEAD(q)                                                         \
  (MEOW_QUEUE_NEXT(q))

#define MEOW_QUEUE_INIT(q)                                                         \
  do {                                                                        \
    MEOW_QUEUE_NEXT(q) = (q);                                                      \
    MEOW_QUEUE_PREV(q) = (q);                                                      \
  }                                                                           \
  while (0)

#define MEOW_QUEUE_ADD(h, n)                                                       \
  do {                                                                        \
    MEOW_QUEUE_PREV_NEXT(h) = MEOW_QUEUE_NEXT(n);                                       \
    MEOW_QUEUE_NEXT_PREV(n) = MEOW_QUEUE_PREV(h);                                       \
    MEOW_QUEUE_PREV(h) = MEOW_QUEUE_PREV(n);                                            \
    MEOW_QUEUE_PREV_NEXT(h) = (h);                                                 \
  }                                                                           \
  while (0)

#define MEOW_QUEUE_SPLIT(h, q, n)                                                  \
  do {                                                                        \
    MEOW_QUEUE_PREV(n) = MEOW_QUEUE_PREV(h);                                            \
    MEOW_QUEUE_PREV_NEXT(n) = (n);                                                 \
    MEOW_QUEUE_NEXT(n) = (q);                                                      \
    MEOW_QUEUE_PREV(h) = MEOW_QUEUE_PREV(q);                                            \
    MEOW_QUEUE_PREV_NEXT(h) = (h);                                                 \
    MEOW_QUEUE_PREV(q) = (n);                                                      \
  }                                                                           \
  while (0)

#define MEOW_QUEUE_MOVE(h, n)                                                      \
  do {                                                                        \
    if (MEOW_QUEUE_EMPTY(h))                                                       \
      MEOW_QUEUE_INIT(n);                                                          \
    else {                                                                    \
      meow_queue_t* q = MEOW_QUEUE_HEAD(h);                                               \
      MEOW_QUEUE_SPLIT(h, q, n);                                                   \
    }                                                                         \
  }                                                                           \
  while (0)

#define MEOW_QUEUE_INSERT_HEAD(h, q)                                               \
  do {                                                                        \
    MEOW_QUEUE_NEXT(q) = MEOW_QUEUE_NEXT(h);                                            \
    MEOW_QUEUE_PREV(q) = (h);                                                      \
    MEOW_QUEUE_NEXT_PREV(q) = (q);                                                 \
    MEOW_QUEUE_NEXT(h) = (q);                                                      \
  }                                                                           \
  while (0)

#define MEOW_QUEUE_INSERT_TAIL(h, q)                                               \
  do {                                                                        \
    MEOW_QUEUE_NEXT(q) = (h);                                                      \
    MEOW_QUEUE_PREV(q) = MEOW_QUEUE_PREV(h);                                            \
    MEOW_QUEUE_PREV_NEXT(q) = (q);                                                 \
    MEOW_QUEUE_PREV(h) = (q);                                                      \
  }                                                                           \
  while (0)

#define MEOW_QUEUE_REMOVE(q)                                                       \
  do {                                                                        \
    MEOW_QUEUE_PREV_NEXT(q) = MEOW_QUEUE_NEXT(q);                                       \
    MEOW_QUEUE_NEXT_PREV(q) = MEOW_QUEUE_PREV(q);                                       \
  }                                                                           \
  while (0)

#define MEOW_QUEUE_FRONT_DATA(queue, type, field) \
    ((!MEOW_QUEUE_EMPTY(queue)) ? MEOW_QUEUE_DATA(MEOW_QUEUE_NEXT(queue), type, field) : NULL)

#endif /* MEOW_MEOW_QUEUE_H */
