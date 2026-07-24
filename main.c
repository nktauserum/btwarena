#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#define POOL_CAPACITY 16
#define BUFFER_SIZE 1024

typedef struct {
    void* ptrs[POOL_CAPACITY];
    size_t available;
} Pool;

void* pool_get(Pool* pool) {
    if (pool->available > 0) {
        void* ptr = pool->ptrs[--pool->available];
        return ptr;
    }

    return malloc(BUFFER_SIZE);
}

void pool_put(Pool* pool, void* ptr) {
    if (pool->available < POOL_CAPACITY) {
        pool->ptrs[pool->available++] = ptr;
        return;
    }

    free(ptr);
}

int main(void) {
    Pool pool = { .available = 0 };

    void* ptr = pool_get(&pool);
    strcpy(ptr, "Hello World!");
    pool_put(&pool, ptr);

    void* ptr1 = pool_get(&pool);
    printf("ptr1: %s\n", (char*)ptr1); // prints "Hello World!"

    return 0;
}
