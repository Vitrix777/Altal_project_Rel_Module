/*
 * ModBus.h
 *
 * Created: 26.07.2018 19:58:49
 *  Author: User
 */ 


#ifndef MODBUS_H
#define MODBUS_H


#include <util/delay.h>
#include <avr/eeprom.h>
#include  "main.h"
#include  "serial.h"

#ifdef __cplusplus
extern "C" {
	#endif
	

#define  slaveID		1
// frame[] is used to recieve and transmit packages.
// The maximum serial ring buffer size is 128
volatile unsigned char frame[portSERIAL_BUFFER_RX],indexBuf,overflow;

unsigned char function;

typedef enum 
   {
	   OVERFLOW_BUFF,
	   ERROR_FUNC,
	   CORRUPT_PACKET,
	   CHECKSUM_FAIL
	} MBerror;

void  modbus_update();
void timer_1_Init();
void start_1_Timer();
void stop_1_Timer();
int readSysReg(xCompressor*compPtr,xEEdata *eeprom,uint16_t adress);
void writeSysReg(xEEdata*comp,uint16_t adress,uint16_t value);
int readHoldingReg(xEEdata*comp,uint16_t adress);
void exceptionResponse(unsigned char exception);
unsigned int calculateCRC(unsigned char bufferSize);
void sendPacket(unsigned char bufferSize);

#ifdef __cplusplus
}
#endif

#endif /* MODBUS_H_ */