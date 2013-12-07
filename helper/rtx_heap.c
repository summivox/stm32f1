#include "rtl.h"
//#include "stdio.h"


////////////
// ensure thread safety for heap allocation

//static OS_MUT heap_mut;
static char inited = 0;
static OS_MUT heap_mut;
void heap_mut_init() {
    os_mut_init(&heap_mut);
    inited = 1;
}

extern void* $Super$$malloc(size_t);
void* $Sub$$malloc(size_t n) {
    void* ret;
    if (!inited) {
        return $Super$$malloc(n);
    }
    os_mut_wait(&heap_mut, 0xFFFF);
    //tsk_lock();
    ret = $Super$$malloc(n);
    //printf("$$$ malloc(%u) => 0x%08X\r\n", n, (unsigned)ret); //DEBUG
    //tsk_unlock();
    os_mut_release(&heap_mut);
    return ret;
}

extern void $Super$$free(void*);
void $Sub$$free(void* p) {
    if (!inited) {
        $Super$$free(p);
        return;
    }
    os_mut_wait(&heap_mut, 0xFFFF);
    //tsk_lock();
    $Super$$free(p);
    //printf("$$$ free(0x%08X)\r\n", (unsigned)p); //DEBUG
    //tsk_unlock();
    os_mut_release(&heap_mut);
}
