//helper/conf
#ifndef CONF_HPP_
#define CONF_HPP_

//Include main CMSIS header here:
#include "stm32f10x.h"

//Include chip-specific helper header here:
#include "helper/stm32f10x.hpp"




//Global setting of [helper/irq]:
static const int IRQ_priority_grouping=4; //NVIC IRQ priority grouping bit index



#endif /* CONF_HPP_ */
