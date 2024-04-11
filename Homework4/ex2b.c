#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHE_SETS 4
struct data_t {
    float a;
    float b;
    float c;
    float d;
};
typedef struct data_t data_t;

struct cache_block {
    char f1[10];
    char f2[10];
};
struct cache_set {
    struct cache_block blocks[2];
    int LRU;
};
struct cache_set cache[CACHE_SETS];


int read_cache(int set, struct cache_block cmp) {
    set = set % CACHE_SETS;
    if (strcmp(cache[set].blocks[0].f1, cmp.f1) == 0 &&
        strcmp(cache[set].blocks[0].f2, cmp.f2) == 0) {
        cache[set].LRU = 1;
        return 1;
    } else if(strcmp(cache[set].blocks[1].f1, cmp.f1) == 0 &&
        strcmp(cache[set].blocks[1].f2, cmp.f2) == 0) {
        cache[set].LRU = 0;
        return 1;
    } 
    if(cache[set].LRU != 0 && cache[set].LRU != 1) {
        cache[set].LRU = 0;
    }
    strcpy(cache[set].blocks[cache[set].LRU].f1, cmp.f1);
    strcpy(cache[set].blocks[cache[set].LRU].f2, cmp.f2);
    cache[set].LRU = (cache[set].LRU + 1) % 2;
    return 0;
}

void print_cache() {
    for (int i = 0; i < CACHE_SETS; i++) {
        printf("%d & $%s$ & $%s$ & $%s$ & $%s$ \\\\ \\hline \n", i, cache[i].blocks[0].f1, cache[i].blocks[0].f2, cache[i].blocks[1].f1, cache[i].blocks[1].f2);
    }
}

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
            // printf("\t x[%d].a - Cache set #: %d\n", x_idx, (x_idx*2) % 8);
            int y_idx = j1 * (i1 + j1) % 4;
            // printf("\t y[%d].b - Cache set #: %d\n", y_idx, (20 + y_idx*2) %
            // 8);

            struct cache_block cmp;
            sprintf(cmp.f1, "x_%d.a", x_idx);
            sprintf(cmp.f2, "x_%d.b", x_idx);
            x_pattern[x_pattern_idx++] = read_cache(x_idx * 2, cmp) ? 'H' : 'M';

            sprintf(cmp.f1, "y_%d.a", y_idx);
            sprintf(cmp.f2, "y_%d.b", y_idx);
            y_pattern[y_pattern_idx++] =
                read_cache(20 + y_idx * 2, cmp) ? 'H' : 'M';

            // float x0 = x[i1 * (i1 + j1) % 4].a;
            // float y0 = y[j1 * (i1 + j1) % 4].b;
            // res += x0 * y0;
        }
        x_pattern[x_pattern_idx] = '\0';
        y_pattern[y_pattern_idx] = '\0';
        printf("X: %s\n", x_pattern);
        printf("Y: %s\n", y_pattern);
        x_pattern_idx = 0;
        y_pattern_idx = 0;
        print_cache();
        printf("\n");
    }
    printf("\n\n ===\nLine 29\n\n");
    for (int i = 0; i < 2; i++) {
        int i1 = i + 1;
        printf("i: %d\n", i);
        for (int j = 0; j <= 10; j += 5) {
            int j1 = j + 1;
            int x_idx = i1 * (i1 + j1) % 7;
            // printf("\t x[%d].c - Cache set #: %d\n", x_idx, (x_idx * 2 + 1) % 8);
            int y_idx = j1 * (i1 + j1) % 7;
            // printf("\t y[%d].d - Cache set #: %d\n", y_idx, (20 + y_idx * 2 + 1) % 8);

            struct cache_block cmp;
            sprintf(cmp.f1, "x_%d.c", x_idx);
            sprintf(cmp.f2, "x_%d.d", x_idx);
            x_pattern[x_pattern_idx++] =
                read_cache(x_idx * 2 + 1, cmp) ? 'H' : 'M';

            sprintf(cmp.f1, "y_%d.c", y_idx);
            sprintf(cmp.f2, "y_%d.d", y_idx);
            y_pattern[y_pattern_idx++] =
                read_cache(20 + y_idx * 2 + 1, cmp) ? 'H' : 'M';

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
        print_cache();
        printf("\n");
    }
    return res;
}

int main(int argc, char const *argv[]) {
    data_t x[10];
    data_t y[10];
    comp(x, y);
    return 0;
}
