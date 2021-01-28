/*
 * modbus.c
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */
#include <modbus.h>
#include "mainpage.h"

modbus_t *mb;

uint16_t tab_reg[32];
extern lv_obj_t * ipTa;
extern lv_obj_t * portTa;
char *temp1,*temp2;

void tcpModbusInit(){
	int port=0;

	temp1=lv_textarea_get_text(ipTa);
	temp2=lv_textarea_get_text(portTa);
	sscanf(temp2,"%d",&port);
	mb =  modbus_new_tcp(temp1, port);
	modbus_set_debug(mb, 1);
}

int tcpModbusConnect(void){
	 return  modbus_connect(mb);
}


void tcpModbusDisconnect(void){
	  modbus_close(mb);
	  modbus_free(mb);
}
