/*
 * AdcSensors.h
 *
 * Created: 14.07.2018 19:16:52
 *  Author: User
 */ 


#ifndef ADCSENSORS_H
#define ADCSENSORS_H

#include "FreeRTOS.h"
#include  "task.h"
#include "main.h"
#include  "TRV.h"
#ifdef __cplusplus
extern "C" {
	#endif
	


float  ConvertToTemp(uint16_t ADC_value);
float  ConvertToHighPress(uint16_t ADC_value);
float  ConvertToLowPress(uint16_t ADC_value);
void   ReadSensors(xCompressor *comp);
float  ReadSuperHeat (xCompressor *comp);
float  ReadSuperCool (xCompressor *comp);
float   corectTrv(xCompressor *comp);

#ifdef __cplusplus
}
#endif
#endif /* ADCSENSORS_H_ */