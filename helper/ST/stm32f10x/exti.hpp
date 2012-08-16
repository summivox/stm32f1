//helper/ST/stm32f10x/exti
#ifndef _HELPER_ST_STM32F10X_EXTI_HPP_
#define _HELPER_ST_STM32F10X_EXTI_HPP_

#include "helper/helper.hpp"

#include "stm32f10x.h"

#include "misc.hpp"




template <int Line>
struct Exti{
	STATIC_ASSERT(0<=Line && Line<=19);

	static const U32 ADDR_BASE=EXTI_BASE;
	static const U32 ADDR_IMR=PERIPH_ADDR(EXTI, ADDR_BASE, IMR);
	static const U32 ADDR_EMR=PERIPH_ADDR(EXTI, ADDR_BASE, EMR);
	static const U32 ADDR_RTSR=PERIPH_ADDR(EXTI, ADDR_BASE, RTSR);
	static const U32 ADDR_FTSR=PERIPH_ADDR(EXTI, ADDR_BASE, FTSR);
	static const U32 ADDR_SWIER=PERIPH_ADDR(EXTI, ADDR_BASE, SWIER);
	static const U32 ADDR_PR=PERIPH_ADDR(EXTI, ADDR_BASE, PR);

	static const U32 CR_SEL=(Line<=15)?((Line>>2)<<2):0;
	static const U32 CR_IDX=(Line<=15)?((Line&0x3)<<2):0;
	static const U32 CR_MASK=(Line<=15)?((0xf)<<(CR_IDX)):0;
	static const U32 ADDR_CR=PERIPH_ADDR(AFIO, AFIO_BASE, EXTICR)+CR_SEL;

	static void enable(){
		RCC_ENR(APB2, AFIOEN)=1;
		SBIT(ADDR_IMR, Line)=1;
	}
	static void disable(){
		SBIT(ADDR_IMR, Line)=0;
	}
	static void set_rising(bool x=true){
		SBIT(ADDR_RTSR, Line)=x;
	}
	static void set_falling(bool x=true){
		SBIT(ADDR_FTSR, Line)=x;
	}
	static void set_port(char port){
		VU32_AT(ADDR_CR)=(VU32_AT(ADDR_CR)&(~CR_MASK))|((port-'A')<<CR_IDX);
	}
	static bool pending(){
		return SBIT(ADDR_PR, Line);
	}
	static void clear(){
		SBIT(ADDR_PR, Line)=1;
	}
};




#endif /* _HELPER_ST_STM32F10X_EXTI_HPP_ */
