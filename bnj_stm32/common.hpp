//common.hpp: generic definitions for Cortex-M3
#ifndef _COMMON_HPP_
#define _COMMON_HPP_
#pragma thumb

//Integer shorthands
//NOTE: Fuck STM32 Firmware's conventions.
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

//Memory macro
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

//Bitwise Hack: Extracts lowbit from a U32
#define LOWBIT(x) ( (x) & -(x) )

//Bitwise Hack: Finds trailing zeros(index of lowbit) of a U32
//Note: CMSIS has every bit field defined in bitmask style, which is a bad design.
//The reason to use indices: Drop read-alter-write and use bitband instead.
namespace {
    static const int LOWBITn_LUT[32]={
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
}
#define LOWBITn(x) (LOWBITn_LUT[(U32)(LOWBIT(x)*0x077CB531u)>>27])

//Bitband Alias macro
//PBIT: Pointer to BITband alias(as U32)
//SBIT: Named after C51 equivalent
#define PBIT(addr, bit) (( (U32)(addr) &0xF0000000)+0x02000000+(((( (U32)(addr) &0xFFFFF)<<3)+ (bit)) <<2))
#define SBIT(addr, bit) VU32_AT(PBIT(addr, bit))
#define PBITMASK(addr, bitmask) PBIT(addr, LOWBITn(bitmask))
#define SBITMASK(addr, bitmask) SBIT(addr, LOWBITn(bitmask))
#define SBIT_COND(x) ( (0x20000000<=(x) && (x)<0x20100000) || (0x40000000<=(x) && (x)<0x40100000) )

//For template metaprogramming
#define APPLY(func_class, ...) func_class::template apply< __VA_ARGS__ >::type

//Compile-time assertion: Use BOOST_STATIC_ASSERT if available. Otherwise use a similar implementation.
#ifdef BOOST_STATIC_ASSERT
#define TOKEN_PASTE(A, B) BOOST_JOIN(A, B)
#define STATIC_ASSERT(B) BOOST_STATIC_ASSERT(B)
#else//BOOST_STATIC_ASSERT

template <bool x> struct STATIC_ASSERTION_FAILURE;
template <> struct STATIC_ASSERTION_FAILURE<true>{enum{value=1};};
template <int x> struct STATIC_ASSERT_TEST{};

#define TOKEN_PASTE_2(A, B) A##B
#define TOKEN_PASTE_1(A, B) TOKEN_PASTE_2(A, B)
#define TOKEN_PASTE(A, B) TOKEN_PASTE_1(A, B)

#define STATIC_ASSERT(B) \
    typedef STATIC_ASSERT_TEST< sizeof(STATIC_ASSERTION_FAILURE<(bool)(B)>) > \
        TOKEN_PASTE(TOKEN_PASTE(TOKEN_PASTE( \
                STATIC_ASSERT_TYPEDEF_, __COUNTER__), _ON_LINE_), __LINE__)

#endif//BOOST_STATIC_ASSERT


#endif//_COMMON_HPP_
