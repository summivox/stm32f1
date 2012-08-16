//misc
#ifndef _MISC_HPP_
#define _MISC_HPP_

#include "helper/helper.hpp"

//Shortcut to peripheral clock control bit
//NOTE: operates on compile-time constant bitfield alias address
//Usage: RCC_ENR(APB2, IOPAEN)=1
#define RCC_xR(bus, periph, x) \
	PERIPH_SBIT(RCC, RCC, bus##x, periph)

#define RCC_ENR(bus, periph) RCC_xR(bus, periph, ENR)
#define RCC_RSTR(bus, periph) RCC_xR(bus, periph, RSTR)

#endif//_MISC_HPP_
