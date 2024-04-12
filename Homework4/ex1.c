#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cache.h"



cache_t* default_cache;


double stencil(double* A, int lda) {
    cache_access(default_cache, A + 0, "");
    double acc = A[0];
    cache_access(default_cache, A - lda, "");
    acc += A[-lda];
    cache_access(default_cache, A + lda, "");
    acc += A[lda];
    return acc;
}

void comp(double* A, double* O, int n, int s) {
    for (int i = 1; i < n - 1; i++) {
        for (int j = 0, jj = 0; j < n; j += s, jj++) {
            O[(i - 1) * n / s + jj] = stencil(&A[i * n + j], n);
            cache_access(default_cache, O + (i - 1) * n / s + jj, ""); 
        }
    }
}

void ex1a(int associativity) {
    int s = 1, n = 8;
    printf("\nn: %d s: %d\n", n, s);

    double* mem = malloc(n*n*2*sizeof(double));

    default_cache = cache_init(32, 1024, associativity, (char*)mem);


    comp(mem, mem+n*n, n, s);
    
    printf("Accesses: %ld\nMisses: %ld\nHits: %ld\nMiss rate: %f\%\n", default_cache->total_accesses, default_cache->total_misses, default_cache->total_hits, (double)default_cache->total_misses/default_cache->total_accesses*100);
    cache_free(default_cache);
}
void ex1b(int associativity) {
    int s = 2, n = 16;
    printf("\nn: %d s: %d\n", n, s);

    double* mem = malloc(n*n*2*sizeof(double));

    default_cache = cache_init(32, 1024, associativity, (char*)mem);
    
    comp(mem, mem+n*n, n, s);
    
    printf("Accesses: %ld\nMisses: %ld\nHits: %ld\nMiss rate: %f\%\n", default_cache->total_accesses, default_cache->total_misses, default_cache->total_hits, (double)default_cache->total_misses/default_cache->total_accesses*100);
    cache_free(default_cache);
}



int main(int argc, char const *argv[]) {
    printf("\n======\nAssociativity 1\n");   
    ex1a(1);
    ex1b(1);
    printf("\n======\nAssociativity 2\n");
    ex1b(2);
}
