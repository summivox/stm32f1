//dma.hpp: Direct Memory Access
#ifndef _DMA_HPP_
#define _DMA_HPP_
#pragma thumb

#include "stm32f10x.h"
#include "common.hpp"
#include "register.hpp"
#include "bus.hpp"

//Dummy base class for enums. Refer to gpio.hpp
struct DMA{
    enum dir_t{
        read_P_write_M = 0, //peripheral -> memory
        read_M_write_P = 1, //memory -> peripheral
    };
    enum size{
        byte = 0, 
        halfword = 1,
        word = 2,
    };
};
template <typename Base, U32 Ch>
struct DMA_Impl : DMA, Base{
    enum addr{
        base        =   Base::base,
        //
        ISR         =   base+0x00,
        IFCR        =   base+0x00,
        CCR         =   base+0x08+20*(Ch-1),
        NDT         =   base+0x0C+20*(Ch-1),
        PA          =   base+0x10+20*(Ch-1),
        MA          =   base+0x14+20*(Ch-1),
    };
    //interrupt flag
    static Reg(Reg_RO, Reg_Bit, ISR, 4*(Ch-1)+0) G_if;
    static Reg(Reg_RO, Reg_Bit, ISR, 4*(Ch-1)+1) TC_if;
    static Reg(Reg_RO, Reg_Bit, ISR, 4*(Ch-1)+2) HT_if;
    static Reg(Reg_RO, Reg_Bit, ISR, 4*(Ch-1)+2) TE_if;

    //interrupt clear
    static Reg(Reg_RC_W1, Reg_Bit, IFCR, 4*(Ch-1)+0) G_ic;
    static Reg(Reg_RC_W1, Reg_Bit, IFCR, 4*(Ch-1)+1) TC_ic;
    static Reg(Reg_RC_W1, Reg_Bit, IFCR, 4*(Ch-1)+2) HT_ic;
    static Reg(Reg_RC_W1, Reg_Bit, IFCR, 4*(Ch-1)+2) TE_ic;

    //CCR
    static Reg(Reg_RW, Reg_Bitfield, CCR, 0, 16) CCR_Reg;
    static Reg(Reg_RW, Reg_Bit, CCR, 0) ch_enabled;
    static Reg(Reg_RW, Reg_Bit, CCR, 1) TC_ie;
    static Reg(Reg_RW, Reg_Bit, CCR, 2) HT_ie;
    static Reg(Reg_RW, Reg_Bit, CCR, 3) TE_ie;
    static Reg(Reg_RW, Reg_Bit, CCR, 4) dir; //enum dir_t
    static Reg(Reg_RW, Reg_Bit, CCR, 5) circular;
    static Reg(Reg_RW, Reg_Bit, CCR, 6) P_inc;
    static Reg(Reg_RW, Reg_Bit, CCR, 7) M_inc;
    static Reg(Reg_RW, Reg_Bitfield, CCR, 8, 2) P_size; //enum size
    static Reg(Reg_RW, Reg_Bitfield, CCR, 10, 2) M_size;
    static Reg(Reg_RW, Reg_Bitfield, CCR, 12, 2) priority; //larger value -> higher priority
    static Reg(Reg_RW, Reg_Bit, CCR, 14) M2M; //memory -> memory


    //NDT, PA, MA
    static Reg(Reg_RW, Reg_Bitfield, NDT, 0, 32/*16*/) data_n; //workaround for a chip bug
    static Reg(Reg_RW, Reg_Bitfield, PA, 0, 32) P_addr;
    static Reg(Reg_RW, Reg_Bitfield, MA, 0, 32) M_addr;

    static void reset(){
        G_ic.clear();
        VU32_AT(CCR)=0; //also workaround (I think)
        VU32_AT(NDT)=0;
        VU32_AT(PA)=0;
        VU32_AT(MA)=0;
    }
    static void ch_enable(){ch_enabled=true;}
    static void ch_disable(){ch_enabled=false;}
    static void wait_TC(){while(!TC_if);}
};

#ifdef RCC_AHBENR_DMA1EN
#undef DMA1
template <U32 Ch>
struct DMA1 : DMA, DMA_Impl<AHB<DMA1_BASE, 0>, Ch>{
    STATIC_ASSERT(1<=Ch && Ch<=7);
};
#endif
#ifdef RCC_AHBENR_DMA2EN
#undef DMA2
template <U32 Ch>
struct DMA2 : DMA, DMA_Impl<AHB<DMA2_BASE, 1>, Ch>{
    STATIC_ASSERT(1<=Ch && Ch<=5);
};
#endif

#endif//_DMA_HPP_
