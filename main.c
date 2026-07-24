#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#define POOL_CAPACITY 16

typedef struct {
    void* ptrs[POOL_CAPACITY];
    size_t available;
    size_t buffer_size;
} Pool;

void pool_init(Pool* pool, size_t buffer_size) {
    pool->available = 0;
    pool->buffer_size = buffer_size;

    for (int i = 0; i < POOL_CAPACITY; ++i) {
        void* buf = malloc(pool->buffer_size);
        if (buf) {
            pool->ptrs[pool->available++] = buf;
        }
    }
}

void pool_destroy(Pool* pool) {
    while (pool->available != 0) {
        free(pool->ptrs[pool->available--]);
    }
}

void* pool_get(Pool* pool) {
    if (pool->available > 0) {
        void* ptr = pool->ptrs[--pool->available];
        return ptr;
    }

    return malloc(pool->buffer_size);
}

void pool_put(Pool* pool, void* ptr) {
    if (pool->available < POOL_CAPACITY) {
        memset(ptr, 0, pool->buffer_size);
        pool->ptrs[pool->available++] = ptr;
        return;
    }

    free(ptr);
}

int main(void) {
    Pool pool = {0};
    pool_init(&pool, 1024);

    void* ptr = pool_get(&pool);
    strcpy(ptr, "Hello World!");
    pool_put(&pool, ptr);
    printf("Available: %lu\n", pool.available);

    void* ptr1 = pool_get(&pool);
    printf("Available: %lu\n", pool.available);
    pool_destroy(&pool);
    printf("Available: %lu\n", pool.available);

    return 0;
}
