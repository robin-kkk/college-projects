//
//  kvs.c
//  Assignment
//
//  Created by User on 23/03/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include "kvs.h"


struct KVS_T* kv_create_db(int degree) {
    struct KVS_T *kvs = (struct KVS_T*) malloc(sizeof(struct KVS_T));
    if (kvs == 0) {
        perror("[ERROR] kv_create_db()");
        exit(-1);
    }
    struct B_TREE_T* btree = (struct B_TREE_T*) malloc(sizeof(struct B_TREE_T));
    init_btree(btree, degree);
    kvs->tree = *btree;
    //pthread_rwlock_init(&kvs->rwlock, NULL);
    pthread_mutex_init(&kvs->mutex, NULL);
    return kvs;
}

void __kv_destroy_db(struct B_TREE_HEAD_T *p_head) {
    struct B_TREE_NODE_T *p_node = list_entry(p_head, struct B_TREE_NODE_T, head);
    struct LIST_HEAD_T *el_head;
    struct LIST_HEAD_T *tmp;
    struct KVS_NODE_T *p_entry;
    if (p_head->leaf) {
        list_for_each_safe(el_head, tmp, p_node->elements) {
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            el_head->prev->next = el_head->next;
            el_head->next->prev = el_head->prev;
            free(p_entry);
        }
    } else {
        int i = 0;
        list_for_each_safe(el_head, tmp, p_node->elements) {
            __kv_destroy_db(p_head->childs[i++]); // left
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            el_head->prev->next = el_head->next;
            el_head->next->prev = el_head->prev;
            free(p_entry);
        }
        __kv_destroy_db(p_head->childs[p_head->n]); // right
    }
    free(p_node->elements);
    free(p_node->head.childs);
    free(p_node);
}

int kv_destroy_db(struct KVS_T *kvs) {
    if (kvs->tree.root->n == 0) return 0;
    __kv_destroy_db(kvs->tree.root);
    //free(&kvs->tree);
    return 1;
}

int kv_put(struct KVS_NODE_T *new_node, struct KVS_T *kvs) {
    int i; // key index in B-Tree
    int parent_el_idx = -1;
    struct B_TREE_HEAD_T *current = kvs->tree.root;
    struct B_TREE_NODE_T *p_node;
    struct LIST_HEAD_T *el_head;
    struct KVS_NODE_T *p_entry;
    
    while (1) {
        if (current->leaf) {
            p_node = list_entry(current, struct B_TREE_NODE_T, head);
            list_for_each(el_head, p_node->elements) {
                if (el_head == 0) {
                    p_node->elements->next = &new_node->list;
                    new_node->list.prev = p_node->elements;
                    new_node->list.next = p_node->elements;
                    current->n++;
                    break;
                }
                
                p_entry = list_entry(el_head, struct KVS_NODE_T, list);
                int comp = kvs->tree.cmp(new_node->key, p_entry->key);
                if (comp == 0) { // 삽입할 키가 현재 원소의 키와 같으면 값 변경
                    p_entry->value = new_node->value;
                    return 1;
                } else if (comp == 1) { // 삽입할 키가 현재 원소의 키보다 작으면 앞에 삽입
                    new_node->list.next = el_head;
                    new_node->list.prev = el_head->prev;
                    el_head->prev->next = &new_node->list;
                    el_head->prev = &new_node->list;
                    current->n++;
                    break;
                }
                
                if (el_head->next == p_node->elements) { // 맨 뒤
                    new_node->list.prev = el_head;
                    new_node->list.next = p_node->elements;
                    el_head->next = &new_node->list;
                    current->n++;
                    break;
                }
            }
            
            // 현재 노드 내 원소가 꽉 찬 경우
            if (current->n == kvs->tree.degree) {
                btree_divide(parent_el_idx, current, &kvs->tree);
            }
            break;
        }
        
        p_node = list_entry(current, struct B_TREE_NODE_T, head);
        parent_el_idx = -1;
        i = 0;
        list_for_each(el_head, p_node->elements) {
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            int comp = kvs->tree.cmp(new_node->key, p_entry->key);
            if (comp == 0) {
                p_entry->value = new_node->value;
                return 1;
            } else if (comp == 1) {
                current = current->childs[i];
                parent_el_idx = i;
                break;
            }
            i++;
        }
        
        if (parent_el_idx == -1) {
            parent_el_idx = current->n;
            current = current->childs[parent_el_idx];
        }
    }
    return 0;
}

int kv_delete(void *key, struct KVS_T *kvs) {
    struct B_TREE_HEAD_T *current = kvs->tree.root;
    struct B_TREE_NODE_T *node;
    struct LIST_HEAD_T *el_head;
    struct LIST_HEAD_T *pr_el_head = 0;
    struct KVS_NODE_T *p_entry;
    int subtree_idx = -1;
    while (current != NULL) {
        int check = 1;
        int i = 0;
        node = list_entry(current, struct B_TREE_NODE_T, head);
        list_for_each(el_head, node->elements) {
            if (el_head == 0) {
                perror("[ERROR] kv_delete()");
                exit(-1);
            }
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            int comp = kvs->tree.cmp(key, p_entry->key);
            if (comp == 1) {
                current = current->childs[i];
                subtree_idx = i;
                pr_el_head = el_head;
                check = 0;
                break;
            } else if (comp == 0){
                btree_delete(i, subtree_idx, current, el_head, pr_el_head, &kvs->tree);
                free(p_entry);
                return 1;
            }
            i++;
        }
        if (check) {
            if (pr_el_head == 0) pr_el_head = node->elements->next;
            subtree_idx = current->n;
            current = current->childs[subtree_idx];
        }
    }
    return 0;
}

struct KVS_NODE_T* kv_get(void *key, struct KVS_T *kvs) {
    struct B_TREE_HEAD_T *current = kvs->tree.root;
    struct B_TREE_NODE_T *node;
    struct LIST_HEAD_T *el_head;
    struct KVS_NODE_T *p_entry;
    while (current != NULL) {
        int check = 1;
        int i = 0;
        node = list_entry(current, struct B_TREE_NODE_T, head);
        list_for_each(el_head, node->elements) {
            if (el_head == 0) {
                perror("[ERROR] kv_get()");
                exit(-1);
            }
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            int comp = kvs->tree.cmp(key, p_entry->key);
            if (comp == 1) {
                current = current->childs[i];
                check = 0;
                break;
            } else if (comp == 0){
                return p_entry;
            }
            i++;
        }
        if (check) current = current->childs[current->n];
    }
    return 0;
}

struct LIST_HEAD_T* __kv_get_range(void *start_key,
                    void* end_key,
                    struct B_TREE_HEAD_T *head,
                    struct LIST_HEAD_T* list,
                    struct B_TREE_T *tree)
{
    struct B_TREE_NODE_T *node = list_entry(head, struct B_TREE_NODE_T, head);
    struct LIST_HEAD_T *el_head = NULL;
    struct KVS_NODE_T *p_entry = NULL;
    struct KVS_NODE_T *new_entry = NULL;
    if (head->leaf) {
        list_for_each(el_head, node->elements) {
            if (el_head == 0) {
                perror("[ERROR] __kv_get_range()");
                exit(-1);
            }
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            int comp1 = tree->cmp(start_key, p_entry->key);
            int comp2 = tree->cmp(end_key, p_entry->key);
            if (comp1 >= 0 && comp2 <= 0) {
                new_entry = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
                if (new_entry == NULL) {
                    perror("[ERROR] __kv_get_range(): new_entry from leaf not allocated\n");
                    exit(-1);
                }
                memcpy(new_entry, p_entry, sizeof(struct KVS_NODE_T));
                init_list_head(&new_entry->list);
                list->next = &new_entry->list;
                new_entry->list.prev = list;
                list = list->next;
            }
        }
        return list;
    } else {
        int i = 0;
        list_for_each(el_head, node->elements) {
            if (el_head == 0) {
                perror("[ERROR] __kv_get_range()");
                exit(-1);
            }
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            int comp1 = tree->cmp(start_key, p_entry->key);
            int comp2 = tree->cmp(end_key, p_entry->key);
            if (comp1 >= 0 && comp2 <= 0) {
                list = __kv_get_range(start_key, end_key, head->childs[i++], list, tree);
                new_entry = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
                if (new_entry == NULL) {
                    perror("[ERROR] __kv_get_range(): new_entry from internal not allocated\n");
                    exit(-1);
                }
                memcpy(new_entry, p_entry, sizeof(struct KVS_NODE_T));
                init_list_head(&new_entry->list);
                list->next = &new_entry->list;
                new_entry->list.prev = list;
                list = list->next;
            }
        }
        list = __kv_get_range(start_key, end_key, head->childs[head->n], list, tree);
    }
    return list;
}


struct KVS_NODE_T* kv_get_range(void *start_key, void* end_key, struct KVS_T *kvs, int *count) {
    struct KVS_NODE_T* ret = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
    if (ret == NULL) {
        perror("[ERROR] kv_get_range()");
        exit(-1);
    }
    ret->key = NULL;
    ret->value = NULL;
    init_list_head(&ret->list);
    struct LIST_HEAD_T *list = __kv_get_range(start_key, end_key, kvs->tree.root, &ret->list, &kvs->tree);
    list->next = &ret->list;
    *count = 0;
    list_for_each(list, &ret->list) {
        (*count)++;
    }
    return ret;
}


void __traverse(struct B_TREE_HEAD_T *p_head, int depth) {
    int i, j;
    struct B_TREE_NODE_T *p_node = list_entry(p_head, struct B_TREE_NODE_T, head);
    struct LIST_HEAD_T *el_head = 0;
    struct KVS_NODE_T *p_entry = 0;
    if (p_head->leaf) {
        for (i=0; i<depth; i++) printf("--");
        printf("[  ");
        list_for_each(el_head, p_node->elements) {
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            printf("%ld  ", *((long*)p_entry->key));
        }
        printf("]\n");
    } else {
        i = 0;
        list_for_each(el_head, p_node->elements) {
            __traverse(p_head->childs[i++], depth+1); // left
            for (j=0; j<depth; j++) printf("--");
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            printf("%ld  ", *((long*)p_entry->key));
            printf("\n");
        }
        __traverse(p_head->childs[p_head->n], depth+1); // right
    }
}

void traverse(struct KVS_T *kvs) {
    printf("depth = %d\n", kvs->tree.depth);
    __traverse(kvs->tree.root, 0);
    printf("\n\n");
}

void _print(struct B_TREE_HEAD_T *p_head, int depth) {
    int i;
    struct B_TREE_NODE_T *p_node = list_entry(p_head, struct B_TREE_NODE_T, head);
    struct LIST_HEAD_T *el_head = 0;
    struct KVS_NODE_T *p_entry = 0;
    if (p_head->leaf) {
        list_for_each(el_head, p_node->elements) {
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            printf("%ld, ", *((long*)p_entry->key));
        }
    } else {
        i = 0;
        list_for_each(el_head, p_node->elements) {
            _print(p_head->childs[i++], depth+1); // left
            p_entry = list_entry(el_head, struct KVS_NODE_T, list);
            printf("%ld, ", *((long*)p_entry->key));
        }
        _print(p_head->childs[p_head->n], depth+1); // right
    }
}

void print(struct KVS_T *kvs) {
    printf("\n");
    printf("B-Tree's depth = %d\n", kvs->tree.depth);
    printf("B-Tree's degree = %d\n", kvs->tree.degree);
    printf("B-Tree keys: [\n");
    _print(kvs->tree.root, 0);
    printf("\n]\n");
}
