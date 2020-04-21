//
//  btree.h
//  Assignment
//
//  Created by User on 31/03/2020.
//  Copyright © 2020 User. All rights reserved.
//

#ifndef btree_h
#define btree_h

#include <stdio.h>

#define TRUE 1
#define FALSE 0

typedef struct {
    void *key;
    void *value;
} B_TREE_ELEMENT_T;


struct B_TREE_NODE_T{
    struct B_TREE_NODE_T *parent;
    struct B_TREE_NODE_T **childs;
    B_TREE_ELEMENT_T **elements;
    unsigned short leaf;
    int n; // the current number of child
};

typedef struct B_TREE_NODE_T B_TREE_NODE_T;


typedef struct {
    int degree;
    unsigned int depth;
    int (*cmp)(void *, void *);
    B_TREE_NODE_T *root;
} B_TREE_T;


int compare(void *k1, void *k2);
void init_btree(B_TREE_T *tree, int degree);
void init_btree_node(B_TREE_NODE_T *node, int degree);
void btree_split(B_TREE_NODE_T *node, B_TREE_T *tree);
void btree_insert(void *key, void *value, B_TREE_T *tree);
void btree_delete_leaf(int key_index, B_TREE_NODE_T *node, B_TREE_T *tree);
void btree_delete_no_leaf(int key_index, B_TREE_NODE_T *node, B_TREE_T *tree);
void btree_delete(void *key, B_TREE_T *tree);
void* btree_get(void *key, B_TREE_T *tree);
void* btree_get_range(void *key_range, B_TREE_T *tree);
int btree_empty(B_TREE_T *tree);
void btree_traverse(B_TREE_NODE_T *node, int depth);

#endif /* btree_h */
