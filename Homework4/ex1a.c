#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// #define BLOCK_SIZE 32
// #define CACHE_SIZE 1024
// #define ASSOCIAVITY 1
// #define SETS (CACHE_SIZE / BLOCK_SIZE / ASSOCIAVITY)

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

cache_t* init_cache(int BLOCK_SIZE, int CACHE_SIZE, int ASSOCIAVITY) {
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
    }
    return cache;
}
void free_cache(cache_t* cache) {
    for (int i = 0; i < cache->SETS; i++) {
        free(cache->sets[i].blocks);
    }
    free(cache->sets);
    free(cache);
}

int access_cache(cache_t *cache, char* ptr) {
    ptrdiff_t mem_addr = ptr - mem_offset;
    int set = (int)floor(mem_addr / cache->BLOCK_SIZE) % cache->SETS;
    ptrdiff_t block_addr = mem_addr - (mem_addr % cache->BLOCK_SIZE);
    cache->total_accesses++;
    if (cache->sets[set].blocks[0].content_addr == block_addr) {
        cache->total_hits++;
        return 1;
    }
    // Miss
    cache->total_misses++;
    cache->sets[set].blocks[0].content_addr = block_addr;
    return 0;
}



// void print_cache() {
//     for (int i = 0; i < SETS; i++) {
//         printf("%d & $%s$ & $%s$ \\\\ \\hline\n", i, cache[i].f1, cache[i].f2);
//     }
// }


cache_t* default_cache;


double stencil(double* A, int lda) {
    access_cache(default_cache, A + 0);
    double acc = A[0];
    access_cache(default_cache, A - lda);
    acc += A[-lda];
    access_cache(default_cache, A + lda);
    acc += A[lda];
    return acc;
}

void comp(double* A, double* O, int n, int s) {
    for (int i = 1; i < n - 1; i++) {
        for (int j = 0, jj = 0; j < n; j += s, jj++) {
            access_cache(default_cache, O + (i - 1) * n / s + jj); 
            O[(i - 1) * n / s + jj] = stencil(&A[i * n + j], n);
        }
    }
}


int main(int argc, char const *argv[])
{   
    const int n = strtol(argv[1], NULL, 10);
    const int s = strtol(argv[2], NULL, 10);
    printf("n: %d\ns: %d\n", n, s);

    default_cache = init_cache(32, 1024, 1);

    double* mem = malloc(n*n*2*sizeof(double));
    mem_offset = (char*)mem;
    
    comp(mem, mem+n*n, n, s);
    
    printf("Total accesses: %ld\nTotal misses: %ld\nTotal hits: %ld\nMiss ratio: %f\n", default_cache->total_accesses, default_cache->total_misses, default_cache->total_hits, (double)default_cache->total_misses/default_cache->total_accesses);
    free_cache(default_cache);
}
