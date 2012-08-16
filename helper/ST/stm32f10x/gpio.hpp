//helper/ST/stm32f10x/gpio
#ifndef _HELPER_ST_STM32F10X_GPIO_HPP_
#define _HELPER_ST_STM32F10X_GPIO_HPP_

#include "helper/helper.hpp"

#include "stm32f10x.h"

#include "misc.hpp"
#include "exti.hpp"




//GPIO pin mode definition (output default to 50M)
enum GpioPinMode{
	GPIO_ANALOG=0x0,
	GPIO_IN_FLOATING=0x4,
	GPIO_IN_PULL=0x8,
	GPIO_OUT_PP=0x3,
	GPIO_OUT_OD=0x7,
	GPIO_AF_PP=0xB,
	GPIO_AF_OD=0xF,
};




//Mix-in base class for checking port validity
template <char Port>
struct GpioPortCheck{
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
};

//Mix-in base class for checking pin validity
template <int Pin>
struct GpioPinCheck{
	STATIC_ASSERT(0<=Pin && Pin<=15);
};




//GPIO port class
template <char Port>
struct GpioPort : GpioPortCheck<Port>{
	static const U32 N_PORT=Port-'A';
	static const U32 EN_MASK=RCC_APB2ENR_IOPAEN<<N_PORT;
	static const U32 RST_MASK=RCC_APB2RSTR_IOPARST<<N_PORT;

	static const U32 ADDR_BASE=GPIOA_BASE+(GPIOB_BASE-GPIOA_BASE)*N_PORT;

	static const U32 ADDR_ODR=PERIPH_ADDR(GPIO, ADDR_BASE, ODR);
	static const U32 ADDR_IDR=PERIPH_ADDR(GPIO, ADDR_BASE, IDR);
	static const U32 ADDR_BSRR=PERIPH_ADDR(GPIO, ADDR_BASE, BSRR);
	static const U32 ADDR_BRR=PERIPH_ADDR(GPIO, ADDR_BASE, BRR);

	static void enable(){
		RCC->APB2ENR|=EN_MASK;
	}
	static void disable(){
		RCC->APB2ENR&=~EN_MASK;
	}
	static void reset(){
		RCC->APB2RSTR|=RST_MASK;
		RCC->APB2RSTR&=~RST_MASK;
	}
};

//GPIO pin class
#define BASE GpioPort<Port>
template <char Port, int Pin>
struct GpioPin : GpioPort<Port>, GpioPinCheck<Pin>{
	static const U32 ADDR_CR=BASE::ADDR_BASE+
			((Pin<=7)?
					OFFSETOF(GPIO_TypeDef, CRL):
					OFFSETOF(GPIO_TypeDef, CRH)
			);
	static const U32 CR_PIN=(Pin<=7)?(Pin):(Pin-8);
	static const U32 CR_POS=(CR_PIN<<2);
	static const U32 CR_MASK=(0xful<<CR_POS);

	static void conf(GpioPinMode mode){
		VU32_AT(ADDR_CR)=(VU32_AT(ADDR_CR)&(~CR_MASK))|(((U8)(mode)&0xf)<<CR_POS);
	}
	static void set_exti(){
		Exti<Pin>::set_port(Port);
	}

	static void set(){
		SBIT(BASE::ADDR_BSRR, Pin)=1;
	}
	static void clear(){
		SBIT(BASE::ADDR_BRR, Pin)=1;
	}
	static bool write(bool x){
		return SBIT(BASE::ADDR_ODR, Pin)=x;
	}
	static bool read(){
		return SBIT(BASE::ADDR_IDR, Pin);
	}

	bool operator=(bool x){
		return write(x);
	}
	operator bool(){
		return read();
	}
};




//aliases for quick access
#ifdef RCC_APB2ENR_IOPAEN

typedef GpioPort<'A'> PA;
#define PA0 (GpioPin<'A', 0>())
#define PA1 (GpioPin<'A', 1>())
#define PA2 (GpioPin<'A', 2>())
#define PA3 (GpioPin<'A', 3>())
#define PA4 (GpioPin<'A', 4>())
#define PA5 (GpioPin<'A', 5>())
#define PA6 (GpioPin<'A', 6>())
#define PA7 (GpioPin<'A', 7>())
#define PA8 (GpioPin<'A', 8>())
#define PA9 (GpioPin<'A', 9>())
#define PA10 (GpioPin<'A', 10>())
#define PA11 (GpioPin<'A', 11>())
#define PA12 (GpioPin<'A', 12>())
#define PA13 (GpioPin<'A', 13>())
#define PA14 (GpioPin<'A', 14>())
#define PA15 (GpioPin<'A', 15>())

#endif

#ifdef RCC_APB2ENR_IOPBEN

typedef GpioPort<'B'> PB;
#define PB0 (GpioPin<'B', 0>())
#define PB1 (GpioPin<'B', 1>())
#define PB2 (GpioPin<'B', 2>())
#define PB3 (GpioPin<'B', 3>())
#define PB4 (GpioPin<'B', 4>())
#define PB5 (GpioPin<'B', 5>())
#define PB6 (GpioPin<'B', 6>())
#define PB7 (GpioPin<'B', 7>())
#define PB8 (GpioPin<'B', 8>())
#define PB9 (GpioPin<'B', 9>())
#define PB10 (GpioPin<'B', 10>())
#define PB11 (GpioPin<'B', 11>())
#define PB12 (GpioPin<'B', 12>())
#define PB13 (GpioPin<'B', 13>())
#define PB14 (GpioPin<'B', 14>())
#define PB15 (GpioPin<'B', 15>())

#endif

#ifdef RCC_APB2ENR_IOPCEN

typedef GpioPort<'C'> PC;
#define PC0 (GpioPin<'C', 0>())
#define PC1 (GpioPin<'C', 1>())
#define PC2 (GpioPin<'C', 2>())
#define PC3 (GpioPin<'C', 3>())
#define PC4 (GpioPin<'C', 4>())
#define PC5 (GpioPin<'C', 5>())
#define PC6 (GpioPin<'C', 6>())
#define PC7 (GpioPin<'C', 7>())
#define PC8 (GpioPin<'C', 8>())
#define PC9 (GpioPin<'C', 9>())
#define PC10 (GpioPin<'C', 10>())
#define PC11 (GpioPin<'C', 11>())
#define PC12 (GpioPin<'C', 12>())
#define PC13 (GpioPin<'C', 13>())
#define PC14 (GpioPin<'C', 14>())
#define PC15 (GpioPin<'C', 15>())

#endif

#ifdef RCC_APB2ENR_IOPDEN

typedef GpioPort<'D'> PD;
#define PD0 (GpioPin<'D', 0>())
#define PD1 (GpioPin<'D', 1>())
#define PD2 (GpioPin<'D', 2>())
#define PD3 (GpioPin<'D', 3>())
#define PD4 (GpioPin<'D', 4>())
#define PD5 (GpioPin<'D', 5>())
#define PD6 (GpioPin<'D', 6>())
#define PD7 (GpioPin<'D', 7>())
#define PD8 (GpioPin<'D', 8>())
#define PD9 (GpioPin<'D', 9>())
#define PD10 (GpioPin<'D', 10>())
#define PD11 (GpioPin<'D', 11>())
#define PD12 (GpioPin<'D', 12>())
#define PD13 (GpioPin<'D', 13>())
#define PD14 (GpioPin<'D', 14>())
#define PD15 (GpioPin<'D', 15>())

#endif

#ifdef RCC_APB2ENR_IOPEEN

typedef GpioPort<'E'> PE;
#define PE0 (GpioPin<'E', 0>())
#define PE1 (GpioPin<'E', 1>())
#define PE2 (GpioPin<'E', 2>())
#define PE3 (GpioPin<'E', 3>())
#define PE4 (GpioPin<'E', 4>())
#define PE5 (GpioPin<'E', 5>())
#define PE6 (GpioPin<'E', 6>())
#define PE7 (GpioPin<'E', 7>())
#define PE8 (GpioPin<'E', 8>())
#define PE9 (GpioPin<'E', 9>())
#define PE10 (GpioPin<'E', 10>())
#define PE11 (GpioPin<'E', 11>())
#define PE12 (GpioPin<'E', 12>())
#define PE13 (GpioPin<'E', 13>())
#define PE14 (GpioPin<'E', 14>())
#define PE15 (GpioPin<'E', 15>())

#endif

#ifdef RCC_APB2ENR_IOPFEN

typedef GpioPort<'F'> PF;
#define PF0 (GpioPin<'F', 0>())
#define PF1 (GpioPin<'F', 1>())
#define PF2 (GpioPin<'F', 2>())
#define PF3 (GpioPin<'F', 3>())
#define PF4 (GpioPin<'F', 4>())
#define PF5 (GpioPin<'F', 5>())
#define PF6 (GpioPin<'F', 6>())
#define PF7 (GpioPin<'F', 7>())
#define PF8 (GpioPin<'F', 8>())
#define PF9 (GpioPin<'F', 9>())
#define PF10 (GpioPin<'F', 10>())
#define PF11 (GpioPin<'F', 11>())
#define PF12 (GpioPin<'F', 12>())
#define PF13 (GpioPin<'F', 13>())
#define PF14 (GpioPin<'F', 14>())
#define PF15 (GpioPin<'F', 15>())

#endif

#ifdef RCC_APB2ENR_IOPGEN

typedef GpioPort<'G'> PG;
#define PG0 (GpioPin<'G', 0>())
#define PG1 (GpioPin<'G', 1>())
#define PG2 (GpioPin<'G', 2>())
#define PG3 (GpioPin<'G', 3>())
#define PG4 (GpioPin<'G', 4>())
#define PG5 (GpioPin<'G', 5>())
#define PG6 (GpioPin<'G', 6>())
#define PG7 (GpioPin<'G', 7>())
#define PG8 (GpioPin<'G', 8>())
#define PG9 (GpioPin<'G', 9>())
#define PG10 (GpioPin<'G', 10>())
#define PG11 (GpioPin<'G', 11>())
#define PG12 (GpioPin<'G', 12>())
#define PG13 (GpioPin<'G', 13>())
#define PG14 (GpioPin<'G', 14>())
#define PG15 (GpioPin<'G', 15>())

#endif


#endif /* _HELPER_ST_STM32F10X_GPIO_HPP_ */
