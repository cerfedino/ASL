#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cache.h"



cache_t* cache_init(int BLOCK_SIZE, int CACHE_SIZE, int ASSOCIAVITY, char* MEM_OFFSET) {
    cache_t* cache = malloc(sizeof(cache_t));
    cache->BLOCK_SIZE = BLOCK_SIZE;
    cache->CACHE_SIZE = CACHE_SIZE;
    cache->ASSOCIAVITY = ASSOCIAVITY;
    cache->SETS = CACHE_SIZE / BLOCK_SIZE / ASSOCIAVITY;
    cache->sets = malloc(cache->SETS * sizeof(struct cache_set));
    cache->MEM_OFFSET = MEM_OFFSET;
    for (int i = 0; i < cache->SETS; i++) {
        cache->sets[i].blocks = malloc(ASSOCIAVITY * sizeof(struct cache_block));
        for (int j = 0; j < ASSOCIAVITY; j++) {
            cache->sets[i].blocks[j].content_addr = -1;
        }
        cache->sets[i].LRU = calloc(ASSOCIAVITY, sizeof(int));
        cache->sets[i].LRU[0] = 1;
    }
    return cache;
}
void cache_free(cache_t* cache) {
    for (int i = 0; i < cache->SETS; i++) {
        free(cache->sets[i].blocks);
    }
    free(cache->sets);
    free(cache);
}

void cache_update_LRU(cache_t* cache, int set_idx, int block_idx) {
    struct cache_set set = cache->sets[set_idx];

    set.LRU[block_idx] = 0;
    for (int i = 0; i < cache->ASSOCIAVITY; i++) {
        if (i != block_idx) {
            set.LRU[i]++;
        }
    }
}
int cache_get_LRU(cache_t* cache, int set_idx) {
    struct cache_set set = cache->sets[set_idx];
    struct cache_block *block;
    int max_LRU = 0;
    int max_LRU_idx = 0;
    for (int i = 0; i < cache->ASSOCIAVITY; i++) {
        if (set.LRU[i] > max_LRU) {
            max_LRU = set.LRU[i];
            max_LRU_idx = i;
        }
    }
    return max_LRU_idx;
}

int cache_read(cache_t *cache, char* ptr) {
    cache->total_accesses++;

    ptrdiff_t mem_addr = ptr - cache->MEM_OFFSET;
    int set_idx = (int)floor(mem_addr / cache->BLOCK_SIZE) % cache->SETS;
    ptrdiff_t block_addr = mem_addr - (mem_addr % cache->BLOCK_SIZE);
    struct cache_set set = cache->sets[set_idx];
    
    struct cache_block block;

    for (int i = 0; i < cache->ASSOCIAVITY; i++) {
        if (set.blocks[i].content_addr == block_addr) {
            // Hit
            cache->total_hits++;
            cache_update_LRU(cache, set_idx, i);
            return 1;
        }
    }

    // Miss
    cache->total_misses++;
    int LRU_idx = cache_get_LRU(cache, set_idx);
    set.blocks[LRU_idx].content_addr = block_addr;
    cache_update_LRU(cache, set_idx, LRU_idx);
    return 0;
}