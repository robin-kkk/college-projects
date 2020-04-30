//
//  skiplist.c
//  skiplist
//
//  Created by User on 28/04/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include "skiplist.h"

#include <limits.h>

const long INF_KEY = LONG_MAX;

/* default compare function */
int compare(void *k1, void *k2) {
    long key1 = *((long*)k1);
    long key2 = *((long*)k2);
    
    if (key1 == key2) return 0;
    else if (key1 < key2) return 1;
    else return -1;
}

struct SKIPLIST_T* generate(int max_level,
                            void *max_key,
                            int rand_seed,
                            double p,
                            int (*cmp_func)(void *, void *))
{
    int i;
    struct SKIPLIST_T *skiplist = (struct SKIPLIST_T*) malloc(sizeof(struct SKIPLIST_T));
    struct SL_NODE_T *sentinel;
    if (skiplist == 0) {
        perror("[ERROR] generate()");
        exit(-1);
    }
    skiplist->level = 1;
    skiplist->max_level = (max_level == 0) ? MAX_LEVEL : max_level;
    skiplist->max_key = (max_key == 0) ? &INF_KEY : max_key;
    skiplist->p = p;
    skiplist->header = make_node(0,0,skiplist);
    sentinel = make_node(0,0,skiplist);
    for (i=0; i<skiplist->max_level; i++) {
        skiplist->header->forward[i] = sentinel;
    }
    skiplist->cmp = (cmp_func == 0) ? compare : cmp_func;
    srand((rand_seed == 0) ? time(0) : rand_seed);
    return skiplist;
}

struct SL_NODE_T* make_node(void *key, void *value, struct SKIPLIST_T *sl)
{
    struct SL_NODE_T *p_node = (struct SL_NODE_T*) malloc(sizeof(struct SL_NODE_T));
    if (p_node == 0) {
        perror("[ERROR] make_node()");
        exit(-1);
    }
    p_node->key = (key == 0) ? sl->max_key : key;
    p_node->value = value;
    p_node->forward = (struct SL_NODE_T**) calloc(sl->max_level, sizeof(struct SL_NODE_T*));
    if (p_node->forward == 0) {
        perror("[ERROR] make_node() - p_node->forward");
        exit(-1);
    }
    return p_node;
}

struct SL_NODE_T* sl_search(void *key, struct SKIPLIST_T *sl)
{
    struct SL_NODE_T *p_node = sl->header;
    int i;
    for (i=sl->level-1; i>=0; i--) {
        while (sl->cmp(key, p_node->forward[i]->key) == -1) {
            p_node = p_node->forward[i];
        }
    }
    p_node = p_node->forward[0];
    if (sl->cmp(p_node->key, key) == 0) return p_node;
    return 0;
}

int random_level(struct SKIPLIST_T *sl) {
    int level = 1;
    double random = (double) rand() / RAND_MAX;
    while ((random < sl->p) && (level < sl->max_level)) {
        level++;
        random = (double) rand() / RAND_MAX;
    }
    return level;
}

int sl_insert(void *key, void *value, struct SKIPLIST_T *sl)
{
    int i;
    struct SL_NODE_T *update[sl->max_level];
    struct SL_NODE_T *p_node = sl->header;
    for (i=sl->level-1; i>=0; i--) {
        while (sl->cmp(key, p_node->forward[i]->key) == -1) {
            p_node = p_node->forward[i];
        }
        update[i] = p_node;
    }
    p_node = p_node->forward[0];
    if (sl->cmp(p_node->key, key) == 0) {
        p_node->value = value;
        return 1;
    } else {
        int level = random_level(sl);
        if (level > sl->level) {
            for (i=sl->level; i<level; i++) {
                update[i] = sl->header;
            }
            sl->level = level;
        }
        p_node = make_node(key, value, sl);
        for (i=0; i<level; i++) {
            p_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = p_node;
        }
    }
    return 0;
}

int sl_delete(void *key, struct SKIPLIST_T *sl)
{
    int i;
    struct SL_NODE_T *update[sl->max_level];
    struct SL_NODE_T *p_node = sl->header;
    for (i=sl->level-1; i>=0; i--) {
        while (sl->cmp(key, p_node->forward[i]->key) == -1) {
            p_node = p_node->forward[i];
        }
        update[i] = p_node;
    }
    p_node = p_node->forward[0];
    if (sl->cmp(p_node->key, key) == 0) {
        for (i=0; i<sl->level; i++) {
            if (update[i]->forward[i] != p_node) break;
            update[i]->forward[i] = p_node->forward[i];
        }
        free(p_node->forward);
        free(p_node);
        
        while ((sl->level > 1) && (sl->header->forward[sl->level-1]->key == sl->max_key)) {
            sl->level--;
        }
        return 1;
    }
    return 0;
}

void traverse(struct SKIPLIST_T *sl) {
    struct SL_NODE_T *p_node;
    int i;
    for (i=sl->level-1; i>=0; i--) {
        printf("\nLevel = %d:\n", i+1);
        p_node = sl->header->forward[i];
        while (p_node->key != sl->max_key) {
            printf("%ld ", *(long*)p_node->key);
            p_node = p_node->forward[i];
        }
        printf("\n");
    }
    printf("\n");
}
