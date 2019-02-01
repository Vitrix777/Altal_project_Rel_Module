/*
 * ade7758.c
 *
 * Created: 18.08.2018 0:05:09
 *  Author: User
 */ 
#include "ade7758.h"

void ADE7758_Init() {
	
	// SPI Init
	spiSetDataMode(SPI_MODE2);
	spiSetClockDivider(SPI_CLOCK_DIV32);
	spiSetBitOrder(SPI_MSBFIRST);
	spiBegin(Default);
	ADE7758_setLcycMode(0); // Todos los bits en 0, ver el cpp para mas informacion.
	ADE7758_gainSetup(INTEGRATOR_OFF,FULLSCALESELECT_0_5V,GAIN_1,GAIN_1);
	ADE7758_setupDivs(DIV,DIV,DIV);
	ADE7758_setOpMode(0x04);
	ADE7758_setWavMode(0x10);
	//spiEnd();
}

	
uint8_t ADE7758_read8(uint8_t reg){
	uint8_t b0;
portENTER_CRITICAL();
	spiSelect(Default);
	spiTransfer(reg);
	b0=spiTransfer(0x00);
	spiDeselect(Default);
portEXIT_CRITICAL();
	return b0;
}
uint16_t ADE7758_read16(uint8_t reg){
	uint8_t b1,b0;
portENTER_CRITICAL();
	spiSelect(Default);
	spiTransfer(reg);
	b1=spiTransfer(0x00);
	b0=spiTransfer(0x00);
	spiDeselect(Default);
portEXIT_CRITICAL();	
	return (unsigned int)b1<<8 | (unsigned int)b0;
}
unsigned long ADE7758_read24(uint8_t reg){
	uint8_t b2,b1,b0;
portENTER_CRITICAL();
	spiSelect(Default);
	spiTransfer(reg);
	b2=spiTransfer(0x00);
	b1=spiTransfer(0x00);
	b0=spiTransfer(0x00);
	spiDeselect(Default);
portEXIT_CRITICAL();
	return (unsigned long)b2<<16 | (unsigned long)b1<<8 | (unsigned long)b0;
}
//////write
void ADE7758_write8(uint8_t reg, uint8_t data){
uint8_t data0;
portENTER_CRITICAL();
	spiSelect(Default);
	// 8th bit (DB7) of the register address controls the Read/Write mode (Refer to spec page 55 table 13)
	// For Write -> DB7 = 1  / For Read -> DB7 = 0
	reg |= WRITE;
	data0 = (uint8_t)data;
	spiTransfer((uint8_t)reg);          //register selection
	spiTransfer((uint8_t)data0);
	spiDeselect(Default);
portEXIT_CRITICAL();
}
void ADE7758_write16(uint8_t reg, uint16_t data){
	uint8_t data0,data1;
portENTER_CRITICAL();	
	spiSelect(Default);
	// 8th bit (DB7) of the register address controls the Read/Write mode (Refer to spec page 55 table 13)
	// For Write -> DB7 = 1  / For Read -> DB7 = 0
	reg |= WRITE;
	data0 = (uint8_t)data;
	data1 = (uint8_t)(data>>8);
	spiTransfer((uint8_t)reg);          //register selection
	spiTransfer((uint8_t)data1);
	spiTransfer((uint8_t)data0);
	spiDeselect(Default);
portEXIT_CRITICAL();
}
void ADE7758_write24(uint8_t reg, unsigned long data){
	uint8_t data0,data1,data2;
portENTER_CRITICAL();	
	spiSelect(Default);
	// 8th bit (DB7) of the register address controls the Read/Write mode (Refer to spec page 55 table 13)
	// For Write -> DB7 = 1  / For Read -> DB7 = 0
	reg |= WRITE;
	data0 = (uint8_t)data;
	data1 = (uint8_t)(data>>8);
	data2 = (uint8_t)(data>>16);	
	spiTransfer((uint8_t)reg);          //register selection
	spiTransfer((uint8_t)data2);
	spiTransfer((uint8_t)data1);
	spiTransfer((uint8_t)data0);
	spiDeselect(Default);
portEXIT_CRITICAL();
}
//////
uint8_t ADE7758_getVersion(){
	return ADE7758_read8(VERSION);
}
void ADE7758_setOpMode(uint8_t data){
	ADE7758_write8(OPMODE, data);
}
int ADE7758_getOpMode(){
	return ADE7758_read8(OPMODE);
}

void ADE7758_setMMode(uint8_t data){
	ADE7758_write8(MMODE, data);
}
int ADE7758_getMMode(){
	return ADE7758_read8(MMODE);
}
void ADE7758_setWavMode(uint8_t data){
	 ADE7758_write8(WAVMODE, data);
}
int ADE7758_getWavMode(){
	return  ADE7758_read8(WAVMODE);
}

void ADE7758_setCompMode(uint8_t data){
	ADE7758_write8(COMPMODE, data);
}
int ADE7758_getCompMode(){
	return ADE7758_read8(COMPMODE);
}

void  ADE7758_setLcycMode(uint8_t data){
	 ADE7758_write8(LCYCMODE, data);
}
int  ADE7758_getLcycMode(){
	return  ADE7758_read8(LCYCMODE);
}
void ADE7758_gainSetup(uint8_t integrator, uint8_t scale, uint8_t PGA2, uint8_t PGA1){
	uint8_t pgas = (integrator<<7) | (PGA2<<5) | (scale<<3) | (PGA1);
	ADE7758_write8(GAIN,pgas);//write GAIN register, format is |3 bits PGA2 gain|2 bits full scale|3 bits PGA1 gain
}

void ADE7758_setupDivs(uint8_t Watt_div,uint8_t VAR_div,uint8_t VA_div){
	ADE7758_write8(WDIV,Watt_div);
	ADE7758_write8(VARDIV,VAR_div);
	ADE7758_write8(VADIV,VA_div);
}
long ADE7758_getMaskInterrupts(void){
	return ADE7758_read24(MASK);
}
void ADE7758_setMaskInterrupts(unsigned long data){
	return ADE7758_write24(MASK, data);
}
unsigned long ADE7758_getStatus(){
	return ADE7758_read24(STATUS);
}
unsigned long ADE7758_resetStatus(){
	return ADE7758_read24(RSTATUS);
}
/////////////////////////////////////////////////
long ADE7758_getAIRMS(){

	ADE7758_resetStatus(); // Clear all interrupts
	return ADE7758_read24(AIRMS);
}
long ADE7758_getBIRMS(){
	
	ADE7758_resetStatus(); // Clear all interrupts
	return ADE7758_read24(BIRMS);
}
long ADE7758_getCIRMS(){
	
	ADE7758_resetStatus(); // Clear all interrupts
	return ADE7758_read24(CIRMS);
	
}
long ADE7758_getAVRMS(){

	ADE7758_resetStatus(); // Clear all interrupts
	return ADE7758_read24(AVRMS);
}
long ADE7758_getBVRMS(){
	
	ADE7758_resetStatus(); // Clear all interrupts
	 return ADE7758_read24(BVRMS);
}
long ADE7758_getCVRMS(){
	ADE7758_resetStatus(); // Clear all interrupts
return ADE7758_read24(CVRMS);

}
long ADE7758_avrms(){
	uint8_t i=0;
	long v=0;
		for(i=0;i<10;++i)v+=ADE7758_getAVRMS();
return v/10;
}
long ADE7758_bvrms(){
	uint8_t i=0;
	long v=0;	
		for(i=0;i<10;++i)v+=ADE7758_getBVRMS();
return v/10;	

	
}
long ADE7758_cvrms(){
	uint8_t i=0;
	long v=0;
		for(i=0;i<10;++i) v+=ADE7758_getCVRMS();
return v/10;	
	}
long ADE7758_airms(){
	uint8_t n=0;
	long i=0;
		for(n=0;n<10;++n)i+=ADE7758_getAIRMS();			
return i/10;
}
long ADE7758_birms(){
	uint8_t n=0;
	long i=0;
		for(n=0;n<10;++n)i+=ADE7758_getBIRMS();
return i/10;
}
long ADE7758_cirms(){
	uint8_t n=0;
	long i=0;
		for(n=0;n<10;++n)i+=ADE7758_getCIRMS();
return i/10;

}
int ADE7758_getFreq(){
	return ADE7758_read16(FREQ);
}
void ADE7758_setLineCyc(int data){
		ADE7758_write16(LINECYC,data);
	}
int ADE7758_getACurrentOffset(){
	return ((ADE7758_read16(AIRMSOS))<<4)>>4;
}
int ADE7758_getBCurrentOffset(){
	return ((ADE7758_read16(BIRMSOS))<<4)>>4;
}
int ADE7758_getCCurrentOffset(){
	return ((ADE7758_read16(CIRMSOS))<<4)>>4;
}
void ADE7758_setACurrentOffset(int data){
	ADE7758_write16(AIRMSOS, data);
}
void ADE7758_setBCurrentOffset(int data){
	ADE7758_write16(BIRMSOS, data);
}
void ADE7758_setCCurrentOffset(int data){
	ADE7758_write16(CIRMSOS, data);
}
int ADE7758_getAVoltageOffset(){
	return ((ADE7758_read16(AVRMSOS))<<4)>>4;
}
int ADE7758_getBVoltageOffset(){
	return ((ADE7758_read16(BVRMSOS))<<4)>>4;
}
int ADE7758_getCVoltageOffset(){
	return ((ADE7758_read16(CVRMSOS))<<4)>>4;
}
void ADE7758_setAVoltageOffset(int data){
	ADE7758_write16(AVRMSOS, data);
}
void ADE7758_setBVoltageOffset(int data){
	ADE7758_write16(BVRMSOS, data);
}
void ADE7758_setCVoltageOffset(int data){
	ADE7758_write16(CVRMSOS, data);
}

void ADE7758_setZeroCrossingTimeout(int data){
	ADE7758_write16(ZXTOUT,data);
}
int ADE7758_getZeroCrossingTimeout(){
	return ADE7758_read16(ZXTOUT);
}
uint8_t ADE7758_setPotLine(uint8_t Phase, int Ciclos){
	uint8_t m = 0;
	switch (Phase){
		case PHASE_A:	m = (LWATT | LVAR | LVA | ZXSEL_A);
		break;
		case PHASE_B:	m = (LWATT | LVAR | LVA | ZXSEL_B);
		break;
		case PHASE_C:	m = (LWATT | LVAR | LVA | ZXSEL_C);
		break;
	}
	ADE7758_setLcycMode(0);
	ADE7758_setLcycMode(m);
	ADE7758_resetStatus();
	ADE7758_setLineCyc(Ciclos);
return Phase;
	
}
int ADE7758_getWatt(uint8_t Phase){
	int temp = 0;
	switch (Phase){
		case PHASE_A:	temp = ADE7758_read16(AWATTHR);
		break;
		case PHASE_B:	temp = ADE7758_read16(BWATTHR);
		break;
		case PHASE_C:	temp = ADE7758_read16(CWATTHR);
		break;
	}
	return temp;
}
int ADE7758_getVar(uint8_t Phase){
	int temp = 0;
	switch (Phase){
		case PHASE_A:	temp = ADE7758_read16(AVARHR);
		break;
		case PHASE_B:	temp = ADE7758_read16(BVARHR);
		break;
		case PHASE_C:	temp = ADE7758_read16(CVARHR);
		break;
	}
	return temp;
}
int ADE7758_getVa(uint8_t Phase){
	int temp = 0;
	switch (Phase){
		case PHASE_A:	temp = ADE7758_read16(AVAHR);
		break;
		case PHASE_B:	temp = ADE7758_read16(BVAHR);
		break;
		case PHASE_C:	temp = ADE7758_read16(CVAHR);
		break;
	}
	return temp;
}
void ADE7758_calibrateVI(uint8_t numSamples){
	ADE7758_write8(LCYCMODE, 0x38);
	ADE7758_write24(MASK, 0xE00);
	long AVRMSBuffer = 0, BVRMSBuffer = 0, CVRMSBuffer = 0;
	long AIRMSBuffer = 0, BIRMSBuffer = 0, CIRMSBuffer = 0;
	for (uint8_t i=0; i<numSamples; i++) {
		ADE7758_resetStatus();
		//while ( digitalRead(2) != 0 ) {} 
		
		AVRMSBuffer += ADE7758_avrms();
		BVRMSBuffer += ADE7758_bvrms();
		CVRMSBuffer += ADE7758_cvrms();
		AIRMSBuffer += ADE7758_airms();
		BIRMSBuffer += ADE7758_birms();
		CIRMSBuffer += ADE7758_cirms();
	}
	AVRMSCalib = AVRMSBuffer/numSamples;
	BVRMSCalib = BVRMSBuffer/numSamples;
	CVRMSCalib = CVRMSBuffer/numSamples;
	AIRMSCalib = AIRMSBuffer/numSamples;
	BIRMSCalib = BIRMSBuffer/numSamples;
	CIRMSCalib = CIRMSBuffer/numSamples;
	
}