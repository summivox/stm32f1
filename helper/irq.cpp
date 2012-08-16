//helper/irq
#include "helper/irq.hpp"




//IRQ priority init helper functions
void IRQ_priority_init(IRQn_Type IRQn, U8 P1, U8 P2){
    NVIC_SetPriority(IRQn, NVIC_EncodePriority(IRQ_priority_grouping, P1, P2));
}
void IRQ_priority_grouping_init() __attribute__((constructor(101)));
void IRQ_priority_grouping_init(){
    NVIC_SetPriorityGrouping(IRQ_priority_grouping);
}
