#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



char* mem_offset;

long hits = 0;
long misses = 0;
long total = 0;


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

    long total_hits;
    long total_misses;
    long total_accesses;
};
typedef struct cache_s cache_t;

cache_t* cache_init(int BLOCK_SIZE, int CACHE_SIZE, int ASSOCIAVITY) {
    cache_t* cache = malloc(sizeof(cache_t));
    cache->BLOCK_SIZE = BLOCK_SIZE;
    cache->CACHE_SIZE = CACHE_SIZE;
    cache->ASSOCIAVITY = ASSOCIAVITY;
    cache->SETS = CACHE_SIZE / BLOCK_SIZE / ASSOCIAVITY;
    cache->sets = malloc(cache->SETS * sizeof(struct cache_set));
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

int cache_access(cache_t *cache, char* ptr) {
    cache->total_accesses++;

    ptrdiff_t mem_addr = ptr - mem_offset;
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



// void print_cache() {
//     for (int i = 0; i < SETS; i++) {
//         printf("%d & $%s$ & $%s$ \\\\ \\hline\n", i, cache[i].f1, cache[i].f2);
//     }
// }


cache_t* default_cache;


double stencil(double* A, int lda) {
    cache_access(default_cache, A + 0);
    double acc = A[0];
    cache_access(default_cache, A - lda);
    acc += A[-lda];
    cache_access(default_cache, A + lda);
    acc += A[lda];
    return acc;
}

void comp(double* A, double* O, int n, int s) {
    for (int i = 1; i < n - 1; i++) {
        for (int j = 0, jj = 0; j < n; j += s, jj++) {
            O[(i - 1) * n / s + jj] = stencil(&A[i * n + j], n);
            cache_access(default_cache, O + (i - 1) * n / s + jj); 
        }
    }
}

void ex1a(int associativity) {
    int s = 1, n = 8;
    printf("\nn: %d s: %d\n", n, s);

    default_cache = cache_init(32, 1024, associativity);

    double* mem = malloc(n*n*2*sizeof(double));
    mem_offset = (char*)mem;
    
    comp(mem, mem+n*n, n, s);
    
    printf("Accesses: %ld\nMisses: %ld\nHits: %ld\nMiss rate: %f\%\n", default_cache->total_accesses, default_cache->total_misses, default_cache->total_hits, (double)default_cache->total_misses/default_cache->total_accesses*100);
    cache_free(default_cache);
}
void ex1b(int associativity) {
    int s = 2, n = 16;
    printf("\nn: %d s: %d\n", n, s);

    default_cache = cache_init(32, 1024, associativity);

    double* mem = malloc(n*n*2*sizeof(double));
    mem_offset = (char*)mem;
    
    comp(mem, mem+n*n, n, s);
    
    printf("Accesses: %ld\nMisses: %ld\nHits: %ld\nMiss rate: %f\%\n", default_cache->total_accesses, default_cache->total_misses, default_cache->total_hits, (double)default_cache->total_misses/default_cache->total_accesses*100);
    cache_free(default_cache);
}



int main(int argc, char const *argv[]) {
    printf("======\nAssociativity 1\n");   
    ex1a(1);
    ex1b(1);
    printf("======\nAssociativity 2\n");
    ex1b(2);
}
