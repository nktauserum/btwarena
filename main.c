#include <stdio.h>
#include <string.h>

#define SIZE 1024
#define CAPACITY 16
#include "pool.h"

void print_bitmap(Pool* pool) {
    for (int offset = 0; offset < pool->count; ++offset) {
        char bit = pool->bitmap[offset/CHAR_BIT] & (1 << offset%CHAR_BIT) ? '1' : '0';
        putc(bit, stdout);
    }
    putc('\n', stdout);
}

int main(void) {
    Pool pool = {0}; // initialize the pool
    pool_init(&pool, CAPACITY, SIZE);

    void* ptr = pool_get(&pool); // alloc

    strcpy(ptr, "hello from the block 1");
    printf("Wrote %lu bytes to %p: %s\n", strlen(ptr), ptr, (char*)ptr);

    printf("Take a look how bitmap looks right now. All blocks are available except the first one: ");
    print_bitmap(&pool);

    pool_put(&pool, ptr); // free allocated block

    void* ptr1 = pool_get(&pool);
    printf("Wonderful! On the second allocation we got a block with the same address: %p\n", ptr1);
    pool_put(&pool, ptr1);

    for (int i = 0; i < CAPACITY; ++i) 
        pool_get(&pool);
    
    printf("We've allocated all memory. Now there are no free blocks: ");
    print_bitmap(&pool);

    void* nullptr = pool_get(&pool);
    printf("And the next allocation will give us NULL-pointer (%p)\n", nullptr);

    return 0;
}
