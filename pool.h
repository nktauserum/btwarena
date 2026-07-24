#ifndef _POOL_H
#define _POOL_H

#include <stdint.h>
#include <string.h>
#include <limits.h>

#define POOL_CAPACITY 16

#ifndef BUFFER_SIZE 
    #error "Error: buffer size is not defined"
#endif // BUFFER_SIZE

typedef struct {
    uint16_t bitmap;
    char arena[POOL_CAPACITY * BUFFER_SIZE];
} Pool;

void pool_init(Pool* pool) {
    pool->bitmap = ~0; // all fields are available
    memset(&pool->arena[0], 0, POOL_CAPACITY * BUFFER_SIZE);
}

void* pool_get(Pool* pool) {
    for (int offset = 0; offset < sizeof(pool->bitmap) * CHAR_BIT; ++offset) {
        if ((pool->bitmap & (1u << offset)) != 0) {
            pool->bitmap = pool->bitmap ^ (1u << offset); // XOR the bit - set as false
            return &pool->arena[offset * BUFFER_SIZE];
        }
    }

    return NULL;
}

void pool_put(Pool* pool, void* ptr) {
    int offset = ((char*)ptr - &pool->arena[0]) / BUFFER_SIZE; // a little **magic** of pointer arithmetic
    if (offset < 0 || offset >= POOL_CAPACITY) return; // decline if ptr is possibly not within the arena

    pool->bitmap |= (1u << offset); // set as true (always)
    memset(ptr, 0, BUFFER_SIZE); // clear given buffr
}

#endif // _POOL_H
