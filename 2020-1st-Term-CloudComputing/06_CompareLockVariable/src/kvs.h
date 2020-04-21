//
//  kvs.h
//  Assignment
//
//  Created by User on 23/03/2020.
//  Copyright © 2020 User. All rights reserved.
//

#ifndef kvs_h
#define kvs_h

#include <unistd.h>
#include <pthread.h>

#include "list.h"


struct KVS_T {
    pthread_rwlock_t rwlock;
    pthread_mutex_t mutex;
    struct B_TREE_T tree;
};

struct KVS_NODE_T {
    void *key;
    void *value;
    struct LIST_HEAD_T list;
};

struct KVS_T* kv_create_db(int degree);

void __kv_destroy_db(struct B_TREE_HEAD_T *p_head);

int kv_destroy_db(struct KVS_T *kvs);

int kv_put(struct KVS_NODE_T *new_node, struct KVS_T *kvs);

int kv_delete(void *key, struct KVS_T *kvs);

struct KVS_NODE_T* kv_get(void *key, struct KVS_T *kvs);

struct LIST_HEAD_T* __kv_get_range(void *start_key,
                                   void* end_key,
                                   struct B_TREE_HEAD_T *head,
                                   struct LIST_HEAD_T* list,
                                   struct B_TREE_T *tree);

struct KVS_NODE_T* kv_get_range(void *start_key,
                                void* end_key,
                                struct KVS_T *kvs,
                                int *count);

void __traverse(struct B_TREE_HEAD_T *p_head, int depth);

void traverse(struct KVS_T *kvs);

void _print(struct B_TREE_HEAD_T *p_head, int depth);

void print(struct KVS_T *kvs);

#endif
