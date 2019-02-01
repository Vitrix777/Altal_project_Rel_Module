
#ifndef MODS_H
#define MODS_H

#include  <time.h>
#include  <stdbool.h>
#include <avr/eeprom.h>
#include <math.h>
#include <util/delay.h>
#include  "FreeRTOS.h"
#include  "timers.h"
#include  "task.h"
#include  "serial.h"
#include  "main.h"
#include  "TRV.h"

#ifdef __cplusplus
extern "C" {
	#endif

#define OFF			0
#define DHW			1
#define HEAT		2
#define COOL		3
#define HEAT_DHW	4
#define COOL_DHW	5

void startDefrost(xCompressor *comp,xEEdata* eeData);
void stopDefrost(xCompressor *comp,xEEdata* eeData);

void activateTimers(xCompressor *comp);
void stop_timers( xCompressor *comp);
void StarSoursePump(xCompressor *comp);
void StartReversingValve(TimerHandle_t xTimer);//
void StartThreeWay();
void StartHeatPump(TimerHandle_t xTimer);
void StartComp(TimerHandle_t xTimer);
void StartCheckErrors(TimerHandle_t xTimer);
void StartCheckTrv(TimerHandle_t xTimer);
//--------------stop function-------------//
void Comp_Stop(xCompressor *comp);
void SwitchRev_1_Time(TimerHandle_t xTimer);
void SwitchRev_2_Time(TimerHandle_t xTimer);
void HeatPump_Stop(TimerHandle_t xTimer);
void ThreeWay_Stop();
void Revers_Stop(TimerHandle_t xTimer);
void SorcePump_Stop(TimerHandle_t xTimer);
//---------------------------------------
uint8_t selmode(xEEdata* eeData);
void goToMode(xCompressor *comp);
void mod_pump( xCompressor *comp,xEEdata* eeData);
void initPins();

extern TaskHandle_t xHandleSecond;
//------start
TimerHandle_t xStartCompressor_1,xStartCompressor_2;
TimerHandle_t xStartHeatPump_1,xStartHeatPump_2;
TimerHandle_t xStartThreeWay;
TimerHandle_t xStartRevValve_1,xStartRevValve_2;
TimerHandle_t xTStartCheckErrors_1,xTStartCheckErrors_2;
TimerHandle_t xTStartCheckTrv_1,xTStartCheckTrv_2;
//------stop

TimerHandle_t xSwitchRevOne_1_time,xSwitchRevOne_2_time;
TimerHandle_t xSwitchRevTwo_1_time,xSwitchRevTwo_2_time;
TimerHandle_t xStopHeatPump_1,xStopHeatPump_2;
TimerHandle_t xStopThreeWay;
TimerHandle_t xStopRevValve_1,xStopRevValve_2;
TimerHandle_t xStopSource_1,xStopSource_2;


TimerHandle_t xSwitchRev_1_time,xSwitchRev_2_time;


time_t timeStartComp_1,timeStartComp_2;
time_t timeStartHeatP_1,timeStartHeatP_2;
time_t timeStartThreeW;
time_t timeStartRevV_1,timeStartRevV_2;
time_t timeStartSrcP_1,timeStartSrcP_2;
#ifdef __cplusplus
}
#endif
#endif /* MODS_H_ */