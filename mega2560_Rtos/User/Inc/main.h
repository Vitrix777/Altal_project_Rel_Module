/*
 * main.h
 *
 * Created: 17.07.2018 8:42:12
 *  Author: User
 */ 


#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
	#endif

	
#define LP_max                  18.0
#define HP_max                  34.0
#define Low_Limit_HP            102.0    //ADC of 1.3v
#define Hi_Limit_HP             922.0    //ADC of 4.5v
#define Low_Limit_LP               55.0    //ADC of 1.3v
#define Hi_Limit_LP                922.0       //ADC of 4.5v

#define START_THWAY_VALVE     (PORTH|=1<<PH7)
#define STOP_THWAY_VALVE      (PORTH&=~(1<<PH7))
#define PIN_THWAY_VALVE       (PINH&(1<<PH7))

#define START_COMPRESSOR_1    (PORTH|=1<<PH5)
#define STOP_COMPRESSOR_1     (PORTH&=~(1<<PH5))
#define START_HEAT_PUMP_1     (PORTH|=1<<PH3)
#define STOP_HEAT_PUMP_1      (PORTH&=~(1<<PH3))
#define START_REV_VALVE_1     (PORTG|=1<<PG4)
#define STOP_REV_VALVE_1      (PORTG&=~(1<<PG4))
#define START_SOURCE_PUMP_1   (PORTH|=1<<PH1)
#define STOP_SOURCE_PUMP_1    (PORTH&=~(1<<PH1))
//-------------------------------state pins COMPRESSOR1
#define PIN_COMPRESSOR_1    (PINH&(1<<PH5))
#define PIN_HEAT_PUMP_1     (PINH&(1<<PH3))
#define PIN_REV_VALVE_1     (PING&(1<<PG4))
#define PIN_SOURCE_PUMP_1   (PINH&(1<<PH1))
#define PIN_SOURCE_FLOW		(PINE&(1<<PE5))
#define PIN_HEAT_FLOW_1		(PIND&(1<<PD2))

//-------------------------------------------------
#define START_COMPRESSOR_2    (PORTH|=1<<PH6)
#define STOP_COMPRESSOR_2     (PORTH&=~(1<<PH6))
#define START_HEAT_PUMP_2     (PORTH|=1<<PH4)
#define STOP_HEAT_PUMP_2      (PORTH&=~(1<<PH4))
#define START_REV_VALVE_2     (PORTG|=1<<PG3)//3
#define STOP_REV_VALVE_2      (PORTG&=~(1<<PG3))//3
#define START_SOURCE_PUMP_2   (PORTH|=1<<PH2)
#define STOP_SOURCE_PUMP_2    (PORTH&=~(1<<PH2))
//---------------------------------state pins COMPRESSOR2
#define PIN_COMPRESSOR_2    (PINH&(1<<PH6))
#define PIN_HEAT_PUMP_2     (PINH&(1<<PH4))
#define PIN_REV_VALVE_2     (PING&(1<<PG3))
#define PIN_SOURCE_PUMP_2   (PINH&(1<<PH2))
#define PIN_HEAT_FLOW_2		(PIND&(1<<PD3))	
//
#define PIN_rs485		PJ2
typedef struct
{  
	bool switchToMode;
	bool checkTrv;
	bool Heating;
	bool Cooling;
	bool heatingWater;
	bool checkErrors;
	bool suply;
	bool calibrate;
	bool trvBusy;
	bool def;
	bool stopdef;
	uint8_t compOn;
	uint8_t heatPOn;
	uint8_t threeWOn;
	uint8_t revVOn;
	uint8_t srcPOn;
	uint8_t modeOfPump;
	uint8_t countError;
	float Dhw_Temp;
	float Bac_Temp;
	float High_Press;
	float Low_Press;
	float Flow_Src;
	float Flow_Heat;
	float Heat_Out;
	float Heat_In;
	float Src_In;
	float Src_Out;
	float Ex_Comp;
	float Suc_Comp;
	float Cond_Temp;
	float SuperHeat;
	float SuperCool;
	//

	float Volt_A;
	float Volt_B;
	float Volt_C;
	float Curr_A;
	float Curr_B;
	float Curr_C;
	//------
	int trvCorection;
	int Steps;
	int Trv_position;
	int error;
	int Index;	
} __attribute__ ((packed)) xCompressor;
//--------------------------------------------
	
typedef struct {

	uint8_t Compressor_delay;
	uint8_t Source_Pump_delay;
	uint8_t Revers_Valve_delay;
	uint8_t HeatingPump_delay;
	uint8_t Three_Way_delay;
	uint8_t TRV_Delay;
	// ************************************** limits********************************************
	uint8_t HP_Max;
	uint8_t HP_Min;	
	uint8_t LP_Max;
	//
	uint8_t HEAT_Max;
	uint8_t HEAT_Min;
	uint8_t Source_Max;
	uint8_t Cool_Max;
	uint8_t Cool_Min;
	uint8_t ExhaustMax;
	uint8_t MinSuperheat;
	uint8_t MaxSuperheat;
	uint8_t MinSupercool;
	uint8_t MaxSupercool;
	uint8_t Delta_heating_min;
	uint8_t Delta_heating_max;
	uint8_t Delta_source_min;
	uint8_t Delta_source_max;
	uint8_t Delta_dhw_min;
	uint8_t Delta_dhw_max;
	//**************************** set histeresis ***********************************
	uint8_t Set_Heating;
	uint8_t Set_Cooling;
	uint8_t Set_DHW;
	uint8_t diffHEAT;
	uint8_t diffCOOL;
	uint8_t diffDHW;
	
	//--------------------defrost
	uint8_t AireToWater;
	uint8_t NombComp;
	uint8_t TimeDef;
	uint8_t TimeBetweenDef;
	uint8_t Humedity;
	
	//******************************mods********************************************
	uint8_t power;
	uint8_t heatWater;
	uint8_t heat;
	uint8_t cool;
	uint8_t reversMod;
	uint8_t Power_380;
	
	int  TempDef;
	int  TempStopDef;
	int  TempOut;
    int  Source_Min;
	// ***************************** timers_1 ******************************
	int trv_corection_1;
	unsigned int time_pump_hiter_1;
	unsigned int time_pump_source_1;
	unsigned int time_revers_1;
	unsigned int time_compressor_1;
	unsigned int time_threeway;	
	unsigned int number_turnon_compressor_1;
	unsigned int number_turnon_source_1;
	unsigned int number_turnon_hiter_1;
	unsigned int number_turnon_revers_1;
	unsigned int number_turnon_threeway;
    // ***************************** timers_2 ******************************
    unsigned int trv_corection_2;
    unsigned int time_pump_hiter_2;
    unsigned int time_pump_source_2;
    unsigned int time_revers_2;
    unsigned int time_compressor_2;
    unsigned int number_turnon_compressor_2;
    unsigned int number_turnon_source_2;
    unsigned int number_turnon_hiter_2;
    unsigned int number_turnon_revers_2;
    unsigned int number_turnon_threeway_2;

	// ************************** states  ****************************
    float LP_Min;
}__attribute__ ((packed)) xEEdata;


#ifdef __cplusplus
}
#endif
#endif /* MAIN_H_ */