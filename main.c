#include <stdio.h>

#define BUFFER_SIZE 1024
#include "pool.h"

void print_bitmap(Pool* pool) {
    for (int offset = 0; offset < sizeof(pool->bitmap) * CHAR_BIT; ++offset) {
        char bit = pool->bitmap & (1 << offset) ? '1' : '0';
        putc(bit, stdout);
    }
    putc('\n', stdout);
}

int main(void) {
    Pool pool = {0};
    pool_init(&pool);

    void* ptr = pool_get(&pool);
    pool_put(&pool, ptr);

    void* ptr1 = pool_get(&pool);
    pool_put(&pool, ptr1);

    return 0;
}
