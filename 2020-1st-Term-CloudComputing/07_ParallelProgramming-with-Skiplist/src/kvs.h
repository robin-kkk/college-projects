//
//  kvs.h
//  skiplist
//
//  Created by User on 28/04/2020.
//  Copyright © 2020 User. All rights reserved.
//

#ifndef kvs_h
#define kvs_h

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "skiplist.h"
#include "list.h"

#define MAX_HASH 65535

struct KVS_T {
    pthread_rwlock_t rwlock;
    struct SKIPLIST_T skiplist;
};

struct KVS_NODE_T {
    void *key;
    void *value;
    struct LIST_HEAD_T head;
};

struct KVS_T* kv_create_db(int sl_max_level,
                           void *max_key,
                           int rand_seed,
                           double p,
                           int (*cmp_func)(void *, void *));
void kv_destroy_db(struct KVS_T *kvs);
int kv_put(struct KVS_NODE_T *p_node, struct KVS_T *kvs);
int kv_delete(void *key, struct KVS_T *kvs);
struct KVS_NODE_T* kv_get(void *key, struct KVS_T *kvs);
struct LIST_HEAD_T* kv_get_range(void *start_key,
                                void* end_key,
                                struct KVS_T *kvs,
                                int *count);
void print(struct KVS_T *kvs);
#endif /* kvs_h */
