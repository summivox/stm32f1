//helper/def
#ifndef _HELPER_DEF_HPP_
#define _HELPER_DEF_HPP_

#include "helper/conf.hpp"




//Integer type shorthands
#include "stdint.h"
typedef int8_t I8;
typedef uint8_t U8;
typedef int16_t I16;
typedef uint16_t U16;
typedef int32_t I32;
typedef uint32_t U32;
typedef int64_t I64;
typedef uint64_t U64;

typedef volatile int8_t VI8;
typedef volatile uint8_t VU8;
typedef volatile int16_t VI16;
typedef volatile uint16_t VU16;
typedef volatile int32_t VI32;
typedef volatile uint32_t VU32;
typedef volatile int64_t VI64;
typedef volatile uint64_t VU64;

//<TYPE>_AT(addr) macros: access memory at address
#define I8_AT(addr) (*(I8*)(addr))
#define U8_AT(addr) (*(U8*)(addr))
#define I16_AT(addr) (*(I16*)(addr))
#define U16_AT(addr) (*(U16*)(addr))
#define I32_AT(addr) (*(I32*)(addr))
#define U32_AT(addr) (*(U32*)(addr))
#define I64_AT(addr) (*(I64*)(addr))
#define U64_AT(addr) (*(U64*)(addr))

#define VI8_AT(addr) (*(VI8*)(addr))
#define VU8_AT(addr) (*(VU8*)(addr))
#define VI16_AT(addr) (*(VI16*)(addr))
#define VU16_AT(addr) (*(VU16*)(addr))
#define VI32_AT(addr) (*(VI32*)(addr))
#define VU32_AT(addr) (*(VU32*)(addr))
#define VI64_AT(addr) (*(VI64*)(addr))
#define VU64_AT(addr) (*(VU64*)(addr))




#endif /* _HELPER_DEF_HPP_ */
