//retarget/retarget
#include "retarget_impl.hpp"
#include "helper/helper.hpp"
#include "retarget_conf.hpp"




extern "C" int fputc_impl(int ch, void* impl){
    RetargetWriteImpl* p=dynamic_cast<RetargetWriteImpl*>(impl);
    if(p){
        return p->fputc(ch);
    }
    return EOF;
}

extern "C" int fflush_impl(void* impl){
    RetargetWriteImpl* p=dynamic_cast<RetargetWriteImpl*>(impl);
    if(p){
        return p->fflush();
    }
    return EOF;
}

extern "C" int fgetc_impl(void* impl){
    RetargetReadImpl* p=dynamic_cast<RetargetReadImpl*>(impl);
    if(p){
        return p->fgetc();
    }
    return EOF;
}

extern "C" int __backspace_impl(void* impl){
    RetargetReadImpl* p=dynamic_cast<RetargetReadImpl*>(impl);
    if(p){
        return p->backspace();
    }
	return 0;
}

extern "C" int fclose_impl(void* impl){
    if(impl){
        //
    }
    return 0;
}
