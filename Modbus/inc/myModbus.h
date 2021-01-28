/*
 * modbus.h
 *
 *  Created on: Jan 24, 2021
 *      Author: Ahmad
 */

#ifndef MODBUS_INC_MYMODBUS_H_
#define MODBUS_INC_MYMODBUS_H_

void tcpModbusInit();
int tcpModbusConnect(void);
void tcpModbusDisconnect(void);

#endif /* MODBUS_INC_MYMODBUS_H_ */
