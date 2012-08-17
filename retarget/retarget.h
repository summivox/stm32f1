//retarget/retarget
#ifndef _RETARGET_RETARGET_H_
#define _RETARGET_RETARGET_H_

#include <stdio.h>




#ifdef __cplusplus
extern "C"{
#endif//__cplusplus

    struct __FILE {
        int handle;
        void* data; //points to arbitrary data field / pimpl
    };

    extern FILE __stdout;
    extern FILE __stdin;
    extern FILE __stderr;

#ifdef __cplusplus
}
#endif//__cplusplus




#endif /* _RETARGET_RETARGET_H_ */
