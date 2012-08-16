//helper/armcc
//Prerequisite: [system/linker.sct]
#ifndef _HELPER_ARMCC_HPP_
#define _HELPER_ARMCC_HPP_

#include "helper/conf.hpp"
#include "helper/def.hpp"




//OFFSETOF() macro: offsetof() that can be recognized by Eclipse CDT
#define OFFSETOF(t, memb) ((U32)(&(((t*)0)->memb)))

//Shortcut for placing code in SRAM (used in conjunction with linker script)
//Switch off for better debugging
//Usage: append to function declaration
#ifndef NDEBUG
#define IN_RAM /*NOP*/
#else //NDEBUG
#define IN_RAM __attribute__((section ("RAMCODE")))
#endif//NDEBUG

//Shortcut for disabling function inlining
//NOTE: 2012-07-05 "Flash, SRAM, inline, Axis::update(), I-Code/D-Code bus malfunction" workaround
//Usage: append to function declaration
#define NO_INLINE __attribute__((noinline))

//Shortcut for making function autorun before main()
//Usage: append to function declaration
#define AUTORUN __attribute__((constructor))
#define AUTORUN_PRIO(prio) __attribute__((constructor(prio)))




#endif /* _HELPER_ARMCC_HPP_ */
