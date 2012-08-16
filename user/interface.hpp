//interface: (RTX task) handles IO with host controller
#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

#include "rtl.h"
#include "conf.hpp"
#include "misc.hpp"




enum interface_flag_t{
};
extern OS_TID interface_TID;
static const int interface_prio=50;
__task void interface_task();




#endif//_INTERFACE_HPP_
