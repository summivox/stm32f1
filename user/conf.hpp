//conf
#ifndef _CONF_HPP_
#define _CONF_HPP_

#include "common.hpp"




//Global setting: IRQ priority grouping
const U8 IRQ_priority_grouping=4;




//External hardware specification




//Unit conversions:
//  [A] * A_in_B == B_from_A * [A] == equivalent [B]
//  e.g. USD_in_CNY == CNY_from_USD == 6.3
//
//Rule:
//  - Prefer integer factor
//  - Prefer large unit converted to small unit
#define CONV(A, type, A_in_B, B) \
    static const type TOKEN_PASTE(TOKEN_PASTE(A, _in_), B) = (A_in_B); \
    static const type TOKEN_PASTE(TOKEN_PASTE(B, _from_), A) = (A_in_B)

//Time: Ts, Tms, Thms(half-ms), Tus, Tk(Tick==1/Clock Freq)
CONV(Ts, U32, 1000, Tms);
CONV(Tms, U32, 2, Thms);    CONV(Ts, U32, 2000, Thms);
CONV(Thms, U32, 500, Tus);  CONV(Tms, U32, 1000, Tus);  CONV(Ts, U32, 1000000, Tus);
CONV(Tus, U32, 72, Tk);     CONV(Thms, U32, 36000, Tk); CONV(Tms, U32, 72000, Tk);  CONV(Ts, U32, 72000000, Tk);

#undef CONV




//Derived parameters




//Software limits




//Always chain-include pinout
#include "pinout.hpp"


#endif//_CONF_HPP_
