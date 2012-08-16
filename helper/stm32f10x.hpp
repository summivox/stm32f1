//helper/stm32f10x
#ifndef _HELPER_STM32F10X_HPP_
#define _HELPER_STM32F10X_HPP_

#include "stm32f10x.h"
#include "helper/conf.hpp"
#include "helper/def.hpp"




//Shortcut to peripheral bitband
//Usage: PERIPH_SBIT(I2C, I2C1, CR1, PE)=1

#define PERIPH_PBIT(type, periph, reg, bit) \
	PBIT( \
		(U32)(periph)+OFFSETOF(type##_TypeDef, reg), \
		LOWBITn(type##_##reg##_##bit) \
	)

#define PERIPH_SBIT(type, periph, reg, bit) \
	SBIT( \
		(U32)(periph)+OFFSETOF(type##_TypeDef, reg), \
		LOWBITn(type##_##reg##_##bit) \
	)

#endif /* _HELPER_STM32F10X_HPP_ */
