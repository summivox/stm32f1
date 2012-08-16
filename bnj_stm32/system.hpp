//system.hpp: Cortex-M3 system controls and core peripherals
#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_
#pragma thumb

#include "stm32f10x.h"
#include "common.hpp"
#include "register.hpp"

//User IRQ control
#undef NVIC
#undef IRQ
template <IRQn N>
struct IRQ{
    STATIC_ASSERT(N>=0);
    //template argument alias
    enum{
            irq_n       =   N,
            mask_n      =   irq_n&31,
            mask        =   1<<mask_n,
            priority_n  = __NVIC_PRIO_BITS,
    };
    enum addr{
        base        =   NVIC_BASE,
        //
        ISER        =   base+0x000, //Interrupt Set Enable Register
        ICER        =   base+0x080, //Interrupt Clear Enable Register
        ISPR        =   base+0x100, //Interrupt Set Pending Register
        ICPR        =   base+0x180, //Interrupt Clear Pending Register
        IABR        =   base+0x200, //Interrupt Active bit Register
        IP          =   base+0x300, //Interrupt Priority Register (8bit wide)
        STIR        =   base+0xE00, //Software Trigger Interrupt Register
        //real address
        //NOTE: NVIC is not in the bitband region so use bitmask instead.
        ISER_n      =   ISER+((irq_n>>5)<<2),
        ICER_n      =   ICER+((irq_n>>5)<<2),
        ISPR_n      =   ISPR+((irq_n>>5)<<2),
        ICPR_n      =   ICPR+((irq_n>>5)<<2),
        IABR_n      =   IABR+((irq_n>>5)<<2),
        IP_n        =   IABR+irq_n, //NOTE: Need no mask--use VU8_AT instead.
    };

    //static Reg_Comb(bool,
    //    /*read*/    Reg_Bit<ISER_n, mask_n>,
    //    /*write*/   Reg_Default,
    //    /*set*/     Reg_Bit<ISER_n, mask_n>,
    //    /*clear*/   Reg(Reg_RC_W1, Reg_Bit, ICER_n, mask_n),
    //    /*toggle*/  Reg_Default) enabled;
    //static void enable(){enabled=true;}
    //static void disable(){enabled=false;}

    //static Reg_Comb(bool,
    //    /*read*/    Reg_Bit<ISPR_n, mask_n>,
    //    /*write*/   Reg_Default,
    //    /*set*/     Reg_Bit<ISPR_n, mask_n>,
    //    /*clear*/   Reg(Reg_RC_W1, Reg_Bit, ICPR_n, mask_n),
    //    /*toggle*/  Reg_Default) pending;
    //static void trigger(){pending=true;}
    //static void dismiss(){pending=false;}

    //2012-01-29: Reverted to plain old implementation due to odd ICER/ICPR read mechanics
    static bool enabled(){return VU32_AT(ISER_n);}
    static void enable(){VU32_AT(ISER_n)=mask;}
    static void disable(){VU32_AT(ICER_n)=mask;}

    static bool pending(){return VU32_AT(ISPR_n);}
    static void trigger(){VU32_AT(ISPR_n)=mask;}
    static void dismiss(){VU32_AT(ICPR_n)=mask;}

    static Reg(Reg_RO, Reg_Bit, IABR_n, mask_n) active;
    static Reg_Norm(IP_n, 0, 8) priority;

#undef IF_USER
};

//System Tick Timer
#undef SysTick
struct SysTick{
    enum addr{
        base        =   SysTick_BASE,
        CTRL        =   base+0x00,
        LOAD        =   base+0x04,
        VAL         =   base+0x08,
        CALIB       =   base+0x0c,
    };
    static Reg_Norm(CTRL, 0, 1) enabled;
    static void enable(){enabled=true;}
    static void disable(){enabled=false;}

    static Reg_Norm(CTRL, 1, 1) irq_enabled;
    static void irq_enable(){irq_enabled=true;}
    static void irq_disable(){irq_enabled=false;}

    static Reg_Norm(CTRL, 2, 1) clock_src;
    enum clock_src_t{
        external=false,
        internal=true,
    };

    static Reg(Reg_RC_R, Reg_Bit, CTRL, 16) flag;

    static Reg_Norm(LOAD, 0, 24) reload;
    static Reg_Norm(VAL,  0, 24) value;
    static Reg_Norm(CALIB, 0, 24) ten_ms;
    static Reg(Reg_RO, Reg_Bit, CALIB, 30) ten_ms_skew;
    static Reg(Reg_RO, Reg_Bit, CALIB, 31) no_reference;

    static void autoconf(U32 x){
        if(no_reference) clock_src=internal;
        reload=x;
        value=0;
        irq_enable();
        enable();
    }
    static void autoconf(){autoconf(ten_ms);}
};

#endif//_SYSTEM_HPP_
