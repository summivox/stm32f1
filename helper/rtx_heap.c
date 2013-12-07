#include "rtl.h"
//#include "stdio.h"


////////////
// ensure thread safety for heap allocation

static OS_MUT heap_mut;
static void heap_mut_init() {
    static char done = 0;
    if (!done) {
        os_mut_init(&heap_mut);
        done = 1;
    }
}

extern void* $Super$$malloc(size_t);
void* $Sub$$malloc(size_t n) {
    void* ret;
    heap_mut_init();
    os_mut_wait(&heap_mut, 0xFFFF);
    ret = $Super$$malloc(n);
    //printf("$$$ malloc(%u) => 0x%08X\r\n", n, (unsigned)ret); //DEBUG
    os_mut_release(&heap_mut);
    return ret;
}

extern void $Super$$free(void*);
void $Sub$$free(void* p) {
    heap_mut_init();
    os_mut_wait(&heap_mut, 0xFFFF);
    $Super$$free(p);
    //printf("$$$ free(0x%08X)\r\n", (unsigned)p); //DEBUG
    os_mut_release(&heap_mut);
}
