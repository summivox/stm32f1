//helper/rtx
//Prerequisite: RTX
#ifndef _HELPER_RTX_HPP_
#define _HELPER_RTX_HPP_

#include "rtl.h"
#include "helper/conf.hpp"
#include "helper/def.hpp"
#include "helper/assert.hpp"





//in RTX operations with timeout, 0xffff means "no timeout" or "forever"
static const U16 FOREVER=0xFFFF;

//sleep for given duration, blocking
void wait_ms(U32 Tms);

//simple type-safe wrapper class for RTX memory pool
//roughly following the naming convention of std::allocator
//NOTE: DOES NOT construct the object in-place! Only memory is allocated.
template <typename T, size_t N>
struct Pool{
    _declare_box(pool, sizeof(T), N);
    int remaining;

    Pool(){
        init();
    }
    void init(){
        _init_box(pool, sizeof(pool), sizeof(T));
        remaining=N;
    }
    T* allocate(){
		__disable_irq();
        assert(remaining-->0);
        T* ret=(T*)_alloc_box(pool);
		__enable_irq();
		return ret;
    }
    void deallocate(T* p){
		__disable_irq();
        assert(!_free_box(pool, p)); //return 1 => error
        ++remaining;
		__enable_irq();
    }
};

#endif /* _HELPER_RTX_HPP_ */
