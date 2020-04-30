//
//  kvs.c
//  skiplist
//
//  Created by User on 28/04/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include "kvs.h"

struct KVS_T* kv_create_db(int sl_max_level,
                           void *max_key,
                           int rand_seed,
                           double p,
                           int (*cmp_func)(void *, void *))
{
    struct KVS_T *kvs = (struct KVS_T*) malloc(sizeof(struct KVS_T));
    if (kvs == 0) {
        perror("[ERROR] kv_create_db()");
        exit(-1);
    }
    struct SKIPLIST_T *skiplist = generate(sl_max_level, max_key, rand_seed, p, cmp_func);
    kvs->skiplist = *skiplist;
    pthread_rwlock_init(&kvs->rwlock, 0);
    return kvs;
}

void kv_destroy_db(struct KVS_T *kvs)
{
    free(kvs);
}

int kv_put(struct KVS_NODE_T *p_node, struct KVS_T *kvs)
{
    pthread_rwlock_wrlock(&kvs->rwlock);
    int result = sl_insert(p_node->key, p_node->value, &kvs->skiplist);
    free(p_node);
    pthread_rwlock_unlock(&kvs->rwlock);
    return result; // 1: modified, 0: inserted
}

int kv_delete(void *key, struct KVS_T *kvs)
{
    pthread_rwlock_wrlock(&kvs->rwlock);
    int result = sl_delete(key, &kvs->skiplist);
    pthread_rwlock_unlock(&kvs->rwlock);
    return result; // 1: deleted, 0: not found
}

struct KVS_NODE_T* kv_get(void *key, struct KVS_T *kvs)
{
    pthread_rwlock_rdlock(&kvs->rwlock);
    struct KVS_NODE_T *ret = 0;
    struct SL_NODE_T *p_node = sl_search(key, &kvs->skiplist);
    if (p_node) {
        ret = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
        ret->key = p_node->key;
        ret->value = p_node->value;
        init_list_head(&ret->head);
    }
    pthread_rwlock_unlock(&kvs->rwlock);
    return ret;
}

struct LIST_HEAD_T* kv_get_range(void *start_key,
                                void* end_key,
                                struct KVS_T *kvs,
                                int *count)
{
    pthread_rwlock_rdlock(&kvs->rwlock);
    int i;
    struct LIST_HEAD_T *ret = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
    struct LIST_HEAD_T *p_head = ret;
    struct SKIPLIST_T *sl = &kvs->skiplist;
    struct SL_NODE_T *sl_node = sl->header;
    for (i=sl->level-1; i>=0; i--) {
        while (sl->cmp(start_key, sl_node->forward[i]->key) == -1) {
            sl_node = sl_node->forward[i];
        }
    }
    *count = 0;
    init_list_head(p_head);
    sl_node = sl_node->forward[0];
    while (sl->cmp(end_key, sl_node->key) != 1) {
        struct KVS_NODE_T *p_node = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
        p_node->key = sl_node->key;
        p_node->value = sl_node->value;
        init_list_head(&p_node->head);
        list_add(&p_node->head, p_head);
        (*count)++;
        sl_node = sl_node->forward[0];
        p_head = p_head->next;
    }
    p_head->next = ret;
    pthread_rwlock_unlock(&kvs->rwlock);
    return ret;
}

void print(struct KVS_T *kvs) {
    pthread_rwlock_rdlock(&kvs->rwlock);
    traverse(&kvs->skiplist);
    pthread_rwlock_unlock(&kvs->rwlock);
}
