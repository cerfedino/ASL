#include <stddef.h>

struct cache_block {
    // Uses content address instead of the actual content for simplicity
    ptrdiff_t content_addr;
    char content[20];
}; 
struct cache_set {
    // The array of cache blocks, determined by ASSOCIAVITY
    struct cache_block* blocks;
    // Array of LRU values for each block. The block with the highest LRU value is the least recently used
    int* LRU;
};

// Struct for the cache
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

// Initializes a cache with the given parameters.
// MEM_OFFSET can be used for aligning the cache to a specific memory address
cache_t* cache_init(int BLOCK_SIZE, int CACHE_SIZE, int ASSOCIAVITY, char* MEM_OFFSET);
void cache_free(cache_t* cache);

// Given a cache block, sets the LRU value to 0 and increments the LRU value of all other blocks
void cache_update_LRU(cache_t* cache, int set_idx, int block_idx);
// Returns the cache index of the least recently used block in the set
int cache_get_LRU(cache_t* cache, int set_idx);
// Given a byte pointer, tries to read the content from the cache. If the content is not in the cache, it is loaded into the cache. Returns 1 if its a hit, 0 if its a miss
// The string_identifier is used for debugging purposes. The string gets stored inside the cache block.
int cache_access(cache_t *cache, char* ptr, char* string_identifier);
// Prints the cache contents formatted as a LaTeX table
void cache_print(cache_t *cache);
