//
//  list.h
//  Assignment
//
//  Created by User on 31/03/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef list_h
#define list_h

#define TRUE 1
#define FALSE 0

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


struct LIST_HEAD_T { // for element list
    struct LIST_HEAD_T *prev;
    struct LIST_HEAD_T *next;
};

struct B_TREE_HEAD_T {
    struct B_TREE_HEAD_T *parent;
    struct B_TREE_HEAD_T **childs;
    unsigned short leaf; // whether it is a leaf node
    int n; // the current number of childs
};

struct B_TREE_NODE_T {
    struct LIST_HEAD_T *elements;
    struct B_TREE_HEAD_T head;
};

struct B_TREE_T {
    int degree;
    unsigned int depth;
    int (*cmp)(void *, void *);
    struct B_TREE_HEAD_T *root;
};

int compare(void *k1, void *k2);
void init_btree(struct B_TREE_T *tree, int degree);
void init_btree_head(struct B_TREE_HEAD_T *head, int degree);
void init_list_head(struct LIST_HEAD_T *head);

// 노드 분할 ( 노드 추가 포함 )
void btree_divide(int el_idx, struct B_TREE_HEAD_T *head, struct B_TREE_T *tree);

// 노드 병합 ; 노드의 포인터를 반환 ( 노드 삭제 포함 )
struct B_TREE_HEAD_T* btree_merge(struct B_TREE_HEAD_T *left, struct B_TREE_HEAD_T *right);

int btree_empty(struct B_TREE_T *tree);


#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)
#define list_for_each_safe(pos, n, head) \
for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)

#endif /* list_h */
