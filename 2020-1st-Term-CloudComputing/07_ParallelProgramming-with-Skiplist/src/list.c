//
//  list.c
//  skiplist
//
//  Created by User on 28/04/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include "list.h"

void init_list_head(struct LIST_HEAD_T *list) {
    list->next = list;
    list->prev = list;
}

static void __list_add(struct LIST_HEAD_T *p_head,
                       struct LIST_HEAD_T *prev,
                       struct LIST_HEAD_T *next)
{
    next->prev = p_head;
    p_head->next = next;
    p_head->prev = prev;
    prev->next = p_head;
}

void list_add(struct LIST_HEAD_T *new_head,
              struct LIST_HEAD_T *p_head)
{
    __list_add(new_head, p_head->prev, p_head->next);
}

static inline void __list_del(struct LIST_HEAD_T *prev,
                       struct LIST_HEAD_T *next)
{
    next->prev = prev;
    prev->next = next;
}

void list_del(struct LIST_HEAD_T *p_head)
{
    __list_del(p_head->prev, p_head->next);
    p_head->next = 0;
    p_head->prev = 0;
}

int list_empty(struct LIST_HEAD_T *p_head)
{
    return p_head->next == p_head;
}
