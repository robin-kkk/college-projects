//
//  btree.c
//  Assignment
//
//  Created by User on 31/03/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include "btree.h"

#include <math.h>
#include <stdlib.h>
#include <assert.h>


int compare(void *k1, void *k2) {
    long key1 = *((long*)k1);
    long key2 = *((long*)k2);
    
    if (key1 == key2) return 0;
    else if (key1 < key2) return 1;
    else return -1;
}

void init_btree(B_TREE_T *tree, int degree) {
    tree->degree = degree;
    tree->depth = 0;
    tree->cmp = &compare;
    tree->root = (B_TREE_NODE_T*) malloc(sizeof(B_TREE_NODE_T));
    if (tree->root == 0) {
        perror("init_btree(): couldn't allocated.");
        exit(-1);
    }
    init_btree_node(tree->root, degree);
}

void init_btree_node(B_TREE_NODE_T *node, int degree) {
    int i = 0;
    node->n = 0;
    node->leaf = TRUE;
    node->parent = 0;
    node->childs = (B_TREE_NODE_T**) malloc(sizeof(B_TREE_NODE_T*) * degree);
    node->elements = (B_TREE_ELEMENT_T**) malloc(sizeof(B_TREE_ELEMENT_T*) * (degree - 1));
    
    if (node->childs == 0 || node->elements == 0) {
        perror("init_btree_head(): couldn't allocated.");
        exit(-1);
    }
    
    for (i=0; i<degree-1; i++) {
        node->elements[i] = 0;
        node->childs[i] = 0;
    }
    node->childs[degree-1] = 0;
}


void btree_split(B_TREE_NODE_T *node, B_TREE_T *tree) {
    int t = ceil(tree->degree/2);
    int i;
    B_TREE_NODE_T *left = 0;
    B_TREE_NODE_T *right = 0;
    B_TREE_NODE_T *parent = node->parent;
    B_TREE_ELEMENT_T* el = node->elements[t-1];
    
    if (parent == 0) {
        // base case: 분할할 노드가 루트인 경우
        left = (B_TREE_NODE_T*) malloc(sizeof(B_TREE_NODE_T));
        right = (B_TREE_NODE_T*) malloc(sizeof(B_TREE_NODE_T));
        if (left == 0 || right == 0) {
            perror("btree_split(): couldn't be allocated (left == 0 or right == 0).");
            exit(-1);
        }
        init_btree_node(left, tree->degree);
        init_btree_node(right, tree->degree);
        
        for (i=0; i<t-1; i++) {
            left->elements[i] = node->elements[i];
            left->childs[i] = node->childs[i];
            left->n++;
            node->elements[i] = 0;
            node->childs[i] = 0;
        }
        left->childs[t-1] = node->childs[t-1];
        node->childs[t-1] = 0;
        
        for (i=t; i<node->n; i++) {
            right->elements[i-t] = node->elements[i];
            right->childs[i-t] = node->childs[i];
            right->n++;
            node->elements[i] = 0;
            node->childs[i] = 0;
        }
        right->childs[right->n] = node->childs[node->n];
        node->childs[node->n] = 0;
        
        left->parent = node;
        right->parent = node;
        left->leaf = (left->childs[0] == 0);
        right->leaf = (right->childs[0] == 0);
        node->leaf = FALSE;
        node->n = 1;
        node->childs[0] = left;
        node->childs[1] = right;
        node->elements[0] = el;
        node->elements[t-1] = 0;
        tree->depth++;
        
        return;
    }
    
    left = node;
    right = (B_TREE_NODE_T*) malloc(sizeof(B_TREE_NODE_T));
    if (right == 0) {
        perror("btree_split(): couldn't be allocated (right == 0).");
        exit(-1);
    }
    init_btree_node(right, tree->degree);
    i = parent->n;
    if (i > 0) {
        while (tree->cmp(el->key, parent->elements[i-1]->key) != -1) {
            parent->elements[i] = parent->elements[i-1];
            parent->childs[i+1] = parent->childs[i];
            i--;
            if (i == 0) break;
        }
    }
    parent->elements[i] = el;
    parent->childs[i+1] = right;
    parent->n++;
    right->parent = parent;
    right->leaf = left->leaf;
    
    for (i=t; i<node->n; i++) {
        right->elements[i-t] = node->elements[i];
        right->childs[i-t] = node->childs[i];
        right->n++;
        node->elements[i] = 0;
    }
    right->childs[right->n] = node->childs[node->n];
    
    node->elements[t-1] = 0;
    node->n -= right->n + 1;
    if (parent->n == tree->degree) {
        btree_split(parent, tree);
    }
}

void btree_insert(void *key, void *value, B_TREE_T *tree) {
    int i;
    int check;
    B_TREE_NODE_T *current = tree->root;
    
    while (1) {
        if (current->leaf) {
            i = current->n;
            if (i > 0) {
                while (tree->cmp(key, current->elements[i-1]->key) != -1) {
                    current->elements[i] = current->elements[i-1];
                    i--;
                    if (i == 0) break;
                }
            }
            current->elements[i] = (B_TREE_ELEMENT_T*) malloc(sizeof(B_TREE_ELEMENT_T));
            current->elements[i]->key = key;
            current->elements[i]->value = value;
            current->n++;
            if (current->n == tree->degree) {
                btree_split(current, tree);
            }
            break;
        }
        check = 1;
        for (i=0; i<current->n; i++) {
            if (tree->cmp(key, current->elements[i]->key) != -1) {
                current = current->childs[i];
                check = 0;
                break;
            }
        }
        if (check) {
            current = current->childs[current->n];
        }
    }
}

void btree_delete_leaf(int key_index, B_TREE_NODE_T *node, B_TREE_T *tree) {
    int i;
    int t = ceil(tree->degree/2);
    B_TREE_ELEMENT_T *deleted = node->elements[key_index];
    B_TREE_NODE_T *parent = node->parent;
    free(deleted);
    
    if (node->n >= t) {
        if (parent->n < t) {
            // 부모의 형제 노드 찾기
            B_TREE_NODE_T *grand_parent = parent->parent;
            int parent_idx = -1;
            for (i=0; i<=grand_parent->n; i++) {
                if (grand_parent->childs[i] == parent) {
                    parent_idx = i;
                    break;
                }
            }
            assert ( parent_idx != -1 );
            
            B_TREE_NODE_T *parent_left_sibling = (parent_idx > 0) ? grand_parent->childs[parent_idx - 1] : 0;
            B_TREE_NODE_T *parent_right_sibling = (parent_idx < grand_parent->n) ? grand_parent->childs[parent_idx + 1] : 0;
            B_TREE_NODE_T *merged_target = 0;
            
            if (parent_left_sibling != 0) {
                if (parent_left_sibling->n < t) {
                    merged_target = parent_left_sibling;
                }
            } else if (parent_right_sibling != 0) {
                if (parent_right_sibling->n < t) {
                    merged_target = parent_right_sibling;
                }
            }
            if (merged_target != 0) {
                int el_idx = (parent_idx == grand_parent->n) ? parent_idx - 1 : parent_idx;
                parent->elements[parent->n++] = grand_parent->elements[el_idx];
                for (i=0; i<merged_target->n; i++) {
                    parent->elements[parent->n] = merged_target->elements[i];
                    parent->childs[parent->n++] = merged_target->childs[i];
                }
                assert(parent->n <= tree->degree - 1);
                parent->childs[parent->n] = merged_target->childs[merged_target->n];
                free(merged_target);
                
                if (grand_parent->n == 1) { // 루트 노드가 병합된 경우
                    tree->root = parent;
                    free(grand_parent);
                } else {
                    for (i=parent_idx+1; i<grand_parent->n; i++) {
                        grand_parent->elements[i-1] = grand_parent->elements[i];
                        grand_parent->childs[i] = grand_parent->childs[i+1];
                    }
                    grand_parent->childs[grand_parent->n] = 0;
                    grand_parent->childs[parent_idx] = parent;
                }
            }
        }
        return;
    }
    
    while (node->n < t) {
        // 형제 노드 찾기
        int node_idx = -1;
        for (i=0; i<=parent->n; i++) {
            if (parent->childs[i] == node) {
                node_idx = i;
                break;
            }
        }
        assert ( node_idx != -1 );
        
        B_TREE_NODE_T *left_sibling = (node_idx > 0) ? parent->childs[node_idx - 1] : 0;
        B_TREE_NODE_T *right_sibling = (node_idx < parent->n) ? parent->childs[node_idx + 1] : 0;
        
        if (left_sibling != 0) {
            int el_idx = (node_idx == parent->n) ? node_idx - 1 : node_idx;
            if (left_sibling->n < t) {
                // 부모 노드에서 부모 원소를 현재 노드로 가져온다. (여기서 부모 원소는 현 노드 내 최솟값이다.)
                // 형제 노드와 병합한다.
                // 이 때, 부모 노드의 원소 수가 t 미만이면 재귀적으로 처리한다.
                left_sibling->elements[left_sibling->n++] = parent->elements[el_idx];
                for (i=0; i<node->n; i++) {
                    left_sibling->elements[left_sibling->n] = node->elements[i];
                    left_sibling->childs[left_sibling->n++] = node->childs[i];
                }
                left_sibling->childs[left_sibling->n] = node->childs[node->n];
                assert (left_sibling->n <= tree->degree - 1);
                for (i=node_idx+1; i<parent->n; i++) {
                    parent->elements[i-1] = parent->elements[i];
                    parent->childs[i] = parent->childs[i+1];
                }
                parent->childs[node_idx] = left_sibling;
                parent->n--;
                free(node);
            } else {
                // 형제 노드로부터 원소를 빌려서 부모 노드에 둔다. (left_sibling -> 최댓값, right_sbiling -> 최솟값)
                // 부모 노드의 원소가 현재 노드로 내려온다.
                node->elements[key_index] = parent->elements[node_idx];
                parent->elements[node_idx] = left_sibling->elements[--(left_sibling->n)];
                left_sibling->elements[left_sibling->n] = 0;
            }
        } else if (right_sibling != 0) {
            if (right_sibling->n < t) {
                node->elements[node->n++] = parent->elements[node_idx];
                for (i=node_idx+1; i<parent->n; i++) {
                    parent->elements[i-1] = parent->elements[i];
                    parent->childs[i] = parent->childs[i+1];
                }
                parent->elements[parent->n - 1] = 0;
                parent->childs[parent->n] = 0;
                parent->n--;
                for (i=0; i<right_sibling->n; i++) {
                    node->elements[node->n] = right_sibling->elements[i];
                    node->childs[node->n++] = right_sibling->childs[i];
                }
                free(right_sibling);
            } else {
                node->elements[key_index] = parent->elements[node_idx];
                parent->elements[node_idx] = right_sibling->elements[0];
                for (i=1; i<=right_sibling->n; i++) {
                    right_sibling->elements[i-1] = right_sibling->elements[i];
                }
                right_sibling->n--;
            }
        }
        
        node = parent;
        parent = node->parent;
    }
    
}

void btree_delete_no_leaf(int key_index, B_TREE_NODE_T *node, B_TREE_T *tree) {
    int i;
    int t = ceil(tree->degree/2);
    B_TREE_NODE_T *left = node->childs[key_index];
    B_TREE_NODE_T *right = node->childs[key_index+1];
    B_TREE_ELEMENT_T *deleted = node->elements[key_index];
    
    if (left->n >= t) {
        // 왼쪽 노드의 원소 수가 t 이상일 경우, 최댓값을 가져온다.
        node->elements[key_index] = left->elements[left->n - 1];
        left->elements[left->n - 1] = 0;
        left->n--;
    } else if (right->n >= t) {
        // 오른쪽 노드의 원소 수가 t 이상일 경우, 최댓값을 가져온다.
        node->elements[key_index] = right->elements[0];
        for (i=1; i<right->n; i++) {
            right->elements[i-1] = right->elements[i];
        }
        right->elements[right->n - 1] = 0;
        right->n--;
    } else {
        // 모든 자식 노드의 원소 수가 t - 1 개인 경우, 합병한다.
        int total = left->n + right->n;
        assert( total <= tree->degree - 1 );
        for (i=0; i<right->n; i++) {
            left->elements[left->n + i] = right->elements[i];
            left->childs[left->n + i + 1] = right->childs[i];
        }
        left->n = total;
        left->childs[left->n + 1] = right->childs[right->n];
        node->childs[key_index+1] = left;
        free(right);
        
        for (i=key_index+1; i<node->n; i++) {
            node->elements[i-1] = node->elements[i];
            node->childs[i-1] = node->childs[i];
        }
        node->n--;
        node->childs[node->n] = node->childs[i];
    }
    free(deleted);
}

void btree_delete(void *key, B_TREE_T *tree) {
    B_TREE_NODE_T *current = tree->root;
    int i;
    int check;
    while (1) {
        check = 1;
        for (i=0; i<current->n; i++) {
            int comp = tree->cmp(key, current->elements[i]->key);
            if (comp == -1) continue;
            if (comp == 1) {
                current = current->childs[i];
                check = 0;
                break;
            } else {
                if (current->leaf) btree_delete_leaf(i, current, tree);
                else btree_delete_no_leaf(i, current, tree);
                return;
            }
        }
        if (check) {
            current = current->childs[current->n];
            if (current == 0) {
                printf("btree_delete(): key not found.\n");
            }
        }
    }
}

void* btree_get(void *key, B_TREE_T *tree) {
    int i;
    B_TREE_NODE_T *current = tree->root;
    while (current != 0) {
        int check = 1;
        for (i=0; i<current->n; i++) {
            int comp = tree->cmp(key, current->elements[i]->key);
            if (comp == -1) continue;
            if (comp == 1) {
                current = current->childs[i];
                check = 0;
                break;
            } else {
                return current->elements[i]->value;
            }
        }
        if (check) current = current->childs[current->n];
    }
    
    return 0;
}

void* btree_get_range(void *key_range, B_TREE_T *tree) {
    return 0;
}

int btree_empty(B_TREE_T *tree) {
    return tree->root->n == 0;
}

void btree_traverse(B_TREE_NODE_T *node, int depth) {
    int i, j;
    if (node->leaf) {
        for (i=0; i<depth; i++) printf("--");
        printf("[ ");
        for (i=0; i<node->n; i++)
            printf("%d  ", *((int*)node->elements[i]->key));
        printf("]\n");
    } else {
        for (i=0; i<node->n; i++) {
            btree_traverse(node->childs[i], depth+1); // left
            for (j=0; j<depth; j++) printf("--");
            printf(" %d", *((int*)node->elements[i]->key));
            printf("\n");
        }
        for (j=0; j<depth; j++) printf("--");
        btree_traverse(node->childs[node->n], depth+1); // right
        printf("\n");
    }
}
