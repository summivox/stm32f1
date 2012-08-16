//interface
#include "stdafx.h"
#pragma hdrstop
using namespace std;

#include "interface.hpp"




OS_TID interface_TID;
void interface_task(){
    interface_TID=os_tsk_self();
    printf("interface_task: TID=%d\r\n", (int)interface_TID);
    printf("commands:\r\n");
    
	char cmd;
    while(1){
        scanf(" %c", &cmd);
        switch(cmd){
            default:
                break;
        }
    }
}
