# Cloud Computing

- Course: Cloud Computing
- Language: C, Python

There are 7 assignments and the final project.

- [`01`](./01_B-Tree) - _B-Tree Library_
  - Condition: Implement insert/delete/search function.
  - Language: C

- [`02`](./02_Key-Value-Store) - _Key Value Store Library with B-Tree_
  - Condition: Implement (key, value) put/get function.
  - Language: C

- [`03`](./03_ParallelProgramming-with-BTree) - _Key Value Test Program_
  - Condition: Implement parallel test program with (key, value) insert/delete/search function.
  - Language: C

- [`04`](./04_PerformanceTest) - _Performance Test of the Key Value Store_
  - Condition: Test sequential or random read/write function.
  - Language: C
  - Menual:

    ``` C++
    puts("- Test Menual -");
    puts("[1] sequential_write()");
    puts("[2] sequential_read()");
    puts("[3] random_write()");
    puts("[4] random_read()");
    printf("Enter the number: ");
    scanf("%d", &choice);
    ```

- [`05`](./05_MultiThreadTest-Mutex) - _Multi-threading with Mutex_
  - Condition: Implement `Lock Variable` using pthread.h.
  - Language: C
  - Structure:

    ``` C++
    struct KVS_T {
        pthread_mutex_t mutex;
        struct B_TREE_T tree;
    };
    ```

- [`06`](./06_CompareLockVariable) - _Comparing Lock Variables_
  - Condition: Implement to compare RW_LOCK variable with Mutex variable.
  - Language: C
  - Structure:

    ``` C++
    struct KVS_T {
        pthread_rwlock_t rwlock;
        pthread_mutex_t mutex;
        struct B_TREE_T tree;
    };
    ```

- [`07`](./07_ParallelProgramming-with-Skiplist) - _Parallel Test of Key Value Store Library with `Skiplist`_
  - Condition: Test performance for RW_LOCK variable.
  - Language: C
  - Structure:

    ``` C++
    struct KVS_T {
        pthread_rwlock_t rwlock;
        struct SKIPLIST_T skiplist;
    };
    ```

- [`08`](./08_web_app_with_redis) - _Web Application with Redis_
  - Condition: Implement web service with Key-Value-Store (=NoSQL Database).
  - Language: Python 3
  - DevOps: Flask + Redis in docker
