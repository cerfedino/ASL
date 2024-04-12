#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct cache_block {
    // Uses content address instead of the actual content for simplicity
    ptrdiff_t content_addr;
}; 
struct cache_set {
    struct cache_block* blocks;
    int* LRU;
};
struct cache_s {
    struct cache_set* sets;
    int BLOCK_SIZE;
    int CACHE_SIZE;
    int ASSOCIAVITY;
    int SETS;
    char* MEM_OFFSET;

    long total_hits;
    long total_misses;
    long total_accesses;
};
typedef struct cache_s cache_t;

cache_t* cache_init(int BLOCK_SIZE, int CACHE_SIZE, int ASSOCIAVITY, char* MEM_OFFSET);
void cache_free(cache_t* cache);

void cache_update_LRU(cache_t* cache, int set_idx, int block_idx);
int cache_get_LRU(cache_t* cache, int set_idx);

int cache_read(cache_t *cache, char* ptr);