//retarget/retarget
#include "retarget_impl.hpp"
#include "helper/helper.hpp"
#include "retarget_conf.hpp"




extern "C" int fputc_impl(int ch, void* impl){
    if(impl){
        return static_cast<RetargetImpl*>(impl)->fputc(ch);
    }
    return EOF;
}

extern "C" int fflush_impl(void* impl){
    if(impl){
        return static_cast<RetargetImpl*>(impl)->fflush();
    }
    return EOF;
}

extern "C" int fgetc_impl(void* impl){
    if(impl){
        return static_cast<RetargetImpl*>(impl)->fgetc();
    }
    return EOF;
}

extern "C" void __backspace_impl(void* impl){
    if(impl){
        return static_cast<RetargetImpl*>(impl)->backspace();
    }
}

extern "C" int fclose_impl(void* impl){
    if(impl){
        //
    }
    return 0;
}
