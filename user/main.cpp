#include "stdafx.h"
#pragma hdrstop
using namespace std;

#include "rtl.h"
#include "interface.hpp"

int main(){
    printf(
            "\x55\xaa\x55\xaa\x55\xaa\x55\xaa\r\n\r\n"
            "RESET\r\n"
            "<project-name>\r\n"
            "Build time: " __DATE__ " " __TIME__ "\r\n\r\n"
    );
    os_sys_init_prio(interface_task, interface_prio);
}
