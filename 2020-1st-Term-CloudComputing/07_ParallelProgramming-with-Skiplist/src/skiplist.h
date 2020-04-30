//
//  skiplist.h
//  skiplist
//
//  Created by User on 28/04/2020.
//  Copyright © 2020 User. All rights reserved.
//

#ifndef skiplist_h
#define skiplist_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEVEL 16

struct SL_NODE_T {
    void *key;
    void *value;
    struct SL_NODE_T **forward;
};

struct SKIPLIST_T {
    int level;
    int max_level;
    struct SL_NODE_T *header;
    int (*cmp)(void *, void *);
    void *max_key;
    double p;
};

int compare(void *k1, void *k2);
struct SKIPLIST_T* generate(int max_level,
                            void *max_key,
                            int rand_seed,
                            double p,
                            int (*cmp_func)(void *, void *));
struct SL_NODE_T* make_node(void *key, void *value, struct SKIPLIST_T *sl);
struct SL_NODE_T* sl_search(void *key, struct SKIPLIST_T *sl);
int sl_insert(void *key, void *value, struct SKIPLIST_T *sl);
int sl_delete(void *key, struct SKIPLIST_T *sl);
void traverse(struct SKIPLIST_T *sl);

#endif /* skiplist_h */
