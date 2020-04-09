//
//  main.c
//  Assignment
//
//  Created by User on 23/03/2020.
//  Copyright © 2020 User. All rights reserved.
//
#include "kvs.h"

#define KEY_NUM 69

long key[] = {40, 70, 100, 130, 10, 20, 30, 50, 60, 80, 90, 150, 180, 200, 210, 240, 260, 280, 300, 140, 135, 137, 110, 115, 120, 330, 380, 420, 450, 480, 510, 125, 127, 550, 580, 305, 315, 320, 335, 340, 350, 360, 425, 435, 445, 447, 449, 390, 410, 415, 364, 368, 370, 374, 376, 378, 375, 379, 201, 202, 204, 206, 208, 220, 224, 228, 190, 192, 194};
long value[KEY_NUM];
struct KVS_T *kvs;

void* search(void *arg) {
    int i;
    for (i=0; i<KEY_NUM; i++) {
        struct KVS_NODE_T *node = kv_get(&key[i], kvs);
        /*
        if (node == 0) {
            printf("[thread %3d] - not searched the key %ld\n", *(int*)arg, key[i]);
        } else {
            printf("[thread %3d] - searched (%ld, %ld)\n", *(int*)arg, *(long*)node->key, *(long*)node->value);
        }
         */
    }
    return 0;
}

void* insert(void *arg) {
    int i, found;
    for (i=0; i<KEY_NUM; i++) {
        value[i] = i;
        struct KVS_NODE_T *new_node = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
        if (new_node == 0) {
            perror("kv_put(): not allocated.");
            exit(-1);
        }
        init_list_head(&new_node->list);
        new_node->key = &key[i];
        new_node->value = &value[i];
        found = kv_put(new_node, kvs);
        if (found == 1) {
            printf("[thread %3d] - modified key %ld\n", *(int*)arg, key[i]);
            free(new_node);
        } else {
            printf("[thread %3d] - inserted key %ld\n", *(int*)arg, key[i]);
        }
        //traverse(kvs);
    }
    return 0;
}

void* delete(void *arg) {
    int i;
    long removed_key[] = {315,208};
    int cnt = 2;
    for (i=0; i<cnt; i++) {
        printf("Finding ... key = %ld\n", removed_key[i]);
        kv_delete(&removed_key[i], kvs);
        printf("[thread %3d] - delete key %ld\n", *(int*)arg, removed_key[i]);
        print(kvs);
    }
    return 0;
}

int main() {
    void* thr_ret;
    pthread_t t_id[102];
    int number[102];
    int i;
    
    for (i=0; i<KEY_NUM; i++) value[i] = 0xdeadbeef + i;
    
    kvs = kv_create_db(4);
    
    // Larget Data Set Test
    number[101] = 101;
    number[100] = 100;
    insert((void*)&number[100]);
    print(kvs);
    
    number[100] = 100;
    if (pthread_create(&t_id[100],NULL,insert,(void*)&number[100]) != 0) {
        perror("pthread_create() error");
        exit(-1);
    }
    
    for (i=0; i<100; i++) {
        number[i] = i;
        if (pthread_create(&t_id[i],NULL,search,(void*)&number[i]) != 0) {
            perror("pthread_create() error");
            exit(-1);
        }
    }
    
    if (pthread_create(&t_id[101],NULL,delete,(void*)&number[101]) != 0) {
        perror("pthread_create() error");
        exit(-1);
    }
    
    for (i=0; i<102; i++) {
        if (pthread_join(t_id[i], &thr_ret) != 0) {
            perror("pthread_join() error");
            exit(-1);
        }
    }
    
    kv_destroy_db(kvs);
    
    return 0;
}
