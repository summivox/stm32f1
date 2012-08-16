//usart.hpp: Universal Synchronous/Asynchronous Receiver Transmitter
#ifndef _USART_HPP_
#define _USART_HPP_
#pragma thumb

#include "stm32f10x.h"
#include "common.hpp"
#include "register.hpp"
#include "bus.hpp"
#include "gpio.hpp"

//Dummy base class for enums. Refer to gpio.hpp
#undef USART
struct USART{
    enum parity_type_t{
        even=0,
        odd=1,
    };
    enum wakeup_type_t{
        wakeup_idle=0,
        wakeup_addr=1,
    };
    enum word_length_t{
        word_8=0,
        word_9=1,
    };
    enum stop_bit_t{
        stop_one=0,
        stop_half=1,
        stop_two=2,
        stop_onehalf=3,
    };
};

template <typename Base>
struct USART_Impl : USART, Base{
    enum addr{
        base        =   Base::base,
        //
        SR          =   base+0x00,  //Status Reg.(9bit)
        DR          =   base+0x04,  //Data Reg.(9bit)
        BRR         =   base+0x08,  //Baud Rate Reg.(16bit = 12bit mantissa + 4bit fraction)
        CR1         =   base+0x0C,  //Control Reg. 1(14bit)
        CR2         =   base+0x10,  //Control Reg. 2(15bit)
        CR3         =   base+0x14,  //Control Reg. 3(11bit)
        PSC         =   base+0x18,  //Prescaler Reg.(8bit)
        GT          =   base+0x19,  //Guard Time Reg.(8bit)
    };

    //DR
    static Reg(Reg_RW, Reg_Bitfield, DR, 0, 8) DR_Reg;

    //SR
    static Reg(Reg_RO, Reg_Bitfield, SR, 0, 16) SR_Reg;
    static Reg(Reg_RO, Reg_Bit, SR, 0) PE;
    static Reg(Reg_RO, Reg_Bit, SR, 1) FE;
    static Reg(Reg_RO, Reg_Bit, SR, 2) NE;
    static Reg(Reg_RO, Reg_Bit, SR, 3) ORE;
    static Reg(Reg_RO, Reg_Bit, SR, 4) IDLE;
    static Reg(Reg_RC_W0, Reg_Bit, SR, 5) RXNE;
    static Reg(Reg_RC_W0, Reg_Bit, SR, 6) TC;
    static Reg(Reg_RO, Reg_Bit, SR, 7) TXE;
    static Reg(Reg_RC_W0, Reg_Bit, SR, 8) LBD;
    static Reg(Reg_RC_W0, Reg_Bit, SR, 9) CTS;

    static void clear_error(){
        SR_Reg.read();
        DR_Reg.read();
    }
    static void TX(char c){
        while(!TXE);
        DR_Reg=c;
    }
    static char RX(){
        while(!RXNE);
        return DR_Reg;
    }
    static void wait_TC(){
        while(!TC);
    }

    //BRR: baud rate calculator included
    static Reg(Reg_RW, Reg_Bitfield, BRR, 0, 16) BRR_Reg;
    struct baud_rate_t{
#define reg USART_Impl<Base>::BRR_Reg
#define round_div(a, b) ( ( (a) + ( (b)>>1 ) ) / (b) )
        U32 operator=(U32 baud){
            reg=round_div(Base::get_clock_freq(), baud); //round to nearest integer
			return baud;
        }
        operator U16(){
            U16 r=reg.read();
            return round_div(Base::get_clock_freq(), r);
        }
#undef reg
#undef round_div
    };
    static baud_rate_t baud_rate;

    //CR1
    static Reg(Reg_RW, Reg_Bitfield, CR1, 0, 16) CR1_Reg;
    static Reg(Reg_RW, Reg_Bit, CR1,  0) send_break;
    static Reg(Reg_RW, Reg_Bit, CR1,  1) RX_muted;
    static Reg(Reg_RW, Reg_Bit, CR1,  2) RX_enabled;
    static Reg(Reg_RW, Reg_Bit, CR1,  3) TX_enabled;
    static Reg(Reg_RW, Reg_Bit, CR1,  4) IDLE_ie; //interrupt enabled
    static Reg(Reg_RW, Reg_Bit, CR1,  5) RXNE_ie;
    static Reg(Reg_RW, Reg_Bit, CR1,  6) TC_ie;
    static Reg(Reg_RW, Reg_Bit, CR1,  7) TXE_ie;
    static Reg(Reg_RW, Reg_Bit, CR1,  8) PE_ie;
    static Reg(Reg_RW, Reg_Bit, CR1,  9) parity_type; //enum parity_type_t;
    static Reg(Reg_RW, Reg_Bit, CR1, 10) parity_enabled;
    static Reg(Reg_RW, Reg_Bit, CR1, 11) wakeup_type; //enum wakeup_type_t;
    static Reg(Reg_RW, Reg_Bit, CR1, 12) word_length; //enum word_length_t;
    static Reg(Reg_RW, Reg_Bit, CR1, 13) UE; //this is weird--refer to USART_CR1 in RM0008

    //CR2
    static Reg(Reg_RW, Reg_Bitfield, CR2, 0, 16) CR2_Reg;
    static Reg(Reg_RW, Reg_Bitfield, CR2, 0, 4) node_addr;
    static Reg(Reg_RW, Reg_Bit, CR2, 5) LBDL;
    static Reg(Reg_RW, Reg_Bit, CR2, 6) LBD_ie;
    static Reg(Reg_RW, Reg_Bit, CR2, 8) clock_last_bit;
    static Reg(Reg_RW, Reg_Bit, CR2, 9) clock_phase;
    static Reg(Reg_RW, Reg_Bit, CR2, 10) clock_polarity;
    static Reg(Reg_RW, Reg_Bit, CR2, 11) clock_enabled;
    static Reg(Reg_RW, Reg_Bitfield, CR2, 12, 2) stop_bit; //enum stop_bit_t;
    static Reg(Reg_RW, Reg_Bit, CR2, 14) LIN_enabled;

    //CR3
    static Reg(Reg_RW, Reg_Bitfield, CR3, 0, 16) CR3_Reg;
    static Reg(Reg_RW, Reg_Bit, CR3, 0) error_ie;
    static Reg(Reg_RW, Reg_Bit, CR3, 1) IRDA_mode;
    static Reg(Reg_RW, Reg_Bit, CR3, 2) IRDA_low_power;
    static Reg(Reg_RW, Reg_Bit, CR3, 3) half_duplex;
    static Reg(Reg_RW, Reg_Bit, CR3, 4) SC_NACK;
    static Reg(Reg_RW, Reg_Bit, CR3, 5) SC_enabled;
    static Reg(Reg_RW, Reg_Bit, CR3, 6) DMA_RX_enabled;
    static Reg(Reg_RW, Reg_Bit, CR3, 7) DMA_TX_enabled;
    static Reg(Reg_RW, Reg_Bit, CR3, 8) RTS_enabled;
    static Reg(Reg_RW, Reg_Bit, CR3, 9) CTS_enabled;
    static Reg(Reg_RW, Reg_Bit, CR3, 10) CTS_ie;

    //GTPR
    static Reg(Reg_RW, Reg_Bitfield, GT, 0, 8) guard_time;
    static Reg(Reg_RW, Reg_Bitfield, PSC, 0, 8) prescaler;
};

#ifdef RCC_APB2ENR_USART1EN
#undef USART1
struct USART1 : USART_Impl<APB2<14> >{
    static Reg(Reg_RW, Reg_Bit, AFIO::MAPR, 2) remap;
};
#endif

#ifdef RCC_APB1ENR_USART2EN
#undef USART2
struct USART2 : USART_Impl<APB1<17> >{
    static Reg(Reg_RW, Reg_Bit, AFIO::MAPR, 3) remap;
};
#endif

#ifdef RCC_APB1ENR_USART3EN
#undef USART3
struct USART3 : USART_Impl<APB1<18> >{
    static Reg(Reg_RW, Reg_Bitfield, AFIO::MAPR, 4, 2) remap;
};
#endif

#endif//_USART_HPP_
