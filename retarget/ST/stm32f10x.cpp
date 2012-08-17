//retarget/ST/stm32f10x.cpp

#include <stdio.h>
#include "retarget/retarget.h"
#include "stm32f10x/usart.hpp"
#include "helper/ST/stm32f10x.hpp"

#include "stm32f10x/usart1.hpp"




extern "C" void rt_entry_impl(){
    FREQ.update();
#if     USART1_EN==1
    USART1_init();
#endif//USART1_EN
};

extern "C" int fputc_impl(int ch, __FILE* f){
    if(f->handle==-1){
        if((U32)(f->data)==0){
#if USART1_TX_BUF_EN==1
            USART1_TX_buf(ch);
#else//USART1_TX_BUF_EN
			USART1_TX_unbuf(ch);
#endif//USART1_TX_BUF_EN
			return ch;
        }else{
            USART1_TX_unbuf(ch);
			return ch;
        }
    }
    return EOF;
}

extern "C" int fflush_impl(__FILE* f){
    if(f->handle==-1){
        if((U32)(f->data)==0){
#if USART1_TX_BUF_EN==1
            USART1_flush_buf();
#else//USART1_TX_BUF_EN
			USART1_flush_unbuf();
#endif//USART1_TX_BUF_EN
        }else{
            USART1_flush_unbuf();
        }
    }
    return 0;
}

extern "C" int fgetc_impl(__FILE* f){
    if(f->handle==-1){
        if((U32)(f->data)==1){
            return USART1_RX();
        }
    }
    return EOF;
}

extern "C" int backspace_impl(__FILE* f){
    if(f->handle==-1){
        if((U32)(f->data)==1){
            USART1_BS();
        }
    }
    return 0;
}

extern "C" int fclose_impl(__FILE* f){
    return 0;
}
