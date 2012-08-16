//gpio.hpp: GPIO and AFIO
#ifndef _GPIO_HPP_
#define _GPIO_HPP_
#pragma thumb

#include "stm32f10x.h"
#include "common.hpp"
#include "register.hpp"
#include "bus.hpp"

//Dummy base class: workaround for enums
//Honestly it doesn't live up to its name--more of a mix-in, but so be it,
//as long as the semantics is good.
#undef GPIO
struct GPIO{
    //direction
    enum dir_t{
        in=0,
        out_10M=1,
        out_2M=2,
        out_50M=3,
    };
    //drive type
    enum in_drive_t{
        in_analog=0,
        in_floating=1,
        in_pull=2,
    };
    enum out_drive_t{
        out_pushpull=0,
        out_opendrain=1,
        af_pushpull=2,
        af_opendrain=3,
    };
};

#undef GPIO_Base
template <char Port=0, U32 Pin=~0u>
struct GPIO_Base : GPIO, APB2<Port-'A'+2>{
    STATIC_ASSERT(Port==0 || ('A'<=Port && Port<='G'));
    STATIC_ASSERT(Pin==~0 || (0<=Pin && Pin<=15));
#ifndef RCC_APB2ENR_IOPAEN
    STATIC_ASSERT(Port!='A');
#endif
#ifndef RCC_APB2ENR_IOPBEN
    STATIC_ASSERT(Port!='B');
#endif
#ifndef RCC_APB2ENR_IOPCEN
    STATIC_ASSERT(Port!='C');
#endif
#ifndef RCC_APB2ENR_IOPDEN
    STATIC_ASSERT(Port!='D');
#endif
#ifndef RCC_APB2ENR_IOPEEN
    STATIC_ASSERT(Port!='E');
#endif
#ifndef RCC_APB2ENR_IOPFEN
    STATIC_ASSERT(Port!='F');
#endif
#ifndef RCC_APB2ENR_IOPGEN
    STATIC_ASSERT(Port!='G');
#endif
    enum{
        port_n      =   Port-'A',
        pin_n       =   Pin,
    };
    enum addr{
		base		=	APB2<Port-'A'+2>::base,
        //
        CRL         =   base+0x00,      //Control Reg. LSB
        CRH         =   base+0x04,      //Control Reg. MSB
        IDR         =   base+0x08,      //Input Data Reg.
        ODR         =   base+0x0C,      //Output Data Reg.
        BSRR        =   base+0x10,      //Bit Set Reset Reg.
        BRR         =   base+0x14,      //Bit Reset Reg.
        LCKR        =   base+0x18,      //Lock Reg.
        //special: CR selector
        CR          =   (0<=Pin && Pin<=7)?CRL:CRH,
        CR_idx      =   (Pin&7)<<2,
    };
};

#define BASE GPIO_Base<Port>
#define REGBASE Reg_Comb_Base(U16,          \
    /*read*/    Reg_Whole<BASE::IDR,  U16>, \
    /*write*/   Reg_Whole<BASE::ODR,  U16>, \
    /*set*/     Reg_Whole<BASE::BSRR, U16>, \
    /*clear*/   Reg(Reg_RC_W1, Reg_Whole, BASE::BRR, U16), \
    /*toggle*/  Reg_Default)
template <char Port=0>
struct GPIO_Port : GPIO, BASE, REGBASE{
    using REGBASE::operator=;
    using REGBASE::operator U16;
    static bool lock(){
        static Reg_Norm(BASE::LCKR, 16, 1) LCKK;
        LCKK=1;LCKK=0;LCKK=1;
        return !LCKK && LCKK; //return true if lock success(read 0 then read 1)
    }
};
#undef BASE
#undef REGBASE


#define BASE GPIO_Base<Port, Pin>
#define REGBASE Reg_Comb_Base(bool,       \
    /*read*/    Reg_Bit<BASE::IDR,  Pin>, \
    /*write*/   Reg_Bit<BASE::ODR,  Pin>, \
    /*set*/     Reg_Bit<BASE::BSRR, Pin>, \
    /*clear*/   Reg(Reg_RC_W1, Reg_Bit, BASE::BRR, Pin), \
    /*toggle*/  Reg_Default)

template <char Port=0, U32 Pin=~0u>
struct GPIO_Pin : GPIO, BASE, REGBASE{
    using REGBASE::operator=;
    using REGBASE::operator bool;
    static Reg_Norm(BASE::CR, BASE::CR_idx,   2) dir;
    static Reg_Norm(BASE::CR, BASE::CR_idx+2, 2) drive;
    static void conf(dir_t dir_=dir_t(0), U32 drive_=1){
        dir=dir_;
        drive=drive_;
    }
    static Reg_Norm(BASE::LCKR, Pin, 1) locked;
    static void lock(){locked=true;}
};
#undef BASE
#undef REGBASE

#undef IF_P
#undef IF_I
#undef IF_PI

//aliases for quick access
//namespace for code folding
namespace{
#ifdef RCC_APB2ENR_IOPAEN

typedef GPIO_Port<'A'> PA;
#define PA0 (GPIO_Pin<'A', 0>())
#define PA1 (GPIO_Pin<'A', 1>())
#define PA2 (GPIO_Pin<'A', 2>())
#define PA3 (GPIO_Pin<'A', 3>())
#define PA4 (GPIO_Pin<'A', 4>())
#define PA5 (GPIO_Pin<'A', 5>())
#define PA6 (GPIO_Pin<'A', 6>())
#define PA7 (GPIO_Pin<'A', 7>())
#define PA8 (GPIO_Pin<'A', 8>())
#define PA9 (GPIO_Pin<'A', 9>())
#define PA10 (GPIO_Pin<'A', 10>())
#define PA11 (GPIO_Pin<'A', 11>())
#define PA12 (GPIO_Pin<'A', 12>())
#define PA13 (GPIO_Pin<'A', 13>())
#define PA14 (GPIO_Pin<'A', 14>())
#define PA15 (GPIO_Pin<'A', 15>())

#endif

#ifdef RCC_APB2ENR_IOPBEN

typedef GPIO_Port<'B'> PB;
#define PB0 (GPIO_Pin<'B', 0>())
#define PB1 (GPIO_Pin<'B', 1>())
#define PB2 (GPIO_Pin<'B', 2>())
#define PB3 (GPIO_Pin<'B', 3>())
#define PB4 (GPIO_Pin<'B', 4>())
#define PB5 (GPIO_Pin<'B', 5>())
#define PB6 (GPIO_Pin<'B', 6>())
#define PB7 (GPIO_Pin<'B', 7>())
#define PB8 (GPIO_Pin<'B', 8>())
#define PB9 (GPIO_Pin<'B', 9>())
#define PB10 (GPIO_Pin<'B', 10>())
#define PB11 (GPIO_Pin<'B', 11>())
#define PB12 (GPIO_Pin<'B', 12>())
#define PB13 (GPIO_Pin<'B', 13>())
#define PB14 (GPIO_Pin<'B', 14>())
#define PB15 (GPIO_Pin<'B', 15>())

#endif

#ifdef RCC_APB2ENR_IOPCEN

typedef GPIO_Port<'C'> PC;
#define PC0 (GPIO_Pin<'C', 0>())
#define PC1 (GPIO_Pin<'C', 1>())
#define PC2 (GPIO_Pin<'C', 2>())
#define PC3 (GPIO_Pin<'C', 3>())
#define PC4 (GPIO_Pin<'C', 4>())
#define PC5 (GPIO_Pin<'C', 5>())
#define PC6 (GPIO_Pin<'C', 6>())
#define PC7 (GPIO_Pin<'C', 7>())
#define PC8 (GPIO_Pin<'C', 8>())
#define PC9 (GPIO_Pin<'C', 9>())
#define PC10 (GPIO_Pin<'C', 10>())
#define PC11 (GPIO_Pin<'C', 11>())
#define PC12 (GPIO_Pin<'C', 12>())
#define PC13 (GPIO_Pin<'C', 13>())
#define PC14 (GPIO_Pin<'C', 14>())
#define PC15 (GPIO_Pin<'C', 15>())

#endif

#ifdef RCC_APB2ENR_IOPDEN

typedef GPIO_Port<'D'> PD;
#define PD0 (GPIO_Pin<'D', 0>())
#define PD1 (GPIO_Pin<'D', 1>())
#define PD2 (GPIO_Pin<'D', 2>())
#define PD3 (GPIO_Pin<'D', 3>())
#define PD4 (GPIO_Pin<'D', 4>())
#define PD5 (GPIO_Pin<'D', 5>())
#define PD6 (GPIO_Pin<'D', 6>())
#define PD7 (GPIO_Pin<'D', 7>())
#define PD8 (GPIO_Pin<'D', 8>())
#define PD9 (GPIO_Pin<'D', 9>())
#define PD10 (GPIO_Pin<'D', 10>())
#define PD11 (GPIO_Pin<'D', 11>())
#define PD12 (GPIO_Pin<'D', 12>())
#define PD13 (GPIO_Pin<'D', 13>())
#define PD14 (GPIO_Pin<'D', 14>())
#define PD15 (GPIO_Pin<'D', 15>())

#endif

#ifdef RCC_APB2ENR_IOPEEN

typedef GPIO_Port<'E'> PE;
#define PE0 (GPIO_Pin<'E', 0>())
#define PE1 (GPIO_Pin<'E', 1>())
#define PE2 (GPIO_Pin<'E', 2>())
#define PE3 (GPIO_Pin<'E', 3>())
#define PE4 (GPIO_Pin<'E', 4>())
#define PE5 (GPIO_Pin<'E', 5>())
#define PE6 (GPIO_Pin<'E', 6>())
#define PE7 (GPIO_Pin<'E', 7>())
#define PE8 (GPIO_Pin<'E', 8>())
#define PE9 (GPIO_Pin<'E', 9>())
#define PE10 (GPIO_Pin<'E', 10>())
#define PE11 (GPIO_Pin<'E', 11>())
#define PE12 (GPIO_Pin<'E', 12>())
#define PE13 (GPIO_Pin<'E', 13>())
#define PE14 (GPIO_Pin<'E', 14>())
#define PE15 (GPIO_Pin<'E', 15>())

#endif

#ifdef RCC_APB2ENR_IOPFEN

typedef GPIO_Port<'F'> PF;
#define PF0 (GPIO_Pin<'F', 0>())
#define PF1 (GPIO_Pin<'F', 1>())
#define PF2 (GPIO_Pin<'F', 2>())
#define PF3 (GPIO_Pin<'F', 3>())
#define PF4 (GPIO_Pin<'F', 4>())
#define PF5 (GPIO_Pin<'F', 5>())
#define PF6 (GPIO_Pin<'F', 6>())
#define PF7 (GPIO_Pin<'F', 7>())
#define PF8 (GPIO_Pin<'F', 8>())
#define PF9 (GPIO_Pin<'F', 9>())
#define PF10 (GPIO_Pin<'F', 10>())
#define PF11 (GPIO_Pin<'F', 11>())
#define PF12 (GPIO_Pin<'F', 12>())
#define PF13 (GPIO_Pin<'F', 13>())
#define PF14 (GPIO_Pin<'F', 14>())
#define PF15 (GPIO_Pin<'F', 15>())

#endif

#ifdef RCC_APB2ENR_IOPGEN

typedef GPIO_Port<'G'> PG;
#define PG0 (GPIO_Pin<'G', 0>())
#define PG1 (GPIO_Pin<'G', 1>())
#define PG2 (GPIO_Pin<'G', 2>())
#define PG3 (GPIO_Pin<'G', 3>())
#define PG4 (GPIO_Pin<'G', 4>())
#define PG5 (GPIO_Pin<'G', 5>())
#define PG6 (GPIO_Pin<'G', 6>())
#define PG7 (GPIO_Pin<'G', 7>())
#define PG8 (GPIO_Pin<'G', 8>())
#define PG9 (GPIO_Pin<'G', 9>())
#define PG10 (GPIO_Pin<'G', 10>())
#define PG11 (GPIO_Pin<'G', 11>())
#define PG12 (GPIO_Pin<'G', 12>())
#define PG13 (GPIO_Pin<'G', 13>())
#define PG14 (GPIO_Pin<'G', 14>())
#define PG15 (GPIO_Pin<'G', 15>())

#endif
};


//AFIO delegate class
//Because it's more tightly associated with the corresponding peripherals, no register
//control has been implemented.
#undef AFIO
struct AFIO : APB2<0>{
    enum addr{
        //base address
        base        =   APB2<0>::base,
        //register offset
        EVCR        =   base+0x00,      //Event Control Reg.
        MAPR        =   base+0x04,      //Remap Reg.
        EXTICR      =   base+0x08,      //External Interrupt Control Reg. *4
    };
};

#endif//_GPIO_HPP_
