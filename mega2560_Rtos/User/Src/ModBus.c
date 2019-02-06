#include "ModBus.h"

extern xCompressor Comp_1,Comp_2;
extern uint8_t  forsDefrost;
extern xEEdata EEpromData;
extern volatile bool port_0,port_3;

void timer_1_Init()
{
	// --- TIMER1_COMPA irq ---
	// selected time = 1562 us (24992 ticks)
	// prescaler = 1 (1 ticks ... 4.096 ms)
	TCCR1B = (1<<WGM12);//|(1<<CS10)
	OCR1AH = 158 ;//158
	OCR1AL =96;//96
	stop_1_Timer();
	
	
}
void start_1_Timer()
{
	TIMSK1 |= (1<<OCIE1A);
	TCCR1B |= (1<<CS11);//1<<CS10 |
	
}
void stop_1_Timer()
{
	TCCR1B &= ~ (1<<CS10);
	TCNT1=0;
	TIMSK1 &= ~( _BV( OCIE1A ) );
	TIFR1 |= _BV( OCF1A );
}

int readSysReg(xCompressor*compPtr,xEEdata *eeprom,uint16_t adress)
{
	int temp;
	switch(adress)
	{
		case 11: case 12: temp =  (int)   compPtr->compOn;break;
		case 21: case 22: temp =   (int)  compPtr->srcPOn;break;
		case 31: case 32: temp =   (int)  compPtr->heatPOn;break;
		case 41: case 42: temp =   (int)  compPtr->revVOn;break;
		case 51: case 52: temp =   (int)  compPtr->threeWOn;break;
		case 61: case 62: temp =   (int)  compPtr->Trv_position;break;
		
		case 71: case 72: temp =   (int)   (compPtr->Bac_Temp*10);break;
		case 81: case 82:  temp =   (int)  (compPtr->Dhw_Temp*10);break;
		case 91:case 92:  temp =  (int)  (compPtr->Src_In*10);break;
		case 101:case 102:  temp =  (int)  (compPtr->Src_Out*10);break;
		case 111:case 112: temp =   (int)  (compPtr->error);break;
		case 121: case 122: temp =   (int)   (compPtr->Cond_Temp*10);break;
		case 131:case 132: temp =   (int)  (compPtr->Suc_Comp*10);break;
		case 141:case 142: temp =   (int)  (compPtr->SuperCool*10);break;
		case 151:case 152: temp =   (int)  (compPtr->SuperHeat*10);break;
		case 161:case 162:  temp =  (int)  (compPtr->Heat_In*10);break;
		case 171:case 172:  temp =  (int)  (compPtr->Heat_Out*10);break;		
		case 181:case 182: temp =   (int)  (compPtr->High_Press*10);break;
		case 191:case 192: temp =   (int)  (compPtr->Low_Press*10);break;
		case 201:case 202: temp =   (int)  (compPtr->Ex_Comp);break;
		
	
		
		//-----------------------------------eeprom--------
		case 10:  temp =    eeprom->Compressor_delay;break;
		case 20:  temp =    eeprom->HeatingPump_delay;break;
		case 30:  temp =    eeprom->Source_Pump_delay;break;
		case 40:  temp =    eeprom->Revers_Valve_delay;break;
		case 50:  temp =    eeprom->Three_Way_delay;break;
		case 60:  temp =    (int)  eeprom->Cool_Max;break;
		case 70:  temp =    (int)  eeprom->Cool_Min;break;
		case 80:  temp =    (int)  eeprom->HEAT_Max;break;
		case 90:  temp =    (int)  eeprom->HEAT_Min;break;
		case 100: temp =    (int)  eeprom->Delta_heating_max;break;
		case 110: temp =    (int)  eeprom->Delta_heating_min;break;
		case 120: temp =    (int)  eeprom->Delta_source_max;break;
		case 130: temp =    (int)  eeprom->Delta_source_min;break;
		case 140: temp =    (int)  eeprom->diffCOOL;break;
		case 150: temp =    (int)  eeprom->diffDHW ;break;
		case 160: temp =    (int)  eeprom->diffHEAT;break;
		case 170: temp =    (int)  eeprom->AireToWater;break;
		case 180: temp =    (int)  eeprom->ExhaustMax;break;
		case 190: temp =    (int)  eeprom->cool;break;
		case 200: temp =    (int)  eeprom->heat;break;
		case 210: temp =    (int)  eeprom->heatWater;break;
		case 220: temp =    (int)  eeprom->HP_Max;break;
		case 230: temp =    (int)  eeprom->LP_Max;break;
		case 240: temp =    (int)  eeprom->HP_Min;break;
		case 250: temp =    (int)  eeprom->LP_Min;break;
		case 260: temp =    (int)  eeprom->MaxSupercool;break;
		case 270: temp =    (int)  eeprom->MinSupercool;break;
		case 280: temp =    (int)  eeprom->MaxSuperheat;break;
		case 290: temp =    (int)  eeprom->MinSuperheat;break;
		case 300: temp =    (int)  eeprom->number_turnon_threeway;break;
		case 310: temp =    (int)  eeprom->power;break;
		case 320: temp =    (int)  eeprom->Set_Cooling;break;
		case 330: temp =    (int)  eeprom->Set_DHW;break;
		case 340: temp =    (int)  eeprom->Set_Heating;break;
		case 350: temp =    (int)  eeprom->TempDef;break;
		case 360: temp =    (int)  eeprom->TimeDef;break;
		
		
		case 370: temp =    (int)  eeprom->TempOut;break;
		case 380: temp =    (int)  eeprom->TempStopDef;break;
		case 390: temp =    (int)  eeprom->time_threeway;break;
		case 400: temp =    (int)  eeprom->NombComp;break;
		case 410: temp =    (int)  eeprom->reversMod;break;
		case 420: temp =    (int)  eeprom->TRV_Delay;break;
		case 430: temp =    (int)  eeprom->Source_Max;break;
		case 440: temp =    (int)  eeprom->Source_Min;break;
		case 450: temp =    (int)  eeprom->Delta_dhw_max;break;
		case 460: temp =    (int)  eeprom->Delta_dhw_min;break;
		case 470: temp =    (int)  eeprom->Power_380;break;
		case 480: temp =    (int)  eeprom->TimeBetweenDef;break;
		//case 470: temp =    (int)  compPtr-> Volt_A;break;
		//case 480: temp =    (int)  compPtr-> Volt_B;break;
		//case 490: temp =    (int)  compPtr-> Volt_C;break;
		//case 500: temp =    (int)  compPtr-> Curr_A;break;
		//case 510: temp =    (int)  compPtr-> Curr_B;break;
		//case 520: temp =    (int)  compPtr-> Curr_C;break;
		
		//--------------------------------------
		case 301: temp =    (int)  eeprom->number_turnon_compressor_1;break;
		case 311: temp =    (int)  eeprom->number_turnon_hiter_1;break;
		case 321: temp =    (int)  eeprom->number_turnon_revers_1;break;
		case 331: temp =    (int)  eeprom->number_turnon_source_1;break;
		case 341: temp =    (int)  eeprom->time_compressor_1;break;
		case 351: temp =    (int)  eeprom->time_pump_hiter_1;break;
		case 371: temp =    (int)  eeprom->time_pump_source_1;break;
		case 381: temp =    (int)  eeprom->time_revers_1;break;
		case 391: temp =    (int)  eeprom->trv_corection_1;break;
		//------------------------------------
		case 302: temp =    (int)  eeprom->number_turnon_compressor_2;break;
		case 312: temp =    (int)  eeprom->number_turnon_hiter_2;break;
		case 322: temp =    (int)  eeprom->number_turnon_revers_2;break;
		case 332: temp =    (int)  eeprom->number_turnon_source_2;break;
		case 342: temp =    (int)  eeprom->time_compressor_2;break;
		case 352: temp =    (int)  eeprom->time_pump_hiter_2;break;
		case 372: temp =    (int)  eeprom->time_pump_source_2;break;
		case 382: temp =    (int)  eeprom->time_revers_2;break;
		case 392: temp =    (int)  eeprom->trv_corection_2;break;
		
	}
	return temp;
}
void writeSysReg(xEEdata*comp,uint16_t adress,uint16_t value)
{
	
	switch(adress)
	{
		case 10:  comp->Compressor_delay  = value;break;
		case 20:  comp->HeatingPump_delay = value;break;
		case 30:  comp->Source_Pump_delay = value;break;
		case 40:  comp->Revers_Valve_delay = value;break;
		case 50:  comp->Three_Way_delay  = value;break;
		case 60:  comp->Cool_Max = value;break;
		case 70:  comp->Cool_Min = value;break;
		case 80:  comp->HEAT_Max  = value;break;
		case 90:  comp->HEAT_Min = value;break;
		case 100: comp->Delta_heating_max  = value;break;
		case 110: comp->Delta_heating_min  = value;break;
		case 120: comp->Delta_source_max  = value;break;
		case 130: comp->Delta_source_min  = value;break;
		case 140: comp->diffCOOL  = value;break;
		case 150: comp->diffDHW  = value;break;
		case 160: comp->diffHEAT  = value;break;
		case 170: comp->AireToWater  = value;break;
		case 180: comp->ExhaustMax  = value;break;
		case 190: comp->cool  = value;break;
		case 200: comp->heat  = value;break;
		case 210: comp->heatWater  = value;break;
		case 220: comp->HP_Max  = value;break;
		case 230: comp->LP_Max  = value;break;
		case 240: comp->HP_Min  = value;break;
		case 250: comp->LP_Min  = value;break;
		case 260: comp->MaxSupercool  = value;break;
		case 270: comp->MinSupercool  = value;break;
		case 280: comp->MaxSuperheat  = value;break;
		case 290: comp->MinSuperheat  = value;break;
		case 300: comp->number_turnon_threeway  = value;break;
		case 310: comp->power  = value;break;
		case 320: comp->Set_Cooling  = value;break;
		case 330: comp->Set_DHW  = value;break;
		case 340: comp->Set_Heating  = value;break;
		case 350: comp->TempDef  = value;break;//
		case 360: comp->TimeDef  = value;break;//
		case 370: comp->TempOut  = value;break;
		case 380: comp->TempStopDef  = value;break;//
		case 390: comp->time_threeway  = value;break;
		case 400: comp->NombComp  = value;break;
		case 410: comp->reversMod =value;break;
		case 420: comp->TRV_Delay=value;break;
		case 430: comp->Source_Max=value;break;
		case 440: comp->Source_Min=value;break;
		case 450: comp->Delta_dhw_max  = value;break;
		case 460: comp->Delta_dhw_min  = value;break;
		case 470: comp->Power_380 = value;break;
		case 480: comp->TimeBetweenDef = value;break;//
		case 391: comp->trv_corection_1 = value;break;
		case 392: comp->trv_corection_2 = value;break;
		case 393:if(comp->AireToWater==1) forsDefrost = value;break;
	}
}

void modbus_update()
{
	unsigned char buffer = 0;
	unsigned char id;
	unsigned int startingAddress;
	unsigned int no_of_registers;
	unsigned int maxData;
	uint16_t index;
	uint16_t address;
	unsigned int crc16;
	unsigned int crc;
	unsigned char noOfBytes;
	unsigned char responseFrameSize;
	unsigned int temp;
	void* ptr=NULL;
	
	if(port_0)buffer=xSerialAvailableChar( &xSerialPort );
	else if (port_3)buffer=xSerialAvailableChar( &xSerial3Port );
	
	//avrSerialPrintf("buffer = %d \r\n",buffer );
	//avrSerialPrintf("overflow= %d \r\n",overflow);
	if (overflow){ return;}//exceptionResponse(OVERFLOW_BUFF);
	if (buffer > 0 && buffer < 8) {  return;}// exceptionResponse(CORRUPT_PACKET);
	
	// The minimum request packet is 8 bytes for function 3 & 16
	
	id = frame[0];
	//avrSerialPrintf("buffer= %d \r\n",buffer);
	
	if (id == slaveID ) // if the recieved ID matches the slaveID or broadcasting id (0), continue
	{
		crc = ((frame[buffer - 2] << 8) | frame[buffer - 1]); // combine the crc Low & High bytes
		//avrSerialPrintf("crc= %d \r\n",crc);
		if (calculateCRC(buffer - 2) == crc) // if the calculated crc matches the recieved crc continue
		{
			function = frame[1];
			startingAddress = ((frame[2] << 8) | frame[3]); // combine the starting address bytes
			no_of_registers = ((frame[4] << 8) | frame[5]);
			maxData = startingAddress + no_of_registers *10;
			//avrSerialPrintf("function= %d \r\n",function);
			//avrSerialPrintf("startingAddress= %d \r\n",startingAddress);
			//avrSerialPrintf("no_of_registers= %d \r\n",no_of_registers);
			//avrSerialPrintf("maxData= %d \r\n",maxData);
			if (function == 3)
			{
				if(startingAddress%10==1) {ptr=&Comp_1;}
				else if(startingAddress%10==2){ptr=&Comp_2;}
				
				noOfBytes = no_of_registers * 2;
				responseFrameSize = 5 + noOfBytes; // ID, function, noOfBytes, (dataLo + dataHi) * number of registers, crcLo, crcHi
				frame[0] = slaveID;
				frame[1] = function;
				frame[2] = noOfBytes;
				//avrSerialPrintf("FrameSize= %d \r\n",responseFrameSize);
				//avrSerialPrintf("frame[1]= %d \r\n",frame[1]);
				//avrSerialPrintf("frame[2]= %d \r\n",frame[2]);
				address = 3; // PDU starts at the 4th byte
				for (index = startingAddress; index < maxData; )
				{
					temp = readSysReg(ptr,&EEpromData,index);//holdingRegs[index]
					frame[address] = temp >> 8; // split the register into 2 bytes
					//avrSerialPrintf("frame[data]= %d \r\n",frame[address]);
					address++;
					frame[address] = temp & 0xFF;
					//avrSerialPrintf("frame[data]= %d \r\n",frame[address]);
					address++;
					index +=10;
				}
				
				crc16 = calculateCRC(responseFrameSize - 2);
				frame[responseFrameSize - 2] = crc16 >> 8; // split crc into 2 bytes
				frame[responseFrameSize - 1] = crc16 & 0xFF;
				//return;
				sendPacket(responseFrameSize);
				
			}
			
			else if (function == 16)
			{
				address = 7; // start at the 8th byte in the frame
				
				for (index = startingAddress; index < maxData; )
				{
					temp = ((frame[address] << 8) | frame[address + 1]);
					writeSysReg(&EEpromData,index,temp);
					address += 2;
					index+=10;
				}
				portENTER_CRITICAL();
				eeprom_write_block((const void*)&EEpromData, (void*)0, sizeof (xEEdata));
				portEXIT_CRITICAL();
				// only the first 6 bytes are used for CRC calculation
				crc16 = calculateCRC(6);
				frame[6] = crc16 >> 8; // split crc into 2 bytes
				frame[7] = crc16 & 0xFF;
				
				// a function 16 response is an echo of the first 6 bytes from the request + 2 crc bytes
				
				sendPacket(8);
				
				
			}
			
			
			
			
		}
		else {return;}// exceptionResponse(CHECKSUM_FAIL);
	}
	else {return;}//exceptionResponse(ERROR_FUNC);
}

void exceptionResponse(unsigned char exception)
{
	unsigned int crc16;
	
	frame[0] = slaveID;
	frame[1] = (function | 0x80); // set the MSB bit high, informs the master of an exception
	frame[2] = exception;
	crc16 = calculateCRC(3); // ID, function + 0x80, exception code == 3 bytes
	frame[3] = crc16 >> 8;
	frame[4] = crc16 & 0xFF;
	sendPacket(5); // exception response is always 5 bytes ID, function + 0x80, exception code, 2 bytes crc

}
unsigned int calculateCRC(unsigned char bufferSize)
{
	unsigned int temp, temp2, flag;
	temp = 0xFFFF;
	for (unsigned char i = 0; i < bufferSize; i++)
	{
		temp = temp ^ frame[i];
		for (unsigned char j = 1; j <= 8; j++)
		{
			flag = temp & 0x0001;
			temp >>= 1;
			if (flag)
			temp ^= 0xA001;
		}
	}
	// Reverse byte order.
	temp2 = temp >> 8;
	temp = (temp << 8) | temp2;
	temp &= 0xFFFF;
	return temp; // the returned value is already swopped - crcLo byte is first & crcHi byte is last
}

void sendPacket(unsigned char bufferSize)
{
	
	for (unsigned char i = 0; i < bufferSize; i++) if(port_0)xSerialPutChar(&xSerialPort,frame[i]); else if(port_3){PORTJ|= (_BV(PIN_rs485));xSerialPutChar(&xSerial3Port,frame[i]);}
	//xSerialRxFlush(&xSerialPort);
	PORTJ &= (_BV(PIN_rs485));

	_delay_us(3645);
	
	
}