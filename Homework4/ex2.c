#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"
struct data_t {
    float a;
    float b;
    float c;
    float d;
};
typedef struct data_t data_t;

cache_t* default_cache;

char x_pattern[10];
int x_pattern_idx = 0;
char y_pattern[10];
int y_pattern_idx = 0;



float comp(data_t x[10], data_t y[10]) {
    printf("\n\n ===\nLine 18\n\n");
    float res = 0;
    for (int i = 0; i < 2; i++) {
        int i1 = i + 1;
        printf("i: %d\n", i);
        for (int j = 0; j <= 4; j += 2) {
            int j1 = j + 1;
            int x_idx = i1 * (i1 + j1) % 4;
            // printf(" x[%d].a - Cache set #: %d\n", x_idx, (x_idx*2) % 8);
            int y_idx = j1 * (i1 + j1) % 4;
            // printf(" y[%d].b - Cache set #: %d\n", y_idx, (20 + y_idx*2) %
            // 8);

            char string_identifier[20];
            sprintf(string_identifier, "$x_%d.a$\\quad$x_%d.b$", x_idx,x_idx);
            x_pattern[x_pattern_idx++] = cache_access(default_cache, &(x[x_idx].a), string_identifier) ? 'H' : 'M';

            // sprintf(cmp.f1, "y_%d.a", y_idx);
            // sprintf(cmp.f2, "y_%d.b", y_idx);
            sprintf(string_identifier, "$y_%d.a$\\quad$y_%d.b$", y_idx,y_idx);
            y_pattern[y_pattern_idx++] = cache_access(default_cache, &(y[y_idx].b), string_identifier) ? 'H' : 'M';

        }
        x_pattern[x_pattern_idx] = '\0';
        y_pattern[y_pattern_idx] = '\0';
        printf("X: %s\n", x_pattern);
        printf("Y: %s\n", y_pattern);
        x_pattern_idx = 0;
        y_pattern_idx = 0;
        cache_print(default_cache);
        printf("\n");
    }
    printf("\n\n ===\nLine 29\n\n");
    for (int i = 0; i < 2; i++) {
        int i1 = i + 1;
        printf("i: %d\n", i);
        for (int j = 0; j <= 10; j += 5) {
            int j1 = j + 1;
            int x_idx = i1 * (i1 + j1) % 7;
            // printf(" x[%d].c - Cache set #: %d\n", x_idx, (x_idx * 2 + 1) % 8);
            int y_idx = j1 * (i1 + j1) % 7;
            // printf(" y[%d].d - Cache set #: %d\n", y_idx, (20 + y_idx * 2 + 1) % 8);
            

            char string_identifier[20];
            sprintf(string_identifier, "$x_%d.c$\\quad$x_%d.d$", x_idx,x_idx);

            x_pattern[x_pattern_idx++] = cache_access(default_cache, &(x[x_idx].c), string_identifier) ? 'H' : 'M';
            

            sprintf(string_identifier, "$y_%d.c$\\quad$y_%d.d$", y_idx,y_idx);
            y_pattern[y_pattern_idx++] = cache_access(default_cache, &(y[y_idx].d), string_identifier) ? 'H' : 'M';

            // float x0 = x[i1 * (i1 + j1) % 7].c;
            // float y0 = y[j1 * (i1 + j1) % 7].d;
            // res -= x0 * y0;
        }  // Draw state of cache and write hit miss pattern here
        x_pattern[x_pattern_idx] = '\0';
        y_pattern[y_pattern_idx] = '\0';
        printf("X: %s\n", x_pattern);
        printf("Y: %s\n", y_pattern);
        x_pattern_idx = 0;
        y_pattern_idx = 0;
        cache_print(default_cache);
        printf("\n");
    }
    return res;
}

void ex2a(int associativity) {
    data_t* mem = malloc(20*sizeof(data_t));

    default_cache = cache_init(8, 64, associativity, (char*)mem);

    comp(mem, mem+10);
    cache_free(default_cache);
}


int main(int argc, char const *argv[]) {
    printf("\n======\nAssociativity 1\n");   
    ex2a(1);
    printf("\n======\nAssociativity 2\n");   
    ex2a(2);
    return 0;
}
