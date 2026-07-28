#ifndef _POOL_H
#define _POOL_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

typedef struct {
    uint16_t bitmap;
    size_t size, count; // count tells us how many buffers is in the pool, size - what size is each buffer 
    char* arena;
} Pool;

void pool_init(Pool* pool, size_t count, size_t size) {
    pool->count = count < 8 ? 8 : count;
    pool->size = size;
    
    pool->bitmap = ~0; // all fields are available
    pool->arena = (char*)malloc(count*size);         
    memset(pool->arena, 0, count * size);
}

void* pool_get(Pool* pool) {
    for (int offset = 0; offset < sizeof(pool->bitmap) * CHAR_BIT; ++offset) {
        if ((pool->bitmap & (1u << offset)) != 0) {
            pool->bitmap = pool->bitmap ^ (1u << offset); // XOR the bit - set as false
            return &pool->arena[offset * pool->size];
        }
    }

    return NULL;
}

void pool_put(Pool* pool, void* ptr) {
    int offset = ((char*)ptr - pool->arena) / pool->size; // a little **magic** of pointer arithmetic
    if (offset < 0 || offset >= pool->count) return; // decline if ptr is possibly not within the arena

    pool->bitmap |= (1u << offset); // set as true (always)
    memset(ptr, 0, pool->size); // clear given buffr
}

#endif // _POOL_H
