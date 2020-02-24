#ifndef EXTERN_H
#define EXTERN_H 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Deque_item Deque_item;
typedef struct Deque Deque;
typedef struct Deque_iterator Deque_iterator;

struct Deque_item {
    int32_t value;
    Deque_item *next;
    Deque_item *prev;
};

struct Deque {
    Deque_item *start;
    Deque_item *end;
    int size;
};

struct Deque_iterator {
    Deque_item *ptr;
};

extern void deque_init(Deque *);
extern int deque_size(Deque *);
extern void deque_clear(Deque *);
extern void deque_push_front(int32_t, Deque *);
extern void deque_push_back(int32_t, Deque *);
extern void deque_insert(Deque_iterator, int32_t, Deque *);
extern void deque_pop_front(Deque *);
extern void deque_pop_back(Deque *);
extern void deque_erase(Deque_iterator, Deque *);
extern int32_t deque_front(Deque *);
extern int32_t deque_back(Deque *);
extern void deque_print(Deque *);

#endif //EXTERN_H