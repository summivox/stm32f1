//bus.hpp: base class for APB1/APB2/AHB bus peripherals
#ifndef _BUS_HPP_
#define _BUS_HPP_
#pragma thumb

#include "stm32f10x.h"
#include "common.hpp"
#include "register.hpp"
#include "rcc.hpp"

template <U32 N>
struct APB1{
    enum{
        base=APB1PERIPH_BASE + 0x0400*N,
    };
    static Reg_Norm(RCC::APB1ENR, N, 1) enabled;
    static void enable(){enabled=true;}
    static void disable(){enabled=false;}
    static void reset(){
        static Reg(Reg_WO, Reg_Bitfield, RCC::APB1RSTR, N, 1) rst;
        rst=1; while(!rst.read()); rst=0; while(rst.read());
    }
	static U32 get_clock_freq(){
        if(!RCC::freq.ready)
            RCC::freq.update();
        return RCC::freq.PCLK1;
    }
};
template <U32 N>
struct APB2{
    enum{
        base=APB2PERIPH_BASE + 0x0400*N,
    };
    static Reg_Norm(RCC::APB2ENR, N, 1) enabled;
    static void enable(){enabled=true;}
    static void disable(){enabled=false;}
    static void reset(){
        static Reg(Reg_WO, Reg_Bitfield, RCC::APB2RSTR, N, 1) rst;
        rst=1; while(!rst); rst=0; while(rst);
    }
	static U32 get_clock_freq(){
        if(!RCC::freq.ready)
            RCC::freq.update();
        return RCC::freq.PCLK2;
    }
};
template <U32 BASE, U32 N>
struct AHB{
    enum{
        base=BASE,
    };
    static Reg_Norm(RCC::AHBENR, N, 1) enabled;
    static void enable(){enabled=true;}
    static void disable(){enabled=false;}
	static U32 get_clock_freq(){
        if(!RCC::freq.ready)
            RCC::freq.update();
        return RCC::freq.HCLK;
    }
};

#endif//_BUS_HPP_
