#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define POOL_CAPACITY 16
#define BUFFER_SIZE 1024

typedef struct {
    uint16_t bitmap;
    char arena[POOL_CAPACITY * BUFFER_SIZE];
} Pool;

void pool_init(Pool* pool) {
    pool->bitmap = ~0; // all fields are available
}

void* pool_get(Pool* pool) {
    for (int offset = 0; offset < sizeof(pool->bitmap) * CHAR_BIT; ++offset) {
        int bit_offset = offset % sizeof(pool->bitmap) * CHAR_BIT;
        bool bit = pool->bitmap & (1 << bit_offset); // get the bit with specified offset

        if (bit != 0) {
            pool->bitmap = pool->bitmap ^ (1 << bit_offset); // Invert the bit
            return &pool->arena[offset * BUFFER_SIZE];
        }
    }

    return NULL;
}

void pool_put(Pool* pool, void* ptr) {
    int offset = ((char*)ptr - &pool->arena[0]) / BUFFER_SIZE;
    pool->bitmap = pool->bitmap ^ (1 << offset % (sizeof(pool->bitmap) * CHAR_BIT));
    memset(ptr, 0, BUFFER_SIZE);
}

void print_bitmap(Pool* pool) {
    for (int offset = 0; offset < sizeof(pool->bitmap) * CHAR_BIT; ++offset) {
        char bit = pool->bitmap & (1 << offset % sizeof(pool->bitmap) * CHAR_BIT) ? '1' : '0';
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
