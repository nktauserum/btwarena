#ifndef _POOL_H
#define _POOL_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

typedef struct {
    uint8_t* bitmap;
    size_t size, count; // count tells us how many buffers is in the pool, size - what size is each buffer 
    char* arena;
} Pool;

void pool_init(Pool* pool, size_t count, size_t size) {
    pool->count = count;
    pool->size = size;
 
    int blocks_count = (size + 8 - 1) / 8 * 8; 
    pool->bitmap = (uint8_t*)malloc(blocks_count*sizeof(uint8_t));
    memset(pool->bitmap, ~0, blocks_count*sizeof(uint8_t));

    pool->arena = (char*)malloc(count*size);         
    memset(pool->arena, 0, count * size);
}

void* pool_get(Pool* pool) {
    for (int offset = 0; offset < pool->count; ++offset) {
        if ((pool->bitmap[offset/CHAR_BIT] & (1u << offset%CHAR_BIT)) != 0) {
            pool->bitmap[offset/CHAR_BIT] = pool->bitmap[offset/CHAR_BIT] ^ (1u << offset%CHAR_BIT); // XOR the bit - set as false
            return &pool->arena[offset * pool->size];
        }
    }

    return NULL;
}

void pool_put(Pool* pool, void* ptr) {
    int offset = ((char*)ptr - pool->arena) / pool->size; // a little **magic** of pointer arithmetic
    if (offset < 0 || offset >= pool->count) return; // decline if ptr is possibly not within the arena

    pool->bitmap[offset/CHAR_BIT] |= (1u << offset%CHAR_BIT); // set as true (always)
    memset(ptr, 0, pool->size); // clear given buffr
}

void pool_destroy(Pool* pool) {
    free(pool->arena);
    free(pool->bitmap);
}

#endif // _POOL_H
