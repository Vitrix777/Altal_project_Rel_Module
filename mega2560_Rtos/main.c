
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include "FreeRTOS.h"
#include  "task.h"
#include  "queue.h"
#include  "timers.h"
#include  "semphr.h"
#include  "croutine.h"
#include  "serial.h"
#include  "Sensors.h"
#include  "EEprom.h"
#include  "ModBus.h"
#include  "Mods.h"
#include  "Errors.h"
#include  "TRV.h"

static void ModBus(void *pvParameters);
static void System_1(void *pvParameters);
static void System_2(void *pvParameters);
static void Defrost(void *pvParameters);
//--------------------------------------------------------
TaskHandle_t  xHandleFirst = NULL,xHandleSecond = NULL;
//--------------------------------------------------------
xEEdata EEpromData;
xCompressor Comp_1,Comp_2;
time_t current_time;
xComPortHandle xSerialPort,xSerial3Port;
SemaphoreHandle_t xMbBinarySemaphore;
volatile bool port_0,port_3;
bool init_system,stop_second;
int _err1,_err2;
int old_corection_1,old_corection_2;
TickType_t countTrvTime_1=0,countTrvTime_2=0;
bool _heatWater=false;
bool _heat =false;
bool _cool =false;
uint8_t forsDefrost=0;
TickType_t timeTillDef;
TickType_t timeTillDef2;
//-------------------------------------------------------
//--------------------------------------------------------eeprom
/*
xEEdata EEpromData EEMEM ={
	7,//uint8_t Compressor_delay;
	7,//uint8_t Source_Pump_delay;
	7,//uint8_t Revers_Valve_delay;
	7,//uint8_t HeatingPump_delay;
	7,//uint8_t Three_Way_delay;
	40,//uint8_t TRV_Delay;
	// limits
	30,//uint8_t HP_Max;
	8,//uint8_t HP_Min;	
	10,//uint8_t LP_Max;
	//
	60,//uint8_t HEAT_Max;
	25,//uint8_t HEAT_Min;
	20,//uint8_t Source_Max;
	25,//uint8_t Cool_Max;
	2,//uint8_t Cool_Min;
	115,//uint8_t ExhaustMax;
	2,//uint8_t MinSuperheat;
	25,//uint8_t MaxSuperheat;
	2,//uint8_t MinSupercool;
	25,//uint8_t MaxSupercool;
	5,//uint8_t Delta_heating_min;
	11,//uint8_t Delta_heating_max;
	2,//uint8_t Delta_source_min;
	4,//uint8_t Delta_source_max;
	5,//uint8_t Delta_dhw_min;
	11,//uint8_t Delta_dhw_max;
	// set histeresis
	45,//uint8_t Set_Heating;
	25,//uint8_t Set_Cooling;
	45,//uint8_t Set_DHW;
	5,//uint8_t diffHEAT;
	5,//uint8_t diffCOOL;
	5,//uint8_t diffDHW;
	//--------------------
	0,//uint8_t AireToWater;
	1,//uint8_t NombComp;
	15,//uint8_t TimeDef;
	40,// TimeBetweenDef;
	89,// Humedity;

	//mods
	0,//uint8_t power;
	0,//uint8_t heatWater;
	0,//uint8_t heat;
	0,//uint8_t cool;
	0,//uint8_t reversMod;
	0,//uint8_t Power_380;
	0,//int  TempDef;
	0,//int  TempStopDef;
	0,//int  TempOut;
    2,//int Source_Min;
	//timers_1
	0,//int trv_corection_1;
	0,//unsigned int time_pump_hiter_1;
	0,//unsigned int time_pump_source_1;
	0,//unsigned int time_revers_1;
	0,//unsigned int time_compressor_1;
	0,//unsigned int time_threeway;	
	0,//unsigned int number_turnon_compressor_1;
	0,//unsigned int number_turnon_source_1;
	0,//unsigned int number_turnon_hiter_1;
	0,//unsigned int number_turnon_revers_1;
	0,//unsigned int number_turnon_threeway;
    // timers_2
    0,//unsigned int trv_corection_2;
    0,//unsigned int time_pump_hiter_2;
    0,// unsigned int time_pump_source_2;
    0,// unsigned int time_revers_2;
    0,//unsigned int time_compressor_2;
    0,//unsigned int number_turnon_compressor_2;
    0,//unsigned int number_turnon_source_2;
    0,//unsigned int number_turnon_hiter_2;
    0,//unsigned int number_turnon_revers_2;
    0,//unsigned int number_turnon_threeway_2;

	//states
    0.5//float LP_Min;
	
	
	};
*/	
//--------------------------------------------------------
volatile long  countTimer;
int main(void) __attribute__ ((OS_main));

int main(void)
{

	initPins();
	ADE7758_Init();
	timer_1_Init();
	Comp_1.Index=1;
	Comp_2.Index=2;
	//eeprom_write_block((void*)&EEpromData, ( void*)0, sizeof (xEEdata));
	current_time = time(NULL);
	xSerialPort = xSerialPortInitMinimal( USART0, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
	xSerial3Port = xSerialPortInitMinimal( USART3, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
	vSemaphoreCreateBinary(xMbBinarySemaphore);

    
	eeprom_read_block((void*)&EEpromData, (const void*)0, sizeof (xEEdata));
	activateTimers(&Comp_1);
	activateTimers(&Comp_2);
	activateTrvTimer(&Comp_1);
	activateTrvTimer(&Comp_2);
	init_system=true;
	old_corection_1=EEpromData.trv_corection_1;
	old_corection_2=EEpromData.trv_corection_2;
	Comp_1.modeOfPump=Comp_2.modeOfPump=selmode(&EEpromData);
	wdt_enable(WDTO_8S);
	//-------------------------------------
	sei(); 
	//-------------------------------------
	xTaskCreate(ModBus,(const portCHAR *)"ModBus",256,NULL,2, NULL );
	xTaskCreate(System_1,(const portCHAR *)"System_1",512,NULL,0, &xHandleFirst );
	xTaskCreate(System_2,(const portCHAR *)"System_2",512,NULL,0, &xHandleSecond );
	xTaskCreate(Defrost,(const portCHAR *)"Defrost",256,NULL,0, NULL );
	//--------------------------------------
	vTaskStartScheduler();
}

ISR(TIMER1_COMPA_vect) // timer_compare_a
{
	
	static BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	countTimer++;
	if(countTimer==7){stop_1_Timer();countTimer=0;xSemaphoreGiveFromISR(xMbBinarySemaphore,&xHigherPriorityTaskWoken);}
	//if( xHigherPriorityTaskWoken == pdTRUE )portYIELD();

}

//////////////////////////////////////////////////
static void Defrost(void *pvParameters)
{
	
while(1){
			wdt_reset();
			//xSerialxPrintf(&xSerial3Port,"Defrost_task\r\n");
			if(EEpromData.AireToWater==1){
				
				if(forsDefrost==1){startDefrost(&Comp_1,&EEpromData);Comp_1.def=true;forsDefrost=0;xSerialxPrintf(&xSerial3Port,"StartForceDEF\r\n");}
				if( !Comp_1.def && ((xTaskGetTickCount()-timeTillDef)>EEpromData.TimeBetweenDef *60000) && Comp_1.Src_In<EEpromData.TempDef && forsDefrost==0 ){startDefrost(&Comp_1,&EEpromData);Comp_1.def=true;xSerialxPrintf(&xSerial3Port,"StartDEF\r\n");}//
				if( Comp_1.def && ((xTaskGetTickCount()-timeTillDef)>EEpromData.TimeDef*60000) && Comp_1.stopdef ){stopDefrost(&Comp_1,&EEpromData);Comp_1.stopdef=Comp_1.def=false;timeTillDef=xTaskGetTickCount();xSerialxPrintf(&xSerial3Port,"StopDEF\r\n");}
				else if(Comp_1.def && Comp_1.Src_In>EEpromData.TempStopDef){stopDefrost(&Comp_1,&EEpromData);Comp_1.stopdef=Comp_1.def=false;timeTillDef=xTaskGetTickCount();xSerialxPrintf(&xSerial3Port,"StopDEF\r\n");}
	
			}
			//xSerialxPrintf(&xSerial3Port,"defrost\r\n");
vTaskDelay(2000);
//xSerialxPrintf(&xSerial3Port,"Stack_0 =%d\r\n",uxTaskGetStackHighWaterMark( NULL ));
//portYIELD();			
}
}
//////////////////////////////////////////////////
static void System_1(void *pvParameters)
{
	(void) pvParameters;
	
	
	while (1)
	{

		wdt_reset();
		//xSerialxPrintf(&xSerial3Port,"System_1_task\r\n");
vTaskSuspend( xHandleSecond );		
portENTER_CRITICAL();
ReadSensors(&Comp_1);
portEXIT_CRITICAL();
//EEpromData.AireToWater==1 &&

		if(EEpromData.NombComp==2 )
		{
			vTaskResume( xHandleSecond );
			stop_second=false;
			
		}
		
		 if(EEpromData.NombComp==1 && !stop_second){stop_second=true;Comp_Stop(&Comp_2);}
//xSerialxPrintf(&xSerial3Port,"error =%d\r\n",xTaskGetSchedulerState());
	
		if(EEpromData.power==1 && !Comp_1.suply && !Comp_1.switchToMode){Comp_1.suply=true;timeTillDef=xTaskGetTickCount();xSerialxPrintf(&xSerial3Port,"On_System_1\r\n");}//xTimerStart(xComutateTrv_1, 0);xTimerStart(xComutateTrv_1, 0);startTrvTimer(&Comp_1);
		else if(EEpromData.power==0 && Comp_1.suply){Comp_1.suply=false;PORTA &= 0x0;Comp_1.error=0;_err1=0;Comp_1.countError=0;xSerialxPrintf(&xSerial3Port,"OFF_System_1\r\n"); stopTrvTimer(&Comp_1);Comp_Stop(&Comp_1);}
		
		//xSerialxPrintf(&xSerial3Port,"mod =%d\r\n",EEpromData.AireToWater);
		//xSerialxPrintf(&xSerial3Port,"TimeBetweenDef =%d\r\n",EEpromData.TimeBetweenDef);
		
	
		
		if(Comp_1.suply &&  Comp_1.countError<3)
		{
			//>EEpromData.TimeBetweenDef EEpromData.TimeDef
			
			if(EEpromData.AireToWater==1){
			//xSerialxPrintf(&xSerial3Port,"forsDefrost =%d\r\n",forsDefrost);	
			if(forsDefrost==1){startDefrost(&Comp_1,&EEpromData);Comp_1.def=true;forsDefrost=0;xSerialxPrintf(&xSerial3Port,"StartForceDEF\r\n");}	
			if( !Comp_1.def && ((xTaskGetTickCount()-timeTillDef)>EEpromData.TimeBetweenDef *60000) && Comp_1.Src_In<EEpromData.TempDef && forsDefrost==0 ){startDefrost(&Comp_1,&EEpromData);Comp_1.def=true;xSerialxPrintf(&xSerial3Port,"StartDEF\r\n");}//
			if( Comp_1.def && ((xTaskGetTickCount()-timeTillDef)>EEpromData.TimeDef*60000) && Comp_1.stopdef ){stopDefrost(&Comp_1,&EEpromData);Comp_1.stopdef=Comp_1.def=false;timeTillDef=xTaskGetTickCount();xSerialxPrintf(&xSerial3Port,"StopDEF\r\n");}
			else if(Comp_1.def && Comp_1.Src_In>EEpromData.TempStopDef){stopDefrost(&Comp_1,&EEpromData);Comp_1.stopdef=Comp_1.def=false;timeTillDef=xTaskGetTickCount();xSerialxPrintf(&xSerial3Port,"StopDEF\r\n");}
			
			}
			
			if(!init_system){_err1=Check_Erors(&Comp_1,&EEpromData);ADE7758_resetStatus();} else init_system=false;
				
			//if(_err1>0 && Comp_1.error==0){Comp_1.error=_err1;Comp_1.countError++;Comp_1.Steps=0;Comp_Stop(&Comp_1);xSerialxPrintf(&xSerial3Port,"error =%d\r\n",Comp_1.error);}//
				
			 mod_pump(&Comp_1,&EEpromData);
			//xSerialxPrintf(&xSerial3Port,"error =%d\r\n",Comp_1.error);
			if(Comp_1.checkTrv && xTaskGetTickCount()-countTrvTime_1>10000 ){Comp_1.Steps = Fuzzy(&Comp_1,EEpromData.trv_corection_1);countTrvTime_1=xTaskGetTickCount();}
			
			
		}
		
		vTaskDelay(1000);
		//xSerialxPrintf(&xSerial3Port,"Stack1 =%d\r\n",uxTaskGetStackHighWaterMark( NULL ));
	}
}
///////////////////////////////////////////////////
static void System_2(void *pvParameters)
{
	(void) pvParameters;

	while (1)
	{
		
    	wdt_reset();
		//xSerialxPrintf(&xSerial3Port,"System_2_task\r\n");
vTaskSuspend( xHandleFirst );				
portENTER_CRITICAL();
ReadSensors(&Comp_2);
portEXIT_CRITICAL();
//xSerialxPrintf(&xSerial3Port,"task_2\r\n");
//xSerialxPrintf(&xSerial3Port,"TRVcorection2= %d \r\n",EEpromData.trv_corection_2);
vTaskResume( xHandleFirst );
		if(EEpromData.power==1 && !Comp_2.suply && !Comp_2.switchToMode){Comp_2.suply=true;xSerialxPrintf(&xSerial3Port,"On_System_2\r\n");}//xTimerStart(xComutateTrv_2, 0);startTrvTimer(&Comp_2);
		else if(EEpromData.power==0 && Comp_2.suply){Comp_2.suply=false;Comp_2.error=0;_err2=0;Comp_2.countError=0; xSerialxPrintf(&xSerial3Port,"OFF_System_2\r\n");stopTrvTimer(&Comp_2);Comp_Stop(&Comp_2);}
	
		if(Comp_2.suply && Comp_2.countError<3)
		{
			_err2=Check_Erors(&Comp_2,&EEpromData);
		
			//if(_err2>0 && Comp_1.error==0){Comp_2.error=_err2;Comp_2.countError++;Comp_2.Steps=0;Comp_Stop(&Comp_2);}//if(Comp_2.countError==3)EEpromData.power=0;
			mod_pump(&Comp_2,&EEpromData);
	       if(Comp_2.checkTrv && xTaskGetTickCount()-countTrvTime_2>10000 ){Comp_2.Steps = Fuzzy(&Comp_2,EEpromData.trv_corection_2);countTrvTime_2=xTaskGetTickCount();}
		}

		vTaskDelay(1000);
		//xSerialxPrintf(&xSerial3Port,"Stack_2 =%d\r\n",uxTaskGetStackHighWaterMark( NULL ));
		//portYIELD();
		//xSerialxPrintf(&xSerial3Port,"Stack2 =%d\r\n",uxTaskGetStackHighWaterMark( NULL ));
	}
}

//////////////////////////////////////////////////
static void ModBus(void *pvParameters)
{
	(void) pvParameters;

	while (1)
	{   
		//wdt_reset();
		xSerialxPrintf(&xSerial3Port,"ModBus_task\r\n");
		if (xSemaphoreTake(xMbBinarySemaphore, portMAX_DELAY) == pdTRUE )
		{
			modbus_update();
			//xSerialxPrintf(&xSerial3Port,"Interrupt\r\n");
			if(port_0){xSerialRxFlush( &xSerialPort );port_0=false;indexBuf=0;}
			else if(port_3){xSerialRxFlush( &xSerial3Port );port_3=false;indexBuf=0;}
			
		}
	//xSerialxPrintf(&xSerial3Port,"Stack_3 =%d\r\n",uxTaskGetStackHighWaterMark( NULL ));
	//vTaskDelay(1000);
	}
}

