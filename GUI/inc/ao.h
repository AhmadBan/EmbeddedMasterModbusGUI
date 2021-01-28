/*
 * input_register.h
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */

#ifndef GUI_INC_AO_H_
#define GUI_INC_AO_H_
#include "mainpage.h"
#include <modbus.h>

void holdingRegister_create(lv_obj_t * parent);
void initiateAo(int start,int count);
void aoEnableSingleWrite(int status);
#endif /* GUI_INC_AO_H_ */
