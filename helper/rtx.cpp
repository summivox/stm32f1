//helper/rtx
#include "helper/rtx.hpp"




//sleep for given duration, blocking
void wait_ms(U32 Tms){
    U32 Tms_before=os_time_get();
    while(I32(os_time_get()-Tms_before)<Tms);
}
