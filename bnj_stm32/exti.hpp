//exti.hpp: External Interrupt
#ifndef _EXTI_HPP_
#define _EXTI_HPP_
#pragma thumb

#include "stm32f10x.h"
#include "common.hpp"
#include "register.hpp"
#include "bus.hpp"
#include "gpio.hpp"

#include <cstddef>

#undef EXTI
template <U32 Line>
struct EXTI : APB2<1>{
    STATIC_ASSERT(0<=Line && Line<=19);
#define IF_Irange(a, b, expr) (a<=Line && Line<=b)?(expr)
    enum{
        line_n  =   Line,
    };
    enum addr{
        base        =   APB2<1>::base,
        //
        IMR         =   base+0x00,      //Interrupt Mask(Enable) Reg.
        EMR         =   base+0x04,      //Event Mask(Enable) Reg.
        RTSR        =   base+0x08,      //Rising Trigger Selection Reg.
        FTSR        =   base+0x0C,      //Falling Trigger Selection Reg.
        SWIER       =   base+0x10,      //Software Interrupt Event Reg.
        PR          =   base+0x14,      //Pending Reg.
        //special: AFIO_EXTICRx for EXTI lines 0-15
        AFIO_CR     =   IF_Irange(0, 3,     AFIO::EXTICR+0x00):
                        IF_Irange(4, 7,     AFIO::EXTICR+0x04):
                        IF_Irange(8, 11,    AFIO::EXTICR+0x08):
                        IF_Irange(12, 15,   AFIO::EXTICR+0x0C):0,
        AFIO_CR_idx =   (line_n&0x03)<<2,
    };

    static Reg_Norm(IMR, line_n, 1) enabled;
	static void enable(){enabled=true;}
    static void disable(){enabled=false;}

    static Reg_Norm(EMR, line_n, 1) event_enabled;
	static void event_enable(){event_enabled=true;}
    static void event_disable(){event_enabled=false;}

    //prepare to listen to the corresponding pin of the given GPIO port
    //Usage: EXTI<Pin>::prepare_GPIO<Port>(in_type);
    //NOTE: Line == Pin#
#define X GPIO_Pin<Port, Line>
    template <char Port>
    static void prepare_GPIO(typename GPIO::in_drive_t in_type=GPIO::in_floating){
        STATIC_ASSERT(0<=Line && Line<=15);
        static Reg_Norm(AFIO_CR, AFIO_CR_idx, 4) AFIO_port;
        X::conf(X::in, in_type);
        AFIO_port=X::port_n;
    }
#undef X

    //set line to be sensitive to rising and/or falling edge
    static Reg_Norm(RTSR, line_n, 1) rising;
    static Reg_Norm(FTSR, line_n, 1) falling;
    //static VU32& rising(){return VU32_AT(RTSR_bit);}
    //static VU32& falling(){return VU32_AT(FTSR_bit);}

    //pending status control
    static Reg(Reg_RC_W1, Reg_Bitfield, PR, line_n, 1) pending;
	static void dismiss(){pending.clear();}
#undef IF_I
#undef IF_P
#undef IF_Irange
};

#endif//_EXTI_HPP_
