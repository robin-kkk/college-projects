//
//  list.h
//  skiplist
//
//  Created by User on 28/04/2020.
//  Copyright © 2020 User. All rights reserved.
//

#ifndef list_h
#define list_h

struct LIST_HEAD_T {
    struct LIST_HEAD_T *next, *prev;
};

#ifdef _WIN32
#define CONTAINING_RECORD(address, type, field) (\
(type *)((char*)(address) - offsetof(type, field)))
// (type *)((char*)(address) - (unsigned long)(&((type *)0)->field)))
#define list_entry(address, type, field) \
CONTAINING_RECORD(address, type, field)

#else
#define container_of(ptr, type, member) ({ \
const typeof(((type *)0)->member) * __mptr = (ptr); \
(type *)((char *)__mptr - offsetof(type, member)); })

#define list_entry(ptr, type, member) container_of(ptr, type, member)
#endif

#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)

void init_list_head(struct LIST_HEAD_T *list);
void list_add(struct LIST_HEAD_T *new_head, struct LIST_HEAD_T *p_head);
void list_del(struct LIST_HEAD_T *p_head);
int list_empty(struct LIST_HEAD_T *p_head);

#endif /* list_h */
