//retarget/ST/stm32f10x.cpp
// <<< Use Configuration Wizard in Context Menu >>>




#include "retarget/retarget.h"
#include "retarget/retarget_impl.hpp"
#include "stm32f10x/usart.hpp"
#include "helper/ST/stm32f10x.hpp"

__FILE COM1;

static const U32 baud1=115200;

struct Com1Impl : UsartTxImpl, UsartRxImpl{
    Com1Impl():UsartImplBase(USART1), UsartTxImpl(USART1), UsartRxImpl(USART1){
        Freq::update();
        USART1->BRR=Freq::PCLK1/baud1;
    }
};
Com1Impl c1i;






// <<< end of configuration section >>>
