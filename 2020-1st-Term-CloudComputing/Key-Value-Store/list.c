//
//  list.c
//  Assignment
//
//  Created by User on 31/03/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include "list.h"

/* default compare function */
int compare(void *k1, void *k2) {
    long key1 = *((long*)k1);
    long key2 = *((long*)k2);
    
    if (key1 == key2) return 0;
    else if (key1 < key2) return 1;
    else return -1;
}


void init_btree(struct B_TREE_T *tree, int degree) {
    tree->degree = degree;
    tree->depth = 0;
    tree->cmp = &compare;
    
    struct B_TREE_NODE_T *root = (struct B_TREE_NODE_T*) malloc(sizeof(struct B_TREE_NODE_T));
    if (root == 0) {
        perror("init_btree(): couldn't allocated.");
        exit(-1);
    }
    tree->root = &root->head;
    root->elements = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
    init_list_head(root->elements);
    init_btree_head(tree->root, degree);
}


void init_btree_head(struct B_TREE_HEAD_T *head, int degree) {
    head->n = 0;
    head->leaf = TRUE;
    head->parent = 0;
    head->childs = (struct B_TREE_HEAD_T**) malloc(sizeof(struct B_TREE_HEAD_T*) * (degree+1));
    
    if (head->childs == 0) {
        perror("init_btree_head(): couldn't allocated.");
        exit(-1);
    }
    
    memset(head->childs, 0, sizeof(struct B_TREE_HEAD_T*) * (degree+1));
}


void init_list_head(struct LIST_HEAD_T *head) {
    head->prev = 0;
    head->next = 0;
}


/*
 el_idx: the index of element in parent.
 head: the head ptr of node to be divided.
 tree: ptr of btree.
 */
void btree_divide(int el_idx, struct B_TREE_HEAD_T *head, struct B_TREE_T *tree) {
    /*
     1. t-1 번째 원소를 부모로 올리고,
     2. 노드를 분할
     3. 부모도 원소가 꽉찼다면 재귀호출
     */
    int i;
    int t = ceil(tree->degree/2);
    struct B_TREE_NODE_T *node = list_entry(head, struct B_TREE_NODE_T, head);
    struct LIST_HEAD_T *target = node->elements;
    struct B_TREE_NODE_T *parent_node;
    struct LIST_HEAD_T *parent;
    struct B_TREE_NODE_T *left;
    struct B_TREE_NODE_T *right;
    for (i=0; i<t; i++) target = target->next;
    
    if (head->parent == 0 || el_idx == -1) { // 루트노드인 경우
        left = (struct B_TREE_NODE_T*) malloc(sizeof(struct B_TREE_NODE_T));
        right = (struct B_TREE_NODE_T*) malloc(sizeof(struct B_TREE_NODE_T));
        if (left == 0 || right == 0) {
            perror("btree_divide(): left or right not allocated.");
            exit(-1);
        }
        
        init_btree_head(&left->head, tree->degree);
        left->elements = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
        if (left->elements == 0) {
            perror("btree_divide(): left->elements not allocated.");
            exit(-1);
        }
        init_list_head(left->elements);
        left->elements->next = node->elements->next;
        left->elements->next->prev = left->elements;
        target->prev->next = left->elements;
        left->head.parent = head;
        left->head.leaf = head->leaf;
        left->head.n = t - 1;
        
        init_btree_head(&right->head, tree->degree);
        right->elements = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
        init_list_head(right->elements);
        right->elements->next = target->next;
        right->elements->next->prev = right->elements;
        right->head.parent = head;
        right->head.leaf = head->leaf;
        right->head.n = tree->degree - left->head.n - 1;
        
        node->elements->next = target;
        target->prev = node->elements;
        target->next = node->elements;
        
        target = right->elements;
        while (target->next != node->elements) target = target->next;
        target->next = right->elements;
        
        head->n = 1;
        if (!head->leaf) {
            memcpy(left->head.childs, head->childs, sizeof(struct B_TREE_HEAD_T*) * t);
            memcpy(right->head.childs, &head->childs[t], sizeof(struct B_TREE_HEAD_T*)*(right->head.n+1));
            memset(head->childs, 0, sizeof(struct B_TREE_HEAD_T*)*tree->degree);
        }
        head->childs[0] = &left->head;
        head->childs[1] = &right->head;
        head->leaf = FALSE;
        tree->depth++;
        
        return;
    }
    
    left = list_entry(head, struct B_TREE_NODE_T, head);
    right = (struct B_TREE_NODE_T*) malloc(sizeof(struct B_TREE_NODE_T));
    if (right == 0) {
        perror("btree_divide(): right not allocated.");
        exit(-1);
    }
    
    target->prev->next = left->elements; // == node->elements
    left->head.n = t - 1;
    left->head.leaf = head->leaf;
    
    init_btree_head(&right->head, tree->degree);
    right->elements = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
    if (right->elements == 0) {
        perror("btree_divide(): right->elements not allocated.");
        exit(-1);
    }
    init_list_head(right->elements);
    right->elements->next = target->next;
    right->elements->next->prev = right->elements;
    right->head.parent = head->parent;
    right->head.leaf = head->leaf;
    right->head.n = tree->degree - left->head.n - 1;
    
    // 삽입할 위치 찾기: ... -> target -> parent_el
    parent_node = list_entry(head->parent, struct B_TREE_NODE_T, head);
    parent = parent_node->elements;
    if (el_idx == head->parent->n) {
        // 맨 뒤 삽입
        while (parent->next != parent_node->elements) parent = parent->next;
        target->next = parent->next;
        target->prev = parent;
        parent->next = target;
    } else {
        for (i=0; i<el_idx; i++) parent = parent->next;
        target->next = parent->next;
        target->prev = parent;
        if (el_idx != 0) parent->next->prev = target;
        parent->next = target;
    }
    
    target = right->elements;
    while (target->next != node->elements) target = target->next;
    target->next = right->elements;
    
    if (!head->leaf) {
        memcpy(right->head.childs, &head->childs[t], sizeof(struct B_TREE_HEAD_T*)*(right->head.n+1));
        memset(&head->childs[t], 0, sizeof(struct B_TREE_HEAD_T*)*(right->head.n+1));
    }
    
    head->parent->n++;
    
    struct B_TREE_HEAD_T** p_childs = parent_node->head.childs;
    for (i=head->parent->n; i>el_idx; i--) p_childs[i] = p_childs[i-1];
    p_childs[el_idx+1] = &(right->head);
    if (el_idx == 0) {
        p_childs[0] = &left->head;
    }
    
    if (parent_node->head.n == tree->degree) {
        if (parent_node->head.parent == 0) {
            btree_divide(-1, head->parent, tree);
        } else {
            node = parent_node;
            parent_node = list_entry(parent_node->head.parent, struct B_TREE_NODE_T, head);
            for (i=0; i<=parent_node->head.n; i++) {
                if (parent_node->head.childs[i] == &node->head) break;
            }
            btree_divide(i, head->parent, tree);
        }
    }
}

// 노드 병합 ; 노드의 포인터를 반환 ( 노드 삭제 포함 )
struct B_TREE_HEAD_T* btree_merge(struct B_TREE_HEAD_T *left, struct B_TREE_HEAD_T *right) {
    struct B_TREE_NODE_T *l_node = list_entry(left, struct B_TREE_NODE_T, head);
    struct B_TREE_NODE_T *r_node = list_entry(right, struct B_TREE_NODE_T, head);
    struct LIST_HEAD_T *l_el = l_node->elements;
    struct LIST_HEAD_T *r_el = r_node->elements;
    while (l_el->next != l_node->elements) l_el = l_el->next;
    while (r_el != r_node->elements) {
        l_el->next = r_el;
        r_el->prev = l_el;
        r_el = r_el->next;
        l_el = l_el->next;
    }
    memcpy(&left->childs[l_node->head.n], right->childs, sizeof(struct B_TREE_HEAD_T*) * right->n);
    free(r_node);
    return left;
}

int btree_empty(struct B_TREE_T *tree) {
    return tree->root->n == 0;
}
