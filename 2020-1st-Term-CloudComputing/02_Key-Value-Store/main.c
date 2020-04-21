//
//  main.c
//  Assignment
//
//  Created by User on 23/03/2020.
//  Copyright © 2020 User. All rights reserved.
//
#include "kvs.h"

#include <unistd.h>
#include <pthread.h>

long key[] = {9,10,13,15,16,17,8,1,2,5,4};
long value[11];
struct KVS_T *kvs;
//pthread_mutex_t mutex;

void* search(void *arg) {
    int i;
    for (i=0; i<11; i++) {
        //pthread_mutex_lock(&mutex);
        struct KVS_NODE_T *node = kv_get(&key[i], kvs);
        if (node == 0) {
            printf("[thread %3d] - not searched the key %ld\n", *(int*)arg, key[i]);
        } else {
            printf("[thread %3d] - searched (%ld, %ld)\n", *(int*)arg, *(long*)node->key, *(long*)node->value);
        }
        //pthread_mutex_unlock(&mutex);
    }
    return 0;
}

void* insert(void *arg) {
    int i;
    for (i=0; i<11; i++) {
        //pthread_mutex_lock(&mutex);
        value[i] = i;
        kv_put(&key[i], &value[i], kvs);
        printf("[thread %3d] - inserted key %ld\n", *(int*)arg, key[i]);
        //traverse(kvs);
        //pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int main() {
    void* thr_ret;
    pthread_t t_id[101];
    int number[101];
    int i;
    
    //pthread_mutex_init(&mutex, NULL);
    
    kvs = kv_create_db(4);
    
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
    
    for (i=0; i<101; i++) {
        if (pthread_join(t_id[i], &thr_ret) != 0) {
            perror("pthread_join() error");
            exit(-1);
        }
    }
    
    kv_destroy_db(kvs);
    
    //pthread_mutex_destroy(&mutex);
    
    return 0;
}
