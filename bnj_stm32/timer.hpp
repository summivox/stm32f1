//timer.hpp: Advanced/General-Purpose/Basic Timer
#ifndef _TIMER_HPP_
#define _TIMER_HPP_
#pragma thumb

#include "stm32f10x.h"
#include "common.hpp"
#include "register.hpp"
#include "bus.hpp"

template <typename Base>
struct Timer_Impl : Base{
    //2012-06-26: Use CMSIS instead, period.
};

#ifdef RCC_APB2ENR_TIM1EN
struct Timer1 : Timer_Impl<APB2<11> > {};
#endif
#ifdef RCC_APB1ENR_TIM2EN
struct Timer2 : Timer_Impl<APB1<0> > {};
#endif
#ifdef RCC_APB1ENR_TIM3EN
struct Timer3 : Timer_Impl<APB1<1> > {};
#endif
#ifdef RCC_APB1ENR_TIM4EN
struct Timer4 : Timer_Impl<APB1<2> > {};
#endif
#ifdef RCC_APB1ENR_TIM5EN
struct Timer5 : Timer_Impl<APB1<3> > {};
#endif
#ifdef RCC_APB1ENR_TIM6EN
struct Timer6 : Timer_Impl<APB1<4> > {};
#endif
#ifdef RCC_APB1ENR_TIM7EN
struct Timer7 : Timer_Impl<APB1<5> > {};
#endif
#ifdef RCC_APB2ENR_TIM8EN
struct Timer8 : Timer_Impl<APB2<13> > {};
#endif

#endif//_TIMER_HPP_
