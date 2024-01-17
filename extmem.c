// External  SDRAM memory allocation functions.  Attempt to use external memory,
// but automatically fall back to internal RAM if external RAM can't be used.

#include <stdlib.h>
#include "smalloc.h"
#include "wiring.h"

struct smalloc_pool sdram_smalloc_pool;


#define IS_SDRAM(addr) (((uint32_t)addr >> 28) == 8)

void *sdram_malloc(size_t size)
{
    void *ptr = sm_malloc_pool(&sdram_smalloc_pool, size);
    if (ptr) return ptr;
    return malloc(size);
}

void sdram_free(void *ptr)
{
    if (IS_SDRAM(ptr)) {
        sm_free_pool(&sdram_smalloc_pool, ptr);
        return;
    }
    free(ptr);
}

void *sdram_calloc(size_t nmemb, size_t size)
{
    // Note: It is assumed that the pool was created with do_zero set to true
    void *ptr = sm_malloc_pool(&sdram_smalloc_pool, nmemb*size);
    if (ptr) return ptr;
    return calloc(nmemb, size);
}

void *sdram_realloc(void *ptr, size_t size)
{
    if (IS_SDRAM(ptr)) {
        return sm_realloc_pool(&sdram_smalloc_pool, ptr, size);
    }
    return realloc(ptr, size);
}
