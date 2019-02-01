/*
 * TRV.h
 *
 * Created: 31.07.2018 9:24:04
 *  Author: User
 */ 


#ifndef TRV_H
#define TRV_H

#include  "FreeRTOS.h"
#include  "timers.h"
#include  "serial.h"
#include  "main.h"
#include <stdlib.h>
extern int old_corection_1,old_corection_2;
#ifdef __cplusplus
extern "C" {
#endif

void activateTrvTimer(xCompressor *comp);	
void ComutateTrv(TimerHandle_t xTimer);	
void startTrvTimer(xCompressor *comp);
void stopTrvTimer(xCompressor *comp);
void deleteTrvTimer(xCompressor *comp);
int Fuzzy(xCompressor *comp,int corection);
void calibration(xCompressor *comp);


TimerHandle_t xComutateTrv_1,xComutateTrv_2;

#ifdef __cplusplus
}
#endif
#endif /* TRV_H_ */