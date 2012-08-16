//helper/irq
//Prerequisite: none
#ifndef _HELPER_IRQ_HPP_
#define _HELPER_IRQ_HPP_

#include "helper/conf.hpp"
#include "helper/def.hpp"
#include "helper/armcc.hpp"




//Shortcut for declaring ISR and specifying interrupt priority
//Place in source file once only--ISRs should NEVER be called from thread code
//Usage: IRQ_DECL(TIM2_IRQn, TIM2_IRQHandler, 2, 0) IN_RAM;
void IRQ_priority_init(IRQn_Type IRQn, U8 P1, U8 P2);
#define IRQ_DECL(IRQn, IRQHandler, P1, P2) \
    void IRQn##_priority_init() __attribute__((weak, constructor)); \
    void IRQn##_priority_init(){ \
        IRQ_priority_init(IRQn, P1, P2); \
    } \
    extern "C" void IRQHandler()




#endif /* _HELPER_IRQ_HPP_ */
