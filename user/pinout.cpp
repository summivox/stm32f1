//pinout
#include "stdafx.h"
#pragma hdrstop
using namespace std;

#include "pinout.hpp"

void pinout_init(){

    //switch on all GPIOs
    PA::enable();
    PB::enable();

#define E(pin) \
    pin##_EXTI::enable(); \
    pin##_EXTI::prepare_GPIO<pin##_PORT>(GPIO::in_floating); \
    pin##_EXTI::rising=true
#define I(pin) pin.conf(GPIO::in, GPIO::in_floating)
#define O(pin) pin.conf(GPIO::out_50M, GPIO::out_pushpull)
#define P(pin) pin.conf(GPIO::out_50M, GPIO::af_pushpull)



#undef P
#undef O
#undef I
#undef E
}
