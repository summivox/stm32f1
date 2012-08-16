#include "stdafx.h"
#pragma hdrstop
using namespace std;

#include "rtl.h"

#include "conf.hpp"
#include "misc.hpp"

__task void main_task(){
	PE::enable();
	PE7.conf(GPIO_OUT_PP);
	PE7=1;

	PF::enable();
	PF13.conf(GPIO_IN_PULL);
	PF13=1;

	Exti<13>::enable();
	Exti<13>::set_falling();
	PF13.set_exti();
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	wait_ms(1000);
	//TODO: Initialize and launch other tasks
	os_tsk_delete_self();
}

IRQ_DECL(EXTI15_10_IRQn, EXTI15_10_IRQHandler, 5, 0);
void EXTI15_10_IRQHandler(){
	PE7=!PE7;
	Exti<13>::clear();
}

int main(){
	os_sys_init_prio(main_task, 0xFE);
}
