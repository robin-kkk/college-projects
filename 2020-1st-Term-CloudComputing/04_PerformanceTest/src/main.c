//
//  main.c
//  Assignment
//
//  Created by User on 23/03/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include "kvs.h"

#include <time.h>

const int MINIMUM_KEY_COUNT = 1000000;
const int THREAD_COUNT = 8;
long *keys;
long *values;
struct KVS_T *kvs;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void test(void);
void unit_test(void* (*p_func)(void*), char *msg);

void initialize_keys(void);
void generate_random_keys(void);
void* sequential_write(void *arg);
void* sequential_read(void *arg);
void* random_write(void *arg);
void* random_read(void *arg);

int main() {
    test();
    return 0;
}

void test() {
    int choice;
    puts("- Test Menual -");
    puts("[1] sequential_write()");
    puts("[2] sequential_read()");
    puts("[3] random_write()");
    puts("[4] random_read()");
    printf("Enter the number: ");
    scanf("%d", &choice);
    
    printf("\n[MAIN] The number of threads: %d\n", THREAD_COUNT);
    kvs = kv_create_db(4);
    initialize_keys();
    int i;
    int key_count = MINIMUM_KEY_COUNT*THREAD_COUNT;
    switch (choice) {
        case 1:
            unit_test(sequential_write, "\n[MAIN] sequential_write()");
            break;
        case 2:
            puts("[MAIN] sequential_write(): writing...");
            for (i=0; i<key_count; i++) {
                struct KVS_NODE_T *new_node = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
                if (new_node == 0) {
                    perror("[ERROR] sequential_write()");
                    exit(-1);
                }
                init_list_head(&new_node->list);
                new_node->key = &keys[i];
                new_node->value = &values[i];
                kv_put(new_node, kvs);
            }
            puts("[MAIN] sequential_write(): finish.");
            unit_test(sequential_read, "\n[MAIN] sequential_read()");
            break;
        case 3:
            generate_random_keys();
            unit_test(random_write, "\n[MAIN] random_write()\n");
            break;
        case 4:
            generate_random_keys();
            puts("[MAIN] random_write(): writing...");
            for (i=0; i<key_count; i++) {
                struct KVS_NODE_T *new_node = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
                if (new_node == 0) {
                    perror("[ERROR] sequential_write()");
                    exit(-1);
                }
                init_list_head(&new_node->list);
                new_node->key = &keys[i];
                new_node->value = &values[i];
                kv_put(new_node, kvs);
            }
            puts("[MAIN] random_write(): finish.");
            unit_test(random_read, "\n[MAIN] random_read()\n");
            break;
        default:
            puts("Please select the number between 1 ~ 4.");
            break;
    }
    kv_destroy_db(kvs);
}


void unit_test(void* (*p_func)(void*), char *msg)
{
    int i;
    void* thr_ret;
    pthread_t t_id[THREAD_COUNT];
    int number[THREAD_COUNT];
    time_t st, et;
    long duration;
    double performance;
    
    st = time(NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    for (i=0; i<THREAD_COUNT; i++) {
        number[i] = i;
        if (pthread_create(&t_id[i],NULL,p_func,(void*)&number[i]) != 0) {
            perror("[ERROR] pthread_create()");
            exit(-1);
        }
    }
    pthread_cond_broadcast(&cond);
    for (i=0; i<THREAD_COUNT; i++) {
        if (pthread_join(t_id[i], &thr_ret) != 0) {
            perror("[ERROR] pthread_join()");
            exit(-1);
        }
    }
    et = time(NULL);
    duration = et - st;
    performance = MINIMUM_KEY_COUNT * THREAD_COUNT / (double)duration;
    printf("%s\n", msg);
    printf("Duration: %ld sec\n", duration);
    printf("Performance: %lf operations per 1 sec\n\n", performance);
}


void initialize_keys() {
    int i;
    int key_count = MINIMUM_KEY_COUNT * THREAD_COUNT;
    if (!(keys = (long*) calloc(key_count, sizeof(long)))) {
        perror("[ERROR] initialize_keys()");
        exit(-1);
    }
    
    if (!(values = (long*) calloc(key_count, sizeof(long)))) {
        perror("[ERROR] initialize_keys()");
        exit(-1);
    }
    
    for (i=0; i<key_count; i++) {
        keys[i] = i+1;
        values[i] = 0xdeadbeef ^ keys[i];
    }
    puts("[MAIN] Sequential Keys Generated!");
}

void generate_random_keys() {
    int i;
    int key_count = MINIMUM_KEY_COUNT * THREAD_COUNT;
    srand(time(NULL));
    for (i=0; i<key_count; i++) {
        int pos = random() % key_count;
        long tmp = keys[i];
        keys[i] = keys[pos];
        keys[pos] = tmp;
    }
    puts("[MAIN] Random Keys Generated!");
}


void* sequential_write(void *arg) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    int i;
    int t_id = *(int*)arg;
    int start = MINIMUM_KEY_COUNT * t_id;
    int end = MINIMUM_KEY_COUNT * (t_id + 1);
    
    printf("[THREAD-%d] sequential_write(): writing...\n", t_id);
    for (i=start; i<end; i++) {
        struct KVS_NODE_T *new_node = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
        if (new_node == 0) {
            perror("[ERROR] sequential_write()");
            exit(-1);
        }
        init_list_head(&new_node->list);
        new_node->key = &keys[i];
        new_node->value = &values[i];
        pthread_mutex_lock(&kvs->mutex);
        kv_put(new_node, kvs);
        pthread_mutex_unlock(&kvs->mutex);
    }
    printf("[THREAD-%d] sequential_write(): finish.\n", t_id);
    pthread_mutex_unlock(&mutex);
    return 0;
}


void* sequential_read(void *arg) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    int i;
    int t_id = *(int*)arg;
    int start = MINIMUM_KEY_COUNT * t_id;
    int end = MINIMUM_KEY_COUNT * (t_id + 1);
    int not_found_cnt = 0;
    
    printf("[THREAD-%d] sequential_read(): reading...\n", t_id);
    for (i=start; i<end; i++) {
        pthread_mutex_lock(&kvs->mutex);
        struct KVS_NODE_T *node = kv_get(&keys[i], kvs);
        pthread_mutex_unlock(&kvs->mutex);
        if (node == 0) not_found_cnt++;
    }
    printf("[THREAD-%d] sequential_read(): not found count = %d\n", t_id, not_found_cnt);
    pthread_mutex_unlock(&mutex);
    return 0;
}


void* random_write(void *arg) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    int i;
    int t_id = *(int*)arg;
    int start = MINIMUM_KEY_COUNT * t_id;
    int end = MINIMUM_KEY_COUNT * (t_id + 1);
    
    printf("[THREAD-%d] random_write(): writing...\n", t_id);
    for (i=start; i<end; i++) {
        struct KVS_NODE_T *new_node = (struct KVS_NODE_T*) malloc(sizeof(struct KVS_NODE_T));
        if (new_node == 0) {
            perror("[ERROR] random_write()");
            exit(-1);
        }
        init_list_head(&new_node->list);
        new_node->key = &keys[i];
        new_node->value = &values[i];
        pthread_mutex_lock(&kvs->mutex);
        kv_put(new_node, kvs);
        pthread_mutex_unlock(&kvs->mutex);
    }
    printf("[THREAD-%d] random_write(): finish.\n", t_id);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* random_read(void *arg) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    int i;
    int t_id = *(int*)arg;
    int start = MINIMUM_KEY_COUNT * t_id;
    int end = MINIMUM_KEY_COUNT * (t_id + 1);
    int not_found_cnt = 0;
    
    printf("[THREAD-%d] random_read(): reading...\n", t_id);
    for (i=start; i<end; i++) {
        pthread_mutex_lock(&kvs->mutex);
        struct KVS_NODE_T *node = kv_get(&keys[i], kvs);
        if (node == 0) not_found_cnt++;
        pthread_mutex_unlock(&kvs->mutex);
    }
    printf("[THREAD-%d] random_read(): not found count = %d\n", t_id, not_found_cnt);
    pthread_mutex_unlock(&mutex);
    return 0;
}
