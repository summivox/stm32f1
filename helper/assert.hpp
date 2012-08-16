//helper/assert
//Prerequisite: stdio
#ifndef _HELPER_ASSERT_HPP_
#define _HELPER_ASSERT_HPP_

#include <stdio.h>
#include "helper/conf.hpp"
#include "helper/def.hpp"




#define PRINT_ERROR(...) fprintf(stderr, __VA_ARGS__)
#define DIEDIEDIE() do{ \
    __disable_irq(); \
    NVIC_SystemReset(); \
    while(1); \
}while(false)

#ifndef NDEBUG
#define assert(x) if(!(x)){ \
    PRINT_ERROR( \
        "\r\n--ASSERTION FAILURE--\r\nFile: %s\r\nLine: %d\r\nCond: %s\r\n", \
        __FILE__, __LINE__, #x \
    ); \
    DIEDIEDIE(); \
}
#else //NDEBUG
#define assert(x) if(!(x)) DIEDIEDIE();
#endif//NDEBUG




#endif /* _HELPER_ASSERT_HPP_ */
