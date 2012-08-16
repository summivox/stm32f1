//helper/bitband
//Prerequisite: none
#ifndef _HELPER_BITBAND_HPP_
#define _HELPER_BITBAND_HPP_

#include "helper/conf.hpp"
#include "helper/def.hpp"




//Bitwise Hack: Extracts lowbit from a U32
#define LOWBIT(x) ( (x) & -(x) )

//Bitwise Hack: return position of lowest bit in x
static const int LOWBITn_LUT[32]={
	0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
	31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};
#define LOWBITn(x) (LOWBITn_LUT[(U32)(LOWBIT(x)*0x077CB531u)>>27])




//Bitband alias macros
//PBIT(addr, bit) returns the address to the bitband alias
//SBIT(addr, bit) returns reference to the bitband alias
//[PS]BITMASK(addr, bitmask) takes bitmask instead of bit position
#define PBIT(addr, bit) (( (U32)(addr) &0xF0000000)+0x02000000+(((( (U32)(addr) &0xFFFFF)<<3)+ (bit)) <<2))
#define SBIT(addr, bit) VU32_AT(PBIT(addr, bit))
#define PBITMASK(addr, bitmask) PBIT(addr, LOWBITn(bitmask))
#define SBITMASK(addr, bitmask) SBIT(addr, LOWBITn(bitmask))




#endif /* _HELPER_BITBAND_HPP_ */
