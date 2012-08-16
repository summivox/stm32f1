//helper/ST/stm32f10x/misc
#ifndef _HELPER_ST_STM32F10X_MISC_HPP_
#define _HELPER_ST_STM32F10X_MISC_HPP_

#include "helper/helper.hpp"

#include "stm32f10x.h"




//Shortcut to peripheral bitband
//Usage: PERIPH_SBIT(I2C, I2C1, CR1, PE)=1

#define PERIPH_PBIT(type, periph, reg, bit) \
	PBITMASK( \
		(U32)(periph)+OFFSETOF(type##_TypeDef, reg), \
		type##_##reg##_##bit \
	)

#define PERIPH_SBIT(type, periph, reg, bit) \
	SBITMASK( \
		(U32)(periph)+OFFSETOF(type##_TypeDef, reg), \
		type##_##reg##_##bit \
	)

//Shortcut to peripheral clock control bit
//NOTE: operates on compile-time constant bitfield alias address
//Usage: RCC_ENR(APB2, IOPAEN)=1
#define RCC_xR(bus, periph, x) \
	PERIPH_SBIT(RCC, RCC, bus##x, periph)
#define RCC_xR_ADDR(bus, periph, x) \
	PERIPH_PBIT(RCC, RCC, bus##x, periph)

#define RCC_ENR(bus, periph) RCC_xR(bus, periph, ENR)
#define RCC_RSTR(bus, periph) RCC_xR(bus, periph, RSTR)
#define RCC_ENR_ADDR(bus, periph) RCC_xR_ADDR(bus, periph, ENR)
#define RCC_RSTR_ADDR(bus, periph) RCC_xR_ADDR(bus, periph, RSTR)

//Shortcut to extract peripheral register address
//Usage: refer to [stm32f10x_gpio.hpp]
#define PERIPH_ADDR(type, base, reg) ((base)+OFFSETOF(type##_TypeDef, reg))




#endif /* _HELPER_ST_STM32F10X_MISC_HPP_ */
