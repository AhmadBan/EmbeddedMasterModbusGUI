/*
 * coil.h
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */

#ifndef GUI_INC_DO_H_
#define GUI_INC_DO_H_
#include "mainpage.h"
#include "stdio.h"
#include <modbus.h>
void coil_create(lv_obj_t * parent);
void initiateCoils(int start,int count);
#endif /* GUI_INC_DO_H_ */
