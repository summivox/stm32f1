//retarget/ST/stm32f10x/usart
#ifndef _RETARGET_ST_STM32F10X_USART_HPP_
#define _RETARGET_ST_STM32F10X_USART_HPP_

#include "helper/ST/stm32f10x.hpp"




enum wordlen_t{
    WORDLEN_8=0,
    WORDLEN_9=1,
};

enum stop_t{
    STOP_1=0,
    STOP_0_5=1,
    STOP_2=2,
    STOP_1_5=3,
};




struct UsartImplBase{
    USART_TypeDef* USART;
    explicit UsartImplBase(USART_TypeDef* USART):USART(USART){
    }
};

//Unbuffered, blocking TX implementation
struct UsartTxImpl : virtual UsartImplBase{
    explicit UsartTxImpl(USART_TypeDef* USART):UsartImplBase(USART){
        PERIPH_SBIT(USART, USART, CR1, UE)=1;
        PERIPH_SBIT(USART, USART, CR1, TE)=1;
    }
    virtual ~UsartTxImpl(){
        PERIPH_SBIT(USART, USART, CR1, TE)=0;
    }

    virtual int fputc(int ch){
        while(!PERIPH_SBIT(USART, USART, SR, TXE));
        return USART->DR=(U8)ch;
    }

    virtual int fflush(){
        while(!PERIPH_SBIT(USART, USART, SR, TC));
        return 0;
    }
};

//Unbuffered, blocking RX implementation with 1-byte backspace support
struct UsartRxImpl : virtual UsartImplBase{
    bool backspace_flag;
    U8 last;

    explicit UsartRxImpl(USART_TypeDef* USART):UsartImplBase(USART){
        PERIPH_SBIT(USART, USART, CR1, UE)=1;
        PERIPH_SBIT(USART, USART, CR1, RE)=1;
        backspace_flag=0;
        last=0;
    }
    virtual ~UsartRxImpl(){
        PERIPH_SBIT(USART, USART, CR1, RE)=0;
    }

    virtual int fgetc(){
        if(backspace_flag){
            backspace_flag=false;
            return last;
        }
        while(!PERIPH_SBIT(USART, USART, SR, RXNE));
        return last=USART->DR;
    }
    virtual int backspace(){
        backspace_flag=true;
        return 0;
    }
};




#endif /* _RETARGET_ST_STM32F10X_USART_HPP_ */
