#include "stdafx.h"
#pragma hdrstop
using namespace std;

#include "rtl.h"

#include "conf.hpp"
#include "misc.hpp"




__task void main_task(){
	wait_ms(1000);
	//TODO: Initialize and launch other tasks
	os_tsk_delete_self();
}

int main(){
	os_sys_init_prio(main_task, 0xFE);
}
