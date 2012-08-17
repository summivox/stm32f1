//retarget/ST/stm32f10x/usart1
#ifndef _RETARGET_ST_STM32F10X_USART1_
#define _RETARGET_ST_STM32F10X_USART1_

#include "helper/ST/stm32f10x.hpp"
#include <algorithm>




// <<< Use Configuration Wizard in Context Menu >>>

//  <e> USART1
#   define USART1_EN 1
#   if     USART1_EN==1
//      <o> Pin Mapping
//          <0=> 0: TX=PA9 ; RX=PA10;
//          <1=> 1: TX=PB6 ; RX=PB7 ;
#       define USART1_REMAP 0

//      <h> Protocol
//          <o> Baud Rate <0-72000000>
#           define USART1_BAUD 115200

//          <o> Word length <0=>8 bits(standard) <1=>9 bits
#           define USART1_M 0

//          <e> Parity bit(standard:no parity)
#           define USART1_PARITY 0
#           if     USART1_PARITY==1
//              <o> Parity type <0=>Even <1=>Odd
#               define USART1_PARITY_TYPE 1

#           endif//USART1_PARITY
//          </e>
//          <o> Stop bit <0=>1(standard) <1=>0.5 <2=>2 <3=>1.5
#           define USART1_STOP 0

//      </h>
//      <h> Behavior
//          <e> TX Enabled
#           define USART1_TX_EN 1
#           if     USART1_TX_EN==1
//              <e> TX (Double) Buffer Enabled
//                  <i> Unbuffered TX remains available even when TX buffer is enabled.
#               define USART1_TX_BUF_EN 1
#               if     USART1_TX_BUF_EN==1
//                  <o> Buffer Type
//                      <0=> Fully Buffered(auto flush on full)
//                      <1=> Line Buffered(auto flush on full or LF(\n))
#                   define USART1_TX_BUF_LF 1

//                  <o> Buffer Size
#                   define USART1_TX_BUF_SIZE 32

#               endif//USART1_TX_BUF_EN
//              </e>
#           endif//USART1_TX_EN
//          </e>
//          <e> RX Enabled
#           define USART1_RX_EN 1
#           if     USART1_RX_EN==1
//              <e> RX (Circular) Buffer Enabled
//                  <i> Unbuffered RX is NOT available when RX buffer is enabled.
#               define USART1_RX_BUF_EN 1
#               if     USART1_RX_BUF_EN==1
//                  <o> Buffer Size
#                   define USART1_RX_BUF_SIZE 128

#               endif//USART1_RX_BUF_EN
//              </e>
//              <e> Echo received characters
#               define USART1_RX_ECHO 1
#               if     USART1_RX_ECHO==1
//                  <q> Echo CR(\r) as CRLF(\r\n)
#                   define USART1_RX_ECHO_CRLF 1

#               endif//USART1_RX_ECHO
//				</e>
#           endif//USART1_RX_EN
//          </e>
//      </h>
#   endif//USART1_EN
//  </e>


// <<< end of configuration section >>>




////Select correct peripherals based on configuration

#if     USART1_EN==1

#   if     USART1_REMAP==0
#       if     USART1_TX_EN==1
#           define USART1_TX_PORT PA
#           define USART1_TX_PIN PA9
#       endif//USART1_TX_EN
#       if     USART1_RX_EN==1
#           define USART1_RX_PORT PA
#           define USART1_RX_PIN PA10
#       endif//USART1_RX_EN
#   elif   USART1_REMAP==1
#       if     USART1_TX_EN==1
#           define USART1_TX_PORT PB
#           define USART1_TX_PIN PB6
#       endif//USART1_TX_EN
#       if     USART1_RX_EN==1
#           define USART1_RX_PORT PB
#           define USART1_RX_PIN PB7
#       endif//USART1_RX_EN
#   endif//USART1_REMAP

#   if     USART1_TX_EN==1 && USART1_TX_BUF_EN==1
#       define USART1_TX_DMA DMA1
#       define USART1_TX_DMA_CH DMA1_Channel4
#       define USART1_TX_DMA_GIC GIC4
#   endif//USART1_TX_EN
#   if     USART1_RX_EN==1 && USART1_RX_BUF_EN==1
#       define USART1_RX_DMA DMA1
#       define USART1_RX_DMA_CH DMA1_Channel5
#       define USART1_RX_DMA_CH_N 5
#   endif//USART1_RX_EN

#endif//USART1_EN




////Implementation

#if     USART1_EN==1




//Buffers(if applicable)
#if USART1_TX_EN==1 && USART1_TX_BUF_EN==1
    static U8 USART1_TX_pool[2][USART1_TX_BUF_SIZE];
    static U8 *USART1_TX_pbegin, *USART1_TX_pnext, *USART1_TX_pcurr, *USART1_TX_pend;
#endif//USART1_TX_EN

#if USART1_RX_EN==1 && USART1_RX_BUF_EN==1
    static U8 USART1_RX_pool[USART1_RX_BUF_SIZE];
    static U32 USART1_RX_head;
#endif

//Initialization
void USART1_init(){
    //Pin mapping
    RCC_ENR(APB2, AFIOEN)=1;
    PERIPH_SBIT(AFIO, AFIO, MAPR, USART1_REMAP)=USART1_REMAP;

    //Enable and configure USART
    RCC_ENR(APB2, USART1EN)=1;
    RCC_RSTR(APB2, USART1RST)=1;
    RCC_RSTR(APB2, USART1RST)=0;

    USART1->BRR=FREQ.PCLK2/USART1_BAUD;
    PERIPH_SBIT(USART, USART1, CR1, M)=USART1_M;
#if USART1_PARITY==1
    PERIPH_SBIT(USART, USART1, CR1, PCE)=USART1_PARITY;
    PERIPH_SBIT(USART, USART1, CR1, PS)=USART1_PARITY_TYPE;
#endif//USART1_PARITY
    USART1->CR2=(USART1->CR2&(~USART_CR2_STOP))|(USART_CR2_STOP*USART1_STOP);
    PERIPH_SBIT(USART, USART1, CR1, UE)=1;

#if USART1_TX_EN==1
    PERIPH_SBIT(USART, USART1, CR1, TE)=1;
    USART1_TX_PORT::enable();
    USART1_TX_PIN.conf(GPIO_AF_PP);

    while(!PERIPH_SBIT(USART, USART1, SR, TC));

#   if USART1_TX_BUF_EN==1
    PERIPH_SBIT(USART, USART1, CR3, DMAT)=1;
    RCC_ENR(AHB, DMA1EN)=1;
    USART1_TX_DMA_CH->CPAR=(U32)&(USART1->DR);
    USART1_TX_DMA_CH->CCR=DMA_CCR1_MINC|DMA_CCR1_DIR;

    USART1_TX_pbegin=USART1_TX_pcurr=USART1_TX_pool[0];
    USART1_TX_pnext=USART1_TX_pool[1];
    USART1_TX_pend=USART1_TX_pcurr+USART1_TX_BUF_SIZE;
#   endif//USART1_TX_BUF_EN

#endif//USART1_TX_EN

#if USART1_RX_EN==1
    PERIPH_SBIT(USART, USART1, CR1, RE)=1;
    USART1_RX_PORT::enable();
    USART1_RX_PIN.conf(GPIO_IN_FLOATING);

#   if USART1_RX_BUF_EN==1
    PERIPH_SBIT(USART, USART1, CR3, DMAR)=1;
    RCC_ENR(AHB, DMA1EN)=1;
    USART1_RX_DMA_CH->CPAR=(U32)&(USART1->DR);
	USART1_RX_DMA_CH->CMAR=(U32)USART1_RX_pool;
    USART1_RX_DMA_CH->CNDTR=USART1_RX_BUF_SIZE;
    USART1_RX_DMA_CH->CCR=DMA_CCR1_MINC|DMA_CCR1_CIRC|DMA_CCR1_EN;
#   endif//USART1_RX_BUF_EN

#endif//USART1_RX_EN
}

#if USART1_TX_EN
void USART1_flush_unbuf(){
    while(!PERIPH_SBIT(USART, USART1, SR, TC));
}
void USART1_TX_unbuf(U8 ch){
    while(!PERIPH_SBIT(USART, USART1, SR, TXE));
    USART1->DR=ch;
}
#   if USART1_TX_BUF_EN
void USART1_flush_buf(){
    USART1_flush_unbuf();

    if(USART1_TX_pcurr==USART1_TX_pbegin){
        return;
    }

    USART1_TX_DMA_CH->CCR&=~DMA_CCR1_EN;
    PERIPH_SBIT(DMA, USART1_TX_DMA, IFCR, CGIF4)=1; //TODO: more generic!(duh)

    USART1_TX_DMA_CH->CMAR=(U32)USART1_TX_pbegin;
    USART1_TX_DMA_CH->CNDTR=USART1_TX_pcurr-USART1_TX_pbegin;
    USART1_TX_DMA_CH->CCR|=DMA_CCR1_EN;

    std::swap(USART1_TX_pbegin, USART1_TX_pnext);
    USART1_TX_pcurr=USART1_TX_pbegin;
    USART1_TX_pend=USART1_TX_pbegin+USART1_TX_BUF_SIZE;
}
void USART1_TX_buf(U8 ch){
    *USART1_TX_pcurr++=ch;
#       if USART1_TX_BUF_LF==1
            if(ch=='\n' || USART1_TX_pcurr==USART1_TX_pend){
#       else//USART1_TX_BUF_LF
            if(USART1_TX_pcurr==USART1_TX_pend){
#       endif//USART1_TX_BUF_LF
                USART1_flush_buf();
            }
}
#   endif//USART1_TX_BUF_EN
#endif//USART1_TX_EN

#if USART1_RX_EN


#   if USART1_RX_ECHO==1 && USART1_TX_EN==1
void USART1_ECHO(U8 ch){
#       if USART1_TX_BUF_EN
            USART1_TX_buf(ch);
#       else//USART1_TX_BUF_EN
            USART1_TX_unbuf(ch);
#       endif//USART1_TX_BUF_EN

#       if USART1_RX_ECHO_CRLF
            if(ch=='\r'){
#               if USART1_TX_BUF_EN
                    USART1_TX_buf('\n');
#               else//USART1_TX_BUF_EN
                    USART1_TX_unbuf('\n');
#               endif//USART1_TX_BUF_EN
            }
#       endif//USART1_RX_ECHO_CRLF

#       if USART1_TX_BUF_EN
             USART1_flush_buf();
#       else//USART1_TX_BUF_EN
            USART1_flush_unbuf();
#       endif//USART1_TX_BUF_EN
}
#   endif//USART1_RX_ECHO


#   if USART1_RX_BUF_EN==1

static U8 USART1_RX_BS_count=0;
U8 USART1_RX(){
    while(USART1_RX_head==(USART1_RX_BUF_SIZE-USART1_RX_DMA_CH->CNDTR)){
        //idle
    }
    U8 ret=USART1_RX_pool[USART1_RX_head];
    if(++USART1_RX_head==USART1_RX_BUF_SIZE){
        USART1_RX_head=0;
    }
    if(USART1_RX_BS_count){
        --USART1_RX_BS_count;
    }else{
#       if USART1_RX_ECHO==1
            USART1_ECHO(ret);
#       endif//USART1_RX_ECHO
    }
    return ret;
}
void USART1_BS(){
    if(USART1_RX_head--==0){
        USART1_RX_head=USART1_RX_BUF_SIZE-1;
    }
    ++USART1_RX_BS_count;
}

#   else//USART1_RX_BUF_EN

static bool USART1_RX_BS_flag;
static U8 USART1_RX_last;
U8 USART1_RX(){
    if(USART1_RX_BS_flag){
        USART1_RX_BS_flag=false;
        return USART1_RX_last;
    }
    while(!PERIPH_SBIT(USART, USART1, SR, RXNE));
    U8 ret=USART1_RX_last=USART1->DR;
#       if USART1_RX_ECHO==1
            USART1_ECHO(ret);
#       endif//USART1_RX_ECHO
    return ret;
}
void USART1_BS(){
    USART1_RX_BS_flag=true;
}

#   endif//USART1_RX_BUF_EN


#endif//USART1_RX_EN




#endif//USART1_EN




#endif /*_RETARGET_ST_STM32F10X_USART1_*/
