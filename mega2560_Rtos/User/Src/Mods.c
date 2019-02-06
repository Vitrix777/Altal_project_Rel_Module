
#include  "Mods.h"


extern xCompressor Comp_1,Comp_2;
extern time_t current_time;
extern xEEdata EEpromData;
extern uint8_t modeOfPump;
extern int _err;
extern bool _heatWater;
extern bool _heat ;
extern bool _cool;
extern TickType_t timeTillDef;

const portBASE_TYPE uxStartDelayCompTimer_1_ID		= 1;
const portBASE_TYPE uxStartDelayHeatPTimer_1_ID		= 2;
const portBASE_TYPE uxStartDelaySourcePTimer_1_ID	= 3;
const portBASE_TYPE uxStartDelayRevVTimer_1_ID		= 4;
const portBASE_TYPE uxStartDelayThreeVTimer_1_ID	= 5 ;
const portBASE_TYPE uxStartDelayChekTrvTimer_1_ID	= 6 ;
const portBASE_TYPE uxStartDelayChekErrTimer_1_ID	= 7;
//----------------------------------------1 compressor stop delay
const portBASE_TYPE uxStopDelayHeatPTimer_1_ID		= 8;
const portBASE_TYPE uxStopDelaySourcePTimer_1_ID	= 9;
const portBASE_TYPE uxStopDelayRevVTimer_1_ID		= 10;
const portBASE_TYPE uxStopDelayThreeVTimer_1_ID		= 11;
const portBASE_TYPE uxSwitchRevOne_1_ID				= 12;
const portBASE_TYPE uxSwitchRevTwo_1_ID				= 13;
//--------------------------------------- 2 compressor start delay
const portBASE_TYPE uxStartDelayCompTimer_2_ID		= 14;
const portBASE_TYPE uxStartDelayHeatPTimer_2_ID		= 15 ;
const portBASE_TYPE uxStartDelaySourcePTimer_2_ID	= 16;
const portBASE_TYPE uxStartDelayRevVTimer_2_ID		= 17;
const portBASE_TYPE uxStartDelayThreeVTimer_2_ID	= 18;
const portBASE_TYPE uxStartDelayChekTrvTimer_2_ID	= 19;
const portBASE_TYPE uxStartDelayChekErrTimer_2_ID	= 20;
//-----------------------------------------2 compressor stop delay
const portBASE_TYPE uxStopDelayHeatPTimer_2_ID		= 21;
const portBASE_TYPE uxStopDelaySourcePTimer_2_ID	= 22;
const portBASE_TYPE uxStopDelayRevVTimer_2_ID		= 23;
const portBASE_TYPE uxSwitchRevOne_2_ID				= 24;
const portBASE_TYPE uxSwitchRevTwo_2_ID				= 25;

void activateTimers(xCompressor *comp)
{
	
	
	if(comp->Index==1)
	{
		
		xStartRevValve_1   =	xTimerCreate("StRevV_1",		7000/ portTICK_PERIOD_MS, pdFALSE, (void*) &uxStartDelayRevVTimer_1_ID, StartReversingValve);//(EEpromData.Revers_Valve_delay*1000) 
		xStartThreeWay     =	xTimerCreate("StThreeW",		7000/ portTICK_PERIOD_MS, pdFALSE, (void*) &uxStartDelayThreeVTimer_1_ID, StartThreeWay);//(EEpromData.Three_Way_delay*1000)
		xStartHeatPump_1   =	xTimerCreate("StHeatP_1",		7000/ portTICK_PERIOD_MS, pdFALSE, (void*) &uxStartDelayHeatPTimer_1_ID, StartHeatPump);//(EEpromData.HeatingPump_delay*1000) 
		xStartCompressor_1 =	xTimerCreate("StComp_1",		7000/ portTICK_PERIOD_MS, pdFALSE, (void*) &uxStartDelayCompTimer_1_ID, StartComp);//(EEpromData.Compressor_delay*1000)
		xTStartCheckErrors_1 =	xTimerCreate("ChErr_1",			300000 / portTICK_PERIOD_MS, pdFALSE, (void*) &uxStartDelayChekErrTimer_1_ID, StartCheckErrors);
		xTStartCheckTrv_1 =		xTimerCreate("ChTrv_1",			7000 / portTICK_PERIOD_MS, pdFALSE, (void*) &uxStartDelayChekTrvTimer_1_ID, StartCheckTrv);//(EEpromData.TRV_Delay*1000)
		//----------stop

		xSwitchRevOne_1_time =	xTimerCreate("swRevOne_1",		5000 / portTICK_PERIOD_MS, pdFALSE, (void*) &uxSwitchRevOne_1_ID, SwitchRev_1_Time);
		xSwitchRevTwo_1_time =	xTimerCreate("swRevTwo_1",		5000 / portTICK_PERIOD_MS, pdFALSE, (void*) &uxSwitchRevTwo_1_ID, SwitchRev_2_Time);
		xStopHeatPump_1   =		xTimerCreate("offHeatP_1",		7000/ portTICK_PERIOD_MS, pdFALSE, (void*) &uxStopDelayHeatPTimer_1_ID, HeatPump_Stop);//(EEpromData.HeatingPump_delay*1000)
		xStopThreeWay     =		xTimerCreate("offThreeW",		7000 / portTICK_PERIOD_MS, pdFALSE, (void*) &uxStopDelayThreeVTimer_1_ID, ThreeWay_Stop);//(EEpromData.Three_Way_delay*1000)
		xStopRevValve_1   =		xTimerCreate("offRevV_1",		7000/ portTICK_PERIOD_MS, pdFALSE, (void*) &uxStopDelayRevVTimer_1_ID, Revers_Stop);//(EEpromData.Revers_Valve_delay*1000)
		xStopSource_1   =		xTimerCreate("offSourceP_1",	7000 / portTICK_PERIOD_MS, pdFALSE, (void*) &uxStopDelaySourcePTimer_1_ID, SorcePump_Stop);//(EEpromData.Source_Pump_delay*1000)
	}
	if (comp->Index==2)
	{
		
		xStartRevValve_2   =	xTimerCreate("StRevV_2",		7000, pdFALSE, (void*) &uxStartDelayRevVTimer_2_ID, StartReversingValve);
		xStartHeatPump_2   =	xTimerCreate("StHeatP_2",		7000, pdFALSE, (void*) &uxStartDelayHeatPTimer_2_ID, StartHeatPump);
		xStartCompressor_2 =	xTimerCreate("StComp_2",		7000, pdFALSE, (void*) &uxStartDelayCompTimer_2_ID, StartComp);
		xTStartCheckErrors_2 =	xTimerCreate("ChErr_2",			300000, pdFALSE, (void*) &uxStartDelayChekErrTimer_2_ID, StartCheckErrors);
		xTStartCheckTrv_2 =		xTimerCreate("ChTrv_2",			7000, pdFALSE, (void*) &uxStartDelayChekTrvTimer_2_ID, StartCheckTrv);
		//---------stop
		xSwitchRevOne_2_time =	xTimerCreate("swRevOne_2",		5000, pdFALSE, (void*) &uxSwitchRevOne_2_ID, SwitchRev_1_Time);
		xSwitchRevTwo_2_time =	xTimerCreate("swRevTwo_2",		5000, pdFALSE, (void*) &uxSwitchRevTwo_2_ID, SwitchRev_2_Time);
		xStopHeatPump_2   =		xTimerCreate("offHeatP_2",		7000, pdFALSE, (void*) &uxStopDelayHeatPTimer_2_ID, HeatPump_Stop);
		xStopRevValve_2    =	xTimerCreate("offRevV_2",		7000, pdFALSE, (void*) &uxStopDelayRevVTimer_2_ID, Revers_Stop);
		xStopSource_2   =		xTimerCreate("offSourceP_2",	7000, pdFALSE, (void*) &uxStopDelaySourcePTimer_2_ID, SorcePump_Stop);
	}
	
	
}
//------------------------------------------------
void startDefrost(xCompressor *comp,xEEdata* eeData){
	_heatWater=eeData->heatWater;
	_heat =eeData->heat;
	_cool =eeData->cool;
	eeData->heatWater=false;
	eeData->heat =false;
	eeData->cool =true;
	//Comp_Stop(comp);
		
}
void stopDefrost(xCompressor *comp,xEEdata* eeData){
	
	//Comp_Stop(comp);
	//eeData->heatWater=false;
	//eeData->heat =true;
	//eeData->cool =false;
	eeData->heatWater=_heatWater;
	eeData->heat =_heat;
	eeData->cool =_cool;

}





//------------------------------------------------
void StarSoursePump(xCompressor *comp){
TickType_t period;

	period=(TickType_t)EEpromData.Revers_Valve_delay;
	period*=1000;
	xSerialxPrintf(&xSerial3Port,"SoursePump_f_%d\r\n",comp->Index);
	if(comp->Index == 1)
	{
		
		Comp_1.error=0;
		Comp_1.switchToMode=true;
		if(PIN_SOURCE_PUMP_1 == 0 && !comp->def )
		{
			START_SOURCE_PUMP_1;
			xSerialxPrintf(&xSerial3Port,"StartSourcePump_1\r\n");
			comp->srcPOn=1;
			timeStartSrcP_1=time(&current_time);
			EEpromData.number_turnon_source_1+=1;
			//xSerialxPrintf(&xSerial3Port,"Number_turnon_source_=%d\r\n",EEpromData.number_turnon_source_1);
			
						
		}
		xTimerChangePeriod( xStartRevValve_1,period, 100 );
	}
	if(comp->Index == 2)
	{
		Comp_2.error=0;
		Comp_1.switchToMode=true;
		
		xSerialxPrintf(&xSerial3Port,"StartSourcePump_2\r\n");
		
		if(PIN_SOURCE_PUMP_2 == 0 && !comp->def)
		{
			START_SOURCE_PUMP_2;						
			comp->srcPOn=1;
			timeStartSrcP_2=time(&current_time);
			EEpromData.number_turnon_source_2+=1;
						
		}
		xTimerChangePeriod( xStartRevValve_2,period, 100 );
	}
	
}
//------------------------------------------------
void StartReversingValve(TimerHandle_t xTimer){
	TickType_t period;
	static time_t time_count_1;
	static time_t time_count_2;
	portBASE_TYPE *pxTimerID;
	pxTimerID = pvTimerGetTimerID(xTimer);
	
	if((*pxTimerID) == uxStartDelayRevVTimer_1_ID)
	{	period=(TickType_t)EEpromData.Three_Way_delay;
		period*=1000;
		period+=10000;
		//////////////////////////////////////////

     //xSerialxPrintf(&xSerial3Port,"StartReversingValve\r\n");

		if(Comp_1.Cooling  )
		{
			
			if(PIN_REV_VALVE_1 >0 && EEpromData.reversMod==1){
				
				STOP_REV_VALVE_1;//
				xSerialxPrintf(&xSerial3Port,"StopRevValve_1\r\n");
				Comp_1.revVOn=0;
				time_count_1+= difftime(time(&current_time), timeStartRevV_1);
				if(time_count_1>ONE_HOUR){EEpromData.time_revers_1++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_1=0;}
				
				
			}
			else if(PIN_REV_VALVE_1 == 0 && EEpromData.reversMod==0){
				START_REV_VALVE_1;
				Comp_1.revVOn=1;
				xSerialxPrintf(&xSerial3Port,"StarRevValve_1\r\n");
				timeStartRevV_1=time(&current_time);
				EEpromData.number_turnon_revers_1+=1;
			}
		}
		if(Comp_1.heatingWater || Comp_1.Heating)
		{
			
			if(PIN_REV_VALVE_1 > 0 && EEpromData.reversMod==0){//
				
				STOP_REV_VALVE_1;
				
				xSerialxPrintf(&xSerial3Port,"StopRevValve_1\r\n");
				Comp_1.revVOn=0;
				time_count_1+= difftime(time(&current_time), timeStartRevV_1);
				if(time_count_1>ONE_HOUR){EEpromData.time_revers_1++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_1=0;}
				
			}
			else if(PIN_REV_VALVE_1 == 0 && EEpromData.reversMod==1){
				START_REV_VALVE_1;
				Comp_1.revVOn=1;
				xSerialxPrintf(&xSerial3Port,"StopRevValve_1\r\n");
				timeStartRevV_1=time(&current_time);
				EEpromData.number_turnon_revers_1+=1;
			}
		}
		
		xTimerChangePeriod( xStartThreeWay,period, 100 );
		
	}
	else if((*pxTimerID) == uxStartDelayRevVTimer_2_ID)
	{
		period=(TickType_t)EEpromData.HeatingPump_delay;
		period*=1000;
		
		if(Comp_2.Cooling)
		{
			if(PIN_REV_VALVE_2 >0 && EEpromData.reversMod==1){
				
				STOP_REV_VALVE_2;//
				
				xSerialxPrintf(&xSerial3Port,"StopRevValve_2\r\n");
				Comp_2.revVOn=0;
				time_count_2+= difftime(time(&current_time), timeStartRevV_2);
				if(time_count_2>ONE_HOUR){EEpromData.time_revers_2++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_2=0;}
				
				
			}
			else if(PIN_REV_VALVE_2 == 0 && EEpromData.reversMod==0){
				START_REV_VALVE_2;
				Comp_1.revVOn=1;
				xSerialxPrintf(&xSerial3Port,"StarRevValve_2\r\n");
				timeStartRevV_2=time(&current_time);
				EEpromData.number_turnon_revers_2+=1;
			}
		}
		if(Comp_2.Heating)
		{
			if(PIN_REV_VALVE_2 > 0 && EEpromData.reversMod==0){//
				
				STOP_REV_VALVE_2;
				
				xSerialxPrintf(&xSerial3Port,"StopRevValve_2\r\n");
				Comp_2.revVOn=0;
				time_count_2+= difftime(time(&current_time), timeStartRevV_2);
				if(time_count_2>ONE_HOUR){EEpromData.time_revers_2++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_2=0;}
				
			}
			else if(PIN_REV_VALVE_2 == 0 && EEpromData.reversMod==1){
				START_REV_VALVE_2;
				Comp_2.revVOn=1;
				xSerialxPrintf(&xSerial3Port,"StopRevValve_2\r\n");
				timeStartRevV_2=time(&current_time);
				EEpromData.number_turnon_revers_2+=1;
			}
		}

		xTimerChangePeriod( xStartHeatPump_2,period, 100 );
		
	}


}
//------------------------------------------------
void StartThreeWay()
{
	static time_t time_count;
    TickType_t period;
	
	
	period=(TickType_t)EEpromData.HeatingPump_delay;
	period*=1000;
	xSerialxPrintf(&xSerial3Port,"StartThreeWay\r\n");
	if(Comp_1.heatingWater)
	{
		
		if(PIN_THWAY_VALVE == 0)//
		{
			START_THWAY_VALVE;
			
			//xSerialxPrintf(&xSerial3Port,"StartThreeWay\r\n");
			Comp_1.threeWOn=1;
			timeStartThreeW=time(&current_time);
			EEpromData.number_turnon_threeway+=1;
			
		} else xSerialxPrintf(&xSerial3Port,"StopThreeWay\r\n");
	}
	if(Comp_1.Heating || Comp_1.Cooling)
	{
		if(PIN_THWAY_VALVE > 0)//
		{
			STOP_THWAY_VALVE;
			
			//xSerialxPrintf(&xSerial3Port,"StopThreeWay\r\n");
			Comp_1.threeWOn=0;
			time_count+= difftime(time(&current_time), timeStartThreeW);
			if(time_count>ONE_HOUR){EEpromData.time_threeway++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count=0;}
		}
	}
	
	xTimerChangePeriod( xStartHeatPump_1,period, 100 );
	
}

//------------------------------------------------

void StartHeatPump(TimerHandle_t xTimer)
{
	portBASE_TYPE *pxTimerID;
	pxTimerID = pvTimerGetTimerID(xTimer);
	TickType_t period;
	period=(TickType_t)EEpromData.Compressor_delay;
	period*=1000;
	//xSerialxPrintf(&xSerial3Port,"StartHeatPump\r\n");
	if((*pxTimerID) == uxStartDelayHeatPTimer_1_ID) {
		xSerialxPrintf(&xSerial3Port,"StartHeatPump_1\r\n");
		if(PIN_HEAT_PUMP_1 ==0)
		{
			
			//xSerialxPrintf(&xSerial3Port,"StartHeatPump_1\r\n");
			Comp_1.heatPOn=1;
			START_HEAT_PUMP_1;
			timeStartHeatP_1=time(&current_time);
			EEpromData.number_turnon_hiter_1+=1;			
			xTimerChangePeriod( xStartCompressor_1,period, 100 );
			
		}
	}

	else if((*pxTimerID) == uxStartDelayHeatPTimer_2_ID) {
		xSerialxPrintf(&xSerial3Port,"StartHeatPump_2\r\n");
		if(PIN_HEAT_PUMP_2 == 0)
		{
			Comp_2.heatPOn=2;
			START_HEAT_PUMP_2;
			timeStartHeatP_2=time(&current_time);
			EEpromData.number_turnon_hiter_2+=1;
			xTimerChangePeriod( xStartCompressor_2,period, 100 );
		}
		
	}
}


void StartComp(TimerHandle_t xTimer){
	
	portBASE_TYPE *pxTimerID;
	
	TickType_t period;	
	period=(TickType_t)EEpromData.TRV_Delay;
    period*=1000;
	
	pxTimerID = pvTimerGetTimerID(xTimer);
//xSerialxPrintf(&xSerial3Port,"StartCompressor\r\n");
	if((*pxTimerID) == uxStartDelayCompTimer_1_ID) {
		Comp_1.switchToMode=false;
		 if(Comp_1.def) {timeTillDef=xTaskGetTickCount();Comp_1.stopdef=true;}
		if(PIN_COMPRESSOR_1 == 0)
		{   
			
			Comp_1.compOn=1;
			//if(Comp_2.compOn)vTaskDelay(5000);
			START_COMPRESSOR_1;
			
			xSerialxPrintf(&xSerial3Port,"StartCompressor_1\r\n");
			
			
			timeStartComp_1=time(&current_time);
			EEpromData.number_turnon_compressor_1+=1;
			//xSerialxPrintf(&xSerial3Port,"Number_turnon_Compressor_=%d\r\n",EEpromData.number_turnon_compressor_1);
			xTimerChangePeriod( xTStartCheckTrv_1,period, 100 );
			xTimerStart(xTStartCheckErrors_1, 0);
	
		}
	}
	else if((*pxTimerID) == uxStartDelayCompTimer_2_ID) {
		xSerialxPrintf(&xSerial3Port,"StartCompressor_2\r\n");
		if(PIN_COMPRESSOR_2 == 0)
		{
			//if(Comp_1.compOn)vTaskDelay(5000);
			Comp_2.compOn=1;
			START_COMPRESSOR_2;
			
			
			Comp_2.switchToMode=false;
			
			timeStartComp_2=time(&current_time);
			EEpromData.number_turnon_compressor_2+=1;
			xTimerChangePeriod( xTStartCheckTrv_2,period, 100 );
			xTimerStart(xTStartCheckErrors_2, 0);
		}
	}
	
	
}
//-------------------------------------------------------------------------Stop mod
void Comp_Stop(xCompressor *comp){
	static time_t time_count_1;
	static time_t time_count_2;
	
	//xSerialxPrintf(&xSerial3Port,"StopCompressor\r\n");
	stop_timers(comp);
	comp->Cooling=comp->heatingWater=comp->Heating=false;
    comp->switchToMode=true;
	if(comp->Index == 1)
	{
		if(PIN_COMPRESSOR_1 > 0)
		{
			STOP_COMPRESSOR_1;
			xSerialxPrintf(&xSerial3Port,"StopCompressor_1\r\n");
			comp->checkErrors =false;
			
			comp->checkTrv =false;
			comp->compOn=0;
			time_count_1+= difftime(time(&current_time), timeStartComp_1);
			
			//xSerialxPrintf(&xSerial3Port,"time_Compressor_=%d\r\n",time_count_1);
			if(time_count_1>ONE_HOUR){EEpromData.time_compressor_1++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_1=0;}
			
		}
		xTimerStart(xSwitchRevOne_1_time, 0);
		
	}
	else if(comp->Index == 2)
	{
		if(PIN_COMPRESSOR_2 > 0)
		{
			
			STOP_COMPRESSOR_2;
			xSerialxPrintf(&xSerial3Port,"StopCompressor_2\r\n");
			comp->checkErrors =false;
			comp->switchToMode=true;
			comp->checkTrv =false;
			comp->compOn=0;
			time_count_2+= difftime(time(&current_time), timeStartComp_2);
			if(time_count_2>ONE_HOUR){EEpromData.time_compressor_2++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_2=0;}
			
		}
		xTimerStart(xSwitchRevOne_2_time, 0);
	}
	
	
	
}
//-------------------------------------------------------------------------
void SwitchRev_1_Time(TimerHandle_t xTimer)
{
	portBASE_TYPE *pxTimerID;
	pxTimerID = pvTimerGetTimerID(xTimer);
	
	//xSerialxPrintf(&xSerial3Port,"SwitchRev_1\r\n");
	
	if((*pxTimerID) == uxSwitchRevOne_1_ID)
	{
		xSerialxPrintf(&xSerial3Port,"SwitchRev_ON_1_1\r\n");
		if(PIN_REV_VALVE_1 == 0){START_REV_VALVE_1;}//
		else {STOP_REV_VALVE_1;}//xSerialxPrintf(&xSerial3Port,"SwitchRev_OFF_1\r\n");
		xTimerStart(xSwitchRevTwo_1_time, 0);
	}
	if((*pxTimerID) == uxSwitchRevOne_2_ID)
	{
		xSerialxPrintf(&xSerial3Port,"SwitchRev_2_1\r\n");
		
		if(PIN_REV_VALVE_2 == 0)START_REV_VALVE_2;
		else STOP_REV_VALVE_2;
		xTimerStart(xSwitchRevTwo_2_time, 0);
	}
}
//-------------------------------------------------------------------------
void SwitchRev_2_Time(TimerHandle_t xTimer)
{
	portBASE_TYPE *pxTimerID;
	
	TickType_t period;
	period=(TickType_t)EEpromData.HeatingPump_delay;
	period*=1000;
	pxTimerID = pvTimerGetTimerID(xTimer);
	
	//xSerialxPrintf(&xSerial3Port,"SwitchRev_2\r\n");
	
	if((*pxTimerID) == uxSwitchRevTwo_1_ID)
	{
		xSerialxPrintf(&xSerial3Port,"SwitchRev_ON_1_2\r\n");
		if(PIN_REV_VALVE_1 == 0){START_REV_VALVE_1;}//
		else {STOP_REV_VALVE_1;}//xSerialxPrintf(&xSerial3Port,"SwitchRev_OFF_2\r\n");
		xTimerChangePeriod( xStopHeatPump_1,period, 100 );
		
	}
	if((*pxTimerID) == uxSwitchRevTwo_2_ID)
	{
		//xSerialxPrintf(&xSerial3Port,"SwitchRev_2_2\r\n");
		if(PIN_REV_VALVE_2 == 0) START_REV_VALVE_2;
		else STOP_REV_VALVE_2;
		xTimerChangePeriod( xStopHeatPump_2,period, 100 );
		
	}
}
//-------------------------------------------------------------------------
void HeatPump_Stop(TimerHandle_t xTimer)
{
	static time_t time_count_1;
	static time_t time_count_2;
	portBASE_TYPE *pxTimerID;
	
	TickType_t period;
	
	//xSerialxPrintf(&xSerial3Port,"StopHeatPump\r\n");
	
	pxTimerID = pvTimerGetTimerID(xTimer);

	if((*pxTimerID) == uxStopDelayHeatPTimer_1_ID)
	{
		period=(TickType_t)EEpromData.Three_Way_delay;
		period*=1000;
		
		if(PIN_HEAT_PUMP_1 > 0)//
		{
			STOP_HEAT_PUMP_1;
			xSerialxPrintf(&xSerial3Port,"StopHeatPump_1\r\n");
			
			Comp_1.heatPOn=0;
			time_count_1+= difftime(time(&current_time), timeStartHeatP_1);
			if(time_count_1>ONE_HOUR){EEpromData.time_pump_hiter_1++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_1=0;}
			
		}
		xTimerChangePeriod( xStopThreeWay,period, 100 );
		//xTimerStart(xStopThreeWay, 0);
	}

	else if((*pxTimerID) == uxStopDelayHeatPTimer_2_ID)
	{
		period=(TickType_t)EEpromData.Revers_Valve_delay;
		period*=1000;
		xSerialxPrintf(&xSerial3Port,"StopHeatPump_2\r\n");
		if(PIN_HEAT_PUMP_2 > 0)//
		{
			STOP_HEAT_PUMP_2;
			
			
			Comp_2.heatPOn=0;
			time_count_2+= difftime(time(&current_time), timeStartHeatP_2);
			if(time_count_2>ONE_HOUR){EEpromData.time_pump_hiter_2++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_2=0;}
			
		}
		xTimerChangePeriod( xStopRevValve_2,(EEpromData.Revers_Valve_delay*1000) / portTICK_PERIOD_MS, 100 );
		
	}
}
//-------------------------------------------------------------------------
void ThreeWay_Stop()
{
	static time_t time_count;
	TickType_t period;
	period=(TickType_t)EEpromData.Revers_Valve_delay;
	period*=1000;
	
	xSerialxPrintf(&xSerial3Port,"ThreeWay_Stop\r\n");
	if(PIN_THWAY_VALVE > 0)
	{
		STOP_THWAY_VALVE;
		//xSerialxPrintf(&xSerial3Port,"ThreeWay_Stop\r\n");
		Comp_1.threeWOn=0;
		time_count+= difftime(time(&current_time), timeStartThreeW);
		if(time_count>ONE_HOUR){EEpromData.time_threeway++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count=0;}
		
	} else{ }//xSerialxPrintf(&xSerial3Port,"ThreeWay_isRun\r\n");
	xTimerChangePeriod( xStopRevValve_1,period, 100 );
	
}

//-------------------------------------------------------------------------
void Revers_Stop(TimerHandle_t xTimer)
{
	static time_t time_count_1;
	static time_t time_count_2;
	TickType_t period;
	period=(TickType_t)EEpromData.Source_Pump_delay;
	period*=1000;

	portBASE_TYPE *pxTimerID;
	pxTimerID = pvTimerGetTimerID(xTimer);
	
   // xSerialxPrintf(&xSerial3Port,"StopRevValve\r\n");

	if((*pxTimerID) == uxStopDelayRevVTimer_1_ID)
	{
		
		
		if(PIN_REV_VALVE_1 > 0)//
		{
			STOP_REV_VALVE_1;
			xSerialxPrintf(&xSerial3Port,"StopRevValve_1\r\n");
			
			Comp_1.revVOn=0;
			time_count_1+= difftime(time(&current_time), timeStartRevV_1);
			if(time_count_1>ONE_HOUR){EEpromData.time_revers_1++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_1=0;}

		}
		xTimerChangePeriod( xStopSource_1,period, 100 );
		//xTimerStart(xStopSource_1, 0);
	}
	
	else if((*pxTimerID) == uxStopDelayRevVTimer_2_ID)
	{
		
		if(PIN_REV_VALVE_2 > 0)//
		{   
			xSerialxPrintf(&xSerial3Port,"StopRevValve_2\r\n");
			STOP_REV_VALVE_2;
			
			
			Comp_2.revVOn=0;
			time_count_2+= difftime(time(&current_time), timeStartRevV_2);
			if(time_count_2>ONE_HOUR){EEpromData.time_revers_2++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_2=0;}

		}
		xTimerChangePeriod( xStopSource_2,period, 100 );
		//xTimerStart(xStopSource_2, 0);
	}
	
	
}
//---------------------------------------------------------------------------
void SorcePump_Stop(TimerHandle_t xTimer)
{
	static time_t time_count_1;
	static time_t time_count_2;
	portBASE_TYPE *pxTimerID;
	pxTimerID = pvTimerGetTimerID(xTimer);
	
     // xSerialxPrintf(&xSerial3Port,"StopSource\r\n");
    
	 	
	 
	if((*pxTimerID) == uxStopDelaySourcePTimer_1_ID)
	{
		Comp_1.switchToMode=false ;
		
		
		if(PIN_SOURCE_PUMP_1 > 0)//
		{
			STOP_SOURCE_PUMP_1;
			xSerialxPrintf(&xSerial3Port,"StopSource_1\r\n");
			
			Comp_1.srcPOn=0;
			
			
			time_count_1+= difftime(time(&current_time), timeStartSrcP_1);
			//xSerialxPrintf(&xSerial3Port,"time_count_1=%d\r\n",time_count_1);
			if(time_count_1>ONE_HOUR){EEpromData.time_pump_source_1++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_1=0;}

		}
		
	}
	
	else if((*pxTimerID) == uxStopDelaySourcePTimer_2_ID)
	{
		//xSerialxPrintf(&xSerial3Port,"StopSource_2\r\n");
		if(PIN_SOURCE_PUMP_2 > 0)//
		{
			STOP_SOURCE_PUMP_2;
			xSerialxPrintf(&xSerial3Port,"StopSource_2\r\n");
			Comp_2.srcPOn=0;
			Comp_2.switchToMode=false;
			time_count_2+= difftime(time(&current_time), timeStartSrcP_2);
			if(time_count_2>ONE_HOUR){EEpromData.time_pump_source_2++;eeprom_update_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));time_count_2=0;}

		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StartCheckErrors(TimerHandle_t xTimer)
{
	portBASE_TYPE *pxTimerID;
	pxTimerID = pvTimerGetTimerID(xTimer);
	
	
	
	if((*pxTimerID) == uxStartDelayChekErrTimer_1_ID){Comp_1.countError=0; Comp_1.checkErrors=true;xSerialxPrintf(&xSerial3Port,"StartCheckErrors_1\r\n");}
	else if((*pxTimerID) == uxStartDelayChekErrTimer_2_ID){Comp_2.countError=0;Comp_2.checkErrors=true;xSerialxPrintf(&xSerial3Port,"StartCheckErrors_2\r\n");}
	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StartCheckTrv(TimerHandle_t xTimer)
{
	portBASE_TYPE *pxTimerID;
	pxTimerID = pvTimerGetTimerID(xTimer);
	if((*pxTimerID) == uxStartDelayChekTrvTimer_1_ID)       {Comp_1.checkTrv=true;xSerialxPrintf(&xSerial3Port,"StartCheckTrv_1\r\n");}
	else if((*pxTimerID) == uxStartDelayChekTrvTimer_2_ID) {Comp_2.checkTrv=true;xSerialxPrintf(&xSerial3Port,"StartCheckTrv_2\r\n");}
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t selmode(xEEdata* eeData)
{

	if(eeData->heatWater && !eeData->heat && !eeData->cool)
	{
		return DHW;
		
	}
	if(!eeData->heatWater && eeData->heat && !eeData->cool)
	{
		return HEAT;
	}
	if(!eeData->heatWater && !eeData->heat && eeData->cool)
	{
		return COOL;
	}
	if(eeData->heatWater && eeData->heat && !eeData->cool)
	{
		return HEAT_DHW;
	}
	if(eeData->heatWater && !eeData->heat && eeData->cool)
	{
		return COOL_DHW;
	}
	else return OFF;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void goToMode(xCompressor *comp)
{   //calibration(comp);
	stop_timers(comp);
	startTrvTimer(comp);
	//comp->switchToMode=true;
	
	StarSoursePump(comp);
	//Comp_1.Steps = Fuzzy(&Comp_1,EEpromData.trv_corection_1);
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void mod_pump( xCompressor *comp,xEEdata* eeData){

	uint8_t temp;
	temp = selmode(&EEpromData);
	if(temp!=comp->modeOfPump ){comp->modeOfPump=temp; Comp_Stop(comp);}

	switch(temp){
		case DHW: {
		if(comp->Index==1){
			if ( comp->Dhw_Temp > eeData->Set_DHW && comp->heatingWater && !comp->switchToMode) {comp->heatingWater=false;Comp_Stop(comp);stopTrvTimer(comp);}
			if ( comp->Dhw_Temp < (eeData->Set_DHW-eeData->diffDHW) && !comp->heatingWater && !comp->switchToMode){calibration(comp);Comp_1.error=0; comp->heatingWater=true; goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_Dhw\r\n");}//Start_Dhw();
			}
		}break;
		
		case HEAT:{
			//xSerialxPrintf(&xSerial3Port,"Set_Heating =%d\r\n,diffHEAT =%d\r\n,Bac_Temp =%f\r\n",eeData->Set_Heating,eeData->diffHEAT,data->Bac_Temp);
			//xSerialxPrintf(&xSerial3Port,"diffHEAT =%d\r\n",eeData->diffHEAT);
			//xSerialxPrintf(&xSerial3Port,"heat mode\r\n");
			if (comp->Bac_Temp < (eeData->Set_Heating-eeData->diffHEAT) && !comp->Heating && !comp->switchToMode) {comp->Heating=true;calibration(comp);Comp_1.error=0;goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_heat\r\n");}
			if (comp->Bac_Temp > eeData->Set_Heating && comp->Heating && !comp->switchToMode) {comp->Heating=false;Comp_Stop(comp);stopTrvTimer(comp);}
		} break;
		
		case COOL:{
			//xSerialxPrintf(&xSerial3Port,"COOL mode\r\n");
			if (comp->Bac_Temp < eeData->Set_Cooling && comp->Cooling && !comp->switchToMode ){comp->Cooling=false;Comp_Stop(comp);stopTrvTimer(comp);}//
			if (comp->Bac_Temp  > (eeData->Set_Cooling + eeData->diffCOOL) && !comp->Cooling  && !comp->switchToMode ) {comp->Cooling=true;calibration(comp);Comp_1.error=0;goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_cool\r\n");}//
			

		}break;
		
		case HEAT_DHW:
		
		{
			if(comp->Index==1)
			{
				if( comp->Dhw_Temp< (eeData->Set_DHW-eeData->diffDHW) && !comp->heatingWater && !comp->Heating && !comp->switchToMode ) {comp->heatingWater=true;calibration(comp);Comp_1.error=0;goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_dhw_1\r\n"); }//avrSerialPrintf("Start_dhwheat_1\r\n" );
				else if( comp->Bac_Temp < (eeData->Set_Heating-eeData->diffHEAT) && !comp->heatingWater && !comp->Heating && !comp->switchToMode ) {calibration(comp);Comp_1.error=0;comp->Heating=true;goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_heat_1\r\n");}//avrSerialPrintf("Start_heatdhw_1\r\n" );
				else if( comp->Dhw_Temp>eeData->Set_DHW && comp->heatingWater && !comp->Heating && !comp->switchToMode ) {comp->heatingWater=false;Comp_Stop(comp);stopTrvTimer(comp); }//
				else if (comp->Bac_Temp>eeData->Set_Heating && !comp->heatingWater && comp->Heating   && !comp->switchToMode  ) {comp->Heating=false;Comp_Stop(comp);stopTrvTimer(comp); }//
				if(EEpromData.NombComp==2) if(comp->Dhw_Temp< (eeData->Set_DHW-eeData->diffDHW)&& !comp->heatingWater && comp->Heating && !comp->switchToMode){comp->Heating=false;Comp_Stop(comp);stopTrvTimer(comp);xSerialxPrintf(&xSerial3Port,"Start_dhw_again\r\n");}	
			}
			else if(comp->Index==2)
			{
				if (comp->Bac_Temp < (eeData->Set_Heating-eeData->diffHEAT) && !comp->Heating && !comp->switchToMode) {calibration(comp);comp->Heating=true;goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_heat_2\r\n");}
				if (comp->Bac_Temp > eeData->Set_Heating && comp->Heating && !comp->switchToMode) {comp->Heating=false;Comp_Stop(comp);stopTrvTimer(comp);}
			}
			
		}break;
		
		case COOL_DHW:
		{
			if(comp->Index==1)
			{
				if( comp->Dhw_Temp< (eeData->Set_DHW-eeData->diffDHW) && !comp->heatingWater && !comp->Cooling && !comp->switchToMode ) {comp->heatingWater=true;calibration(comp);Comp_1.error=0;comp->heatingWater=true;goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_dhwcool_1\r\n");}//
				else if( comp->Bac_Temp < (eeData->Set_Heating-eeData->diffHEAT) && !comp->heatingWater && !comp->Cooling && !comp->switchToMode ) {calibration(comp);Comp_1.error=0;comp->Cooling=true;goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_cooldhw_1\r\n");}//
				else if( comp->Dhw_Temp>eeData->Set_DHW && comp->heatingWater && !comp->Cooling && !comp->switchToMode ) {comp->heatingWater=false;Comp_Stop(comp); stopTrvTimer(comp);}//
				else if (comp->Bac_Temp>eeData->Set_Heating && !comp->heatingWater && comp->Cooling  && !comp->switchToMode  ) {comp->Cooling=false;Comp_Stop(comp); stopTrvTimer(comp);}//
				if(EEpromData.NombComp==2) if (comp->Dhw_Temp< (eeData->Set_DHW-eeData->diffDHW) && !comp->heatingWater && comp->Cooling && !comp->switchToMode  ) {comp->Cooling=false;Comp_Stop(comp); stopTrvTimer(comp);}	
			}
			else if(comp->Index==2)
			{
				if (comp->Bac_Temp < eeData->Set_Cooling && comp->Cooling && !comp->switchToMode ){comp->Cooling=false;Comp_Stop(comp);stopTrvTimer(comp);}//
				if (comp->Bac_Temp  > (eeData->Set_Cooling + eeData->diffCOOL) && !comp->Cooling  && !comp->switchToMode ) {calibration(comp);comp->Cooling=true;goToMode(comp);xSerialxPrintf(&xSerial3Port,"Start_cool_1\r\n");}//
			}
		}break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void stop_timers( xCompressor *comp){
	if(comp->Index==1)
	{
		if( xTimerIsTimerActive( xStartCompressor_1 ) != pdFALSE )      xTimerStop(xStartCompressor_1,1);
		if( xTimerIsTimerActive( xStartHeatPump_1 ) != pdFALSE )        xTimerStop(xStartHeatPump_1,1);
		if( xTimerIsTimerActive( xStartThreeWay ) != pdFALSE )			xTimerStop(xStartThreeWay,1);
		if( xTimerIsTimerActive( xStartRevValve_1 ) != pdFALSE )		xTimerStop(xStartRevValve_1,1);
		if( xTimerIsTimerActive( xSwitchRevOne_1_time ) != pdFALSE )    xTimerStop(xSwitchRevOne_1_time,1);
		if( xTimerIsTimerActive( xSwitchRevTwo_1_time ) != pdFALSE )	xTimerStop(xSwitchRevTwo_1_time,1);
		if( xTimerIsTimerActive( xStopHeatPump_1 ) != pdFALSE )			xTimerStop(xStopHeatPump_1,1);
		if( xTimerIsTimerActive( xStopThreeWay ) != pdFALSE )			xTimerStop(xStopThreeWay,1);
		if( xTimerIsTimerActive( xStopRevValve_1 ) != pdFALSE )			xTimerStop(xStopRevValve_1,1);
		if( xTimerIsTimerActive( xStopSource_1 ) != pdFALSE )			xTimerStop(xStopSource_1,1);
		if( xTimerIsTimerActive( xTStartCheckErrors_1 ) != pdFALSE )    xTimerStop(xTStartCheckErrors_1,1);
		if( xTimerIsTimerActive( xTStartCheckTrv_1 ) != pdFALSE )       xTimerStop(xTStartCheckTrv_1,1);
		if( xTimerIsTimerActive( xComutateTrv_1 ) != pdFALSE )          xTimerStop(xComutateTrv_1,1);
	}
	else if (comp->Index==2)
	{
		if( xTimerIsTimerActive( xStartCompressor_2 ) != pdFALSE )      xTimerStop(xStartCompressor_2,1);
		if( xTimerIsTimerActive( xStartHeatPump_2 ) != pdFALSE )        xTimerStop(xStartHeatPump_2,1);
		if( xTimerIsTimerActive( xStartRevValve_2 ) != pdFALSE )		xTimerStop(xStartRevValve_2,1);
		if( xTimerIsTimerActive( xSwitchRevOne_2_time ) != pdFALSE )    xTimerStop(xSwitchRevOne_2_time,1);
		if( xTimerIsTimerActive( xSwitchRevTwo_2_time ) != pdFALSE )	xTimerStop(xSwitchRevTwo_1_time,1);
		if( xTimerIsTimerActive( xStopHeatPump_2 ) != pdFALSE )			xTimerStop(xStopHeatPump_2,1);
		if( xTimerIsTimerActive( xStopRevValve_2 ) != pdFALSE )			xTimerStop(xStopRevValve_2,1);
		if( xTimerIsTimerActive( xStopSource_2 ) != pdFALSE )			xTimerStop(xStopSource_2,1);
		if( xTimerIsTimerActive( xTStartCheckErrors_2 ) != pdFALSE )    xTimerStop(xTStartCheckErrors_2,1);
		if( xTimerIsTimerActive( xTStartCheckTrv_2 ) != pdFALSE )       xTimerStop(xTStartCheckTrv_2,1);
		if( xTimerIsTimerActive( xComutateTrv_2 ) != pdFALSE )          xTimerStop(xComutateTrv_2,1);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void initPins()
{

	DDRG |= (_BV(PG4) | _BV(PG3));
	DDRH |= (_BV(PH7) | _BV(PH6) | _BV(PH5) | _BV(PH4) | _BV(PH3) | _BV(PH1) | _BV(PH2));
	DDRL |= (_BV(PL2) | _BV(PL1) | _BV(PL0));
	DDRD &= (_BV(PD2) | _BV(PD3));
	DDRE &= (_BV(PE5));
	DDRA = 0xFF;
	PORTD|= (_BV(PD2) | _BV(PD3));
	PORTE|= (_BV(PE5));
	
    DDRJ|= (_BV(PIN_rs485));
}
