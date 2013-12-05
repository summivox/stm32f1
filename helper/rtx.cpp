//helper/rtx
#include "helper/rtx.hpp"

#include "stdlib.h"

//sleep for given duration, blocking
void wait_ms(U32 Tms){
    U32 Tms_before=os_time_get();
    while(I32(os_time_get()-Tms_before)<Tms);
}

void* MALLOC(size_t n) {
    tsk_lock();
    void* ret = malloc(n);
    tsk_unlock();
    return ret;
}

void FREE(void* p) {
    tsk_lock();
    free(p);
    tsk_unlock();
}
