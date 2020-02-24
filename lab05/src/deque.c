#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "extern.h"

void deque_init(Deque *d) {
    d->start = (Deque_item *) malloc(sizeof(Deque_item));
    d->end = (Deque_item *) malloc(sizeof(Deque_item));
    d->start->next = d->end;
    d->start->prev = d->end;
    d->end->next = d->start;
    d->end->prev = d->start;
    d->size = 0;
}

int deque_size(Deque *d) {
    return d->size;
}

void deque_clear(Deque *d) {
    if (d->size == 0) {
        printf("Deque is empty.\n");
        return;
    }
    Deque_item *iter = d->start->next;
    int sz_before_dstr = deque_size(d);
    for (int i = 0; i < sz_before_dstr; i++) {
        Deque_item *m = iter;
        iter->prev->next = iter->next;
        iter->next->prev = iter->prev;
        iter = iter->next;
        free(m);
        d->size--;
    }
}

void deque_push_front(int32_t num, Deque *d) {
    d->size++;
    Deque_item *temp = (Deque_item *) malloc(sizeof(Deque_item));
    temp->value = num;
    temp->next = d->start->next;
    d->start->next->prev = temp;
    temp->prev = d->start;
    d->start->next = temp;
}

void deque_push_back(int32_t num, Deque *d) {
    d->size++;
    Deque_item *temp = (Deque_item *) malloc(sizeof(Deque_item));
    temp->value = num;
    d->end->prev->next = temp;
    temp->prev = d->end->prev;
    temp->next = d->end;
    d->end->prev = temp;
}

void deque_insert(Deque_iterator it, int32_t num, Deque *d) {
    d->size++;
    Deque_item *temp = (Deque_item *) malloc(sizeof(Deque_item));
    temp->value = num;
    temp->next = it.ptr->next;
    it.ptr->next->prev = temp;
    temp->prev = it.ptr;
    it.ptr->next = temp;
}

void deque_pop_front(Deque *d) {
    d->size--;
    Deque_item *temp = d->start->next;
    d->start->next = temp->next;
    temp->next->prev = temp->prev;
    free(temp);
}

void deque_pop_back(Deque *d) {
    d->size--;
    Deque_item *temp = d->end->prev;
    d->end->prev = temp->prev;
    temp->prev->next = temp->next;
    free(temp);
}

void deque_erase(Deque_iterator it, Deque *d) {
    d->size--;
    Deque_item *temp = it.ptr;
    it.ptr->prev->next = it.ptr->next;
    it.ptr->next->prev = it.ptr->prev;
    free(temp);
}

int32_t deque_front(Deque *d) {
    if (d->size == 0) {
        printf("Deque is empty.\n");
        return 0;
    }
    return d->start->next->value;
}

int32_t deque_back(Deque *d) {
    if (d->size == 0) {
        printf("Deque is empty.\n");
        return 0;
    }
    return d->end->prev->value;
}

void deque_print(Deque *d) {
    if (d->size == 0) {
        printf("Deque is empty.\n");
        return;
    }
    Deque_item *temp = d->start->next;
    for (int i = 0; i < d->size; i++) {
        printf("%d ", temp->value);
        temp = temp->next;
    }
    printf("\n");
}