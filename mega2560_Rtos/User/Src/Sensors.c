
#include <math.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include  "FreeRTOS.h"
#include  "queue.h"
#include  "croutine.h" 
#include  "digitalAnalog.h"
#include  "Sensors.h"
#include  "main.h"
#include "ade7758.h"

extern QueueHandle_t CompData_1,CompData_2;
extern xCompressor Comp_1,Comp_2;
extern xEEdata EEpromData;

const float R22_Saturation_Temp[] PROGMEM =
{ -25.2, -14.7, -6.6, 0.1, 5.09, 10.9, 15.5, 19.7, 23.5, 27.1,
	30.4, 33.6, 36.6, 39.4, 42.1, 44.7, 42.2, 49.6, 51.9, 54.1,
	56.3, 58.4, 60.4, 62.3, 64.3, 66.1, 67.9, 69.7, 71.5, 73.2,
	74.8, 76.4
};


float ConvertToTemp(uint16_t ADC_value){

	float resistor = 10000.0;
	float Resistance;
	float Temp;		
	Resistance = resistor * ((1023.0 / ADC_value) - 1);
	Temp=(3470*298.15)/(3470 + (298.15 * log(Resistance / 5000)));
	Temp = log(Resistance); 
	Temp = 1 / (0.001294548729 + (0.0002344885576 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
	Temp = Temp - 273.15;	
	return Temp;
}
float ConvertToHighPress(uint16_t ADC_value){ 
	return (ADC_value - Low_Limit_HP) * HP_max/ (Hi_Limit_HP - Low_Limit_HP);
}

float ConvertToLowPress(uint16_t ADC_value){
	return (ADC_value - Low_Limit_LP) * LP_max/ (Hi_Limit_LP - Low_Limit_LP) ;
}

void ReadSensors(xCompressor *comp)  
{ 
 uint16_t temp = 0;
 const uint8_t samples = 10;        // determines the number of samples taken
 unsigned int sum;               // holds the summated samples
 uint8_t i;
 //static TickType_t countTrvTime=0;
 setAnalogMode(MODE_10_BIT);
 i = samples;                     // clean up for next channel
 sum = 0;
 
 do
 {
	 startAnalogConversion(1, 0);
	 while( analogIsConverting() )_delay_us(25);//portYIELD();
	 sum += 	analogConversionResult();
	 //temp =analogConversionResult();
 } while (--i);
 temp = (unsigned int) (sum / samples);
 comp->Bac_Temp = ConvertToTemp(temp);
 //read DHW
 PORTL &= ~ (1 << PL2);//A0
 PORTL &= ~ (1 << PL1);//A1
 PORTL |=   (1 << PL0);//A2
 i = samples;
 sum = 0;

 do
 {
	 startAnalogConversion(0, 0);
	 while( analogIsConverting())_delay_us(25);// portYIELD();
	 sum += 	analogConversionResult();
 } while (--i);
 //temp = analogConversionResult();
 temp = (unsigned int) (sum / samples);
 comp->Dhw_Temp = ConvertToTemp(temp);
  //------------------------------------------
  
  //read tempout
  PORTL |=   (1 << PL2);//A0
  PORTL &= ~ (1 << PL1);//A1
  PORTL |=   (1 << PL0);//A2
  i = samples;
  sum = 0;

  do
  {
	  startAnalogConversion(0, 0);
	  while( analogIsConverting())_delay_us(25);// portYIELD();
	  sum += 	analogConversionResult();
  } while (--i);
 //temp = analogConversionResult();
  temp = (unsigned int) (sum / samples);
  comp->Temp_Out = ConvertToTemp(temp);
   /*
  */
  //------------------------------------------
 if(comp->Index==1){
	 
	 //read High_Pressure_1
	 PORTL |=    (1 << PL2);//a0
	 PORTL |=    (1 << PL1);//a1
	 PORTL &= ~  (1 << PL0);//a2
	 i = samples;
	 sum = 0;
	 do
	 {		     startAnalogConversion(0, 0);
		 while( analogIsConverting() ) _delay_us(25);//portYIELD();
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->High_Press = ConvertToHighPress(temp);
	 //read Low_Pressure_1
	 PORTL &= ~   (1 << PL2);
	 PORTL &= ~   (1 << PL1);
	 PORTL &= ~   (1 << PL0);
	 i = samples;
	 sum = 0;
	 do
	 {
		 startAnalogConversion(0, 0);
		 while( analogIsConverting() )_delay_us(25);//portYIELD();
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->Low_Press = ConvertToLowPress( temp );
	 //---------------------------------------------------------------
	 i = samples;
	 sum = 0;
	 do
	 {
		 startAnalogConversion(9, 0);
		 while( analogIsConverting() )_delay_us(25);//portYIELD();
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->Heat_Out = ConvertToTemp( temp );
	 //---------------------------------------------------------------
	 i = samples;
	 sum = 0;
	 do
	 {
		 startAnalogConversion(10, 0);
		 while( analogIsConverting() )_delay_us(25);//portYIELD();
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->Heat_In = ConvertToTemp( temp );
	 //---------------------------------------------------------------
	 i = samples;
	 sum = 0;
	 do
	 {
		 startAnalogConversion(11, 0);
		 while( analogIsConverting() )_delay_us(25);//portYIELD();
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->Src_In = ConvertToTemp( temp );
	 //---------------------------------------------------------------
	 i = samples;
	 sum = 0;
	 do
	 {
		 startAnalogConversion(12, 0);
		 while( analogIsConverting() )_delay_us(25);
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->Src_Out = ConvertToTemp( temp );
	 //---------------------------------------------------------------
	 i = samples;
	 sum = 0;
	 do
	 {
		 startAnalogConversion(13, 0);
		 while( analogIsConverting() )_delay_us(25);//portYIELD();
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->Ex_Comp = ConvertToTemp( temp );
	 //---------------------------------------------------------------
	 i = samples;
	 sum = 0;
	 do
	 {
		 startAnalogConversion(14, 0);
		 while( analogIsConverting() )_delay_us(25);//portYIELD();
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->Suc_Comp = ConvertToTemp( temp );
	 //---------------------------------------------------------------
	 i = samples;
	 sum = 0;
	 do
	 {
		 startAnalogConversion(15, 0);
		 while( analogIsConverting() )_delay_us(25);//portYIELD();
		 sum += analogConversionResult();
	 } while (--i);
	 //temp = analogConversionResult();
	 temp = (unsigned int) (sum / samples);
	 comp->Cond_Temp = ConvertToTemp( temp )	;
	 //--------------------------------------------------------------
	 comp->SuperHeat=ReadSuperHeat (comp);
	 comp->SuperCool=ReadSuperCool (comp);				
						}
						
    if(comp->Index==2) 
	{
					 //read High_Pressure_1
					 PORTL |=    (1 << PL2);//a0
					 PORTL &= ~  (1 << PL1);//a1
					 PORTL &= ~  (1 << PL0);//a2
					 startAnalogConversion(0, 0);
					 while( analogIsConverting() )
					 _delay_us(25);//portYIELD();
					 temp = analogConversionResult();
					 comp->High_Press = ConvertToHighPress(temp);
					 //read Low_Pressure_1
					 PORTL &= ~   (1 << PL2);
					 PORTL |=     (1 << PL1);
					 PORTL &= ~   (1 << PL0);
					 startAnalogConversion(0, 0);
					 while( analogIsConverting() )
					_delay_us(25);// portYIELD();
					 temp = analogConversionResult();
					 comp->Low_Press = ConvertToLowPress( temp );
					startAnalogConversion(2, 0);
					while( analogIsConverting() )
					_delay_us(25);//portYIELD();
					temp = analogConversionResult();
					comp->Heat_Out = ConvertToTemp( temp );
					//---------------------------------------------------------------
					startAnalogConversion(3, 0);
					while( analogIsConverting() )
					_delay_us(25);//portYIELD();
					temp = analogConversionResult();
					comp->Heat_In = ConvertToTemp( temp );
					//---------------------------------------------------------------
					startAnalogConversion(4, 0);
					while( analogIsConverting() )
					_delay_us(25);//portYIELD();
					temp = analogConversionResult();
					comp->Src_In = ConvertToTemp( temp );
					//---------------------------------------------------------------
					startAnalogConversion(5, 0);
					while( analogIsConverting() )
					_delay_us(25);//portYIELD();
					temp = analogConversionResult();
					comp->Src_Out = ConvertToTemp( temp );
					//---------------------------------------------------------------
					startAnalogConversion(6, 0);
					while( analogIsConverting() )
					_delay_us(25);//portYIELD();
					temp = analogConversionResult();
					comp->Ex_Comp = ConvertToTemp( temp );
					//---------------------------------------------------------------
					startAnalogConversion(7, 0);
					while( analogIsConverting() )
					_delay_us(25);//portYIELD();
					temp = analogConversionResult();
					comp->Suc_Comp = ConvertToTemp( temp );
					//---------------------------------------------------------------
					startAnalogConversion(8, 0);
					while( analogIsConverting() )
					_delay_us(25);//portYIELD();
					temp = analogConversionResult();
					comp->Cond_Temp = ConvertToTemp( temp )	;
					//--------------------------------------------------------------
					comp->SuperHeat=ReadSuperHeat (comp);
					comp->SuperCool=ReadSuperCool (comp);
					if(comp->checkTrv)comp->trvCorection=corectTrv(comp);
					//EEpromData.Volt_A= ADE7758_avrms();
					//EEpromData.Volt_B= ADE7758_bvrms();
					//EEpromData.Volt_C= ADE7758_cvrms();
					}

return;	
}

float  ReadSuperHeat (xCompressor *comp){
	float Val, Next_val, Span, Saturation_Temp, decimals, work;
	int I;
	work = ((comp->Low_Press - 1.0) / 1.0);
	I = work;                                //drop decimals to get array index
	decimals = work - I; //restore decimals for interpolation between array points
	Val = pgm_read_float(&R22_Saturation_Temp[I]);           //get array value
	Next_val = pgm_read_float(&R22_Saturation_Temp[I + 1]);  //get Next array value
	Span = Next_val - Val;                  //Calc Span
	Saturation_Temp = Val + (Span * decimals);//interpolate between array points
		
	return(comp->Suc_Comp - Saturation_Temp);// Suct_T - Saturation_Temp;

}
float ReadSuperCool (xCompressor *comp){
	float Val, Next_val, Span, Saturation_Temp, decimals, work;
	int I;
	work = ((comp->High_Press - 1.0) / 1.0);

	I = work;                                //drop decimals to get array index

	decimals = work - I; //restore decimals for interpolation between array points
	Val = pgm_read_float(&R22_Saturation_Temp[I]);           //get array value
	Next_val = pgm_read_float(&R22_Saturation_Temp[I + 1]);  //get Next array value
	Span = Next_val - Val;                   //Calc Span
	Saturation_Temp = Val + (Span * decimals);//interpolate between array points
	
    return(Saturation_Temp - comp->Cond_Temp);// Suct_T - Saturation_Temp;	
}
//-------------------------------------------------------------
float  corectTrv(xCompressor *comp){
	static float Superheat_old=0;
	int SSet;
	float ds;
	float Superheat;
	Superheat=comp->SuperHeat;
	ds = Superheat - Superheat_old; // calculate the change in Superheat
	ds = ds * 2; // scaling factor for DS
	SSet=7;

	// Variable sampling rate --------------------------------------------
	int  k = 1; // Gain for the steps

	if ( Superheat - SSet > 3)
	{
		if (ds > 2 || ds < -2)

		{
			k = 2; // Gain for the steps
			
		}
	}

	if ( Superheat - SSet >= 5)
	{
		if (ds > 2 || ds < -2)
		{
			k = 3; // Gain for the steps
			
		}
	}

	//------------------------

	if (Superheat < SSet)
	{
		k = -1; // if the Superheat is too low, this gain changes the step direction
		Superheat = SSet + 10 - Superheat;
		ds = -ds;
	}
	
	Superheat_old = Superheat;
	
	if (ds > 20)
	{
		ds = 20;
	}

	 else if (ds < -20)
	{
		ds = -20;
	}
//--------------------------------------
	if (ds < -10)
	{
		if (Superheat == SSet)
		{
			return(-0.123 * ds * ds - 2.768 * ds - 30.62)* k ;
		}

		if (Superheat > SSet && Superheat <= SSet + 1)
		{
			return( 0.02416 * ds * ds * ds + 0.95688 * ds * ds + 12.69588 * ds + 44.31142)* k;
		}

		if (Superheat > SSet + 1 && Superheat <= SSet + 2)
		{
			return( 0.01715 * ds * ds * ds + 0.64880 * ds * ds + 8.39021 * ds + 27.61058)* k;
		}

		if (Superheat > SSet + 2 && Superheat <= SSet + 3)
		{
			return( -0.099 * ds * ds - 2.002 * ds - 16.444);
		}

		if (Superheat > SSet + 3 && Superheat <= SSet + 4)
		{
			return( -0.065 * ds * ds - 0.773 * ds - 4.451);
		}

		if (Superheat > SSet + 4 && Superheat <= SSet + 5)
		{
			return( 0.01315 * ds * ds * ds + 0.59145 * ds * ds + 10.05298 * ds + 54.52301)* k;
		}

		if (Superheat > SSet + 5 && Superheat <= SSet + 6)
		{
			return( 0.02485 * ds * ds * ds + 1.09724 * ds * ds + 17.05530 * ds + 89.43916)* k;
		}

		if (Superheat > SSet + 6 && Superheat <= SSet + 7)
		{
			return( 0.02257 * ds * ds * ds + 0.99967 * ds * ds + 15.71735 * ds + 86.11652)* k;
		}
		if (Superheat > SSet + 7 && Superheat <= SSet + 8)
		{
			return( 0.02199 * ds * ds * ds + 0.99841 * ds * ds + 16.07877 * ds + 91.69972)* k;
		}

		if (Superheat > SSet + 8 && Superheat <= SSet + 9)
		{
			return( 0.02293 * ds * ds * ds + 1.04454 * ds * ds + 16.79148 * ds + 97.65075)* k;
		}

		if (Superheat > SSet + 9)
		{
			return( 0.01242 * ds * ds * ds + 0.55907 * ds * ds + 9.57142 * ds + 67.21499)* k;
		}
	}

	else if (ds < 0 && ds >= -10)
	{
		if (Superheat == SSet)
		{
			return( 0.01259 * ds * ds * ds + 0.18896 * ds * ds + 2.12770 * ds - 0.01350)* k;
		}

		if (Superheat > SSet && Superheat <= SSet + 1)
		{
			return( 0.02252 * ds * ds * ds + 0.32376 * ds * ds + 2.48397 * ds + 3.72657)* k;
		}

		if (Superheat > SSet + 1 && Superheat <= SSet + 2)
		{
			return( 0.02106 * ds * ds * ds + 0.30427 * ds * ds + 2.44311 * ds + 6.31417)* k;
		}

		if (Superheat > SSet + 2 && Superheat <= SSet + 3)
		{
			return( 0.02106 * ds * ds * ds + 0.32749 * ds * ds + 2.67528 * ds + 8.74842)* k;
		}

		if (Superheat > SSet + 3 && Superheat <= SSet + 4)
		{
			return( 0.02252 * ds * ds * ds + 0.35182 * ds * ds + 2.76462 * ds + 11.25664)* k;
		}

		if (Superheat > SSet + 4 && Superheat <= SSet + 5)
		{
			return( 0.01251 * ds * ds * ds + 0.18754 * ds * ds + 2.12069 * ds + 14.98441)* k;
		}

		if (Superheat > SSet + 5 && Superheat <= SSet + 6)
		{
			return(0.062 * ds * ds + 1.818 * ds + 15.22)* k;
		}

		if (Superheat > SSet + 6 && Superheat <= SSet + 7)
		{
			return( 0.097 * ds * ds + 1.941 * ds + 16.00)* k;
		}

		if (Superheat > SSet + 7 && Superheat <= SSet + 8)
		{
			return( 0.01598 * ds * ds * ds + 0.35913 * ds * ds + 2.90559 * ds + 17.70294)* k;
		}

		if (Superheat > SSet + 8 && Superheat <= SSet + 9)
		{
			return( 0.02358 * ds * ds * ds + 0.48252 * ds * ds + 3.36313 * ds + 20.09441)* k;
		}

		if (Superheat > SSet + 9)
		{
			return( 0.01076 * ds * ds * ds + 0.28823 * ds * ds + 2.82362 * ds + 25.08042)* k;
		}
	}

	else if (ds >= 0 && ds <= 10)
	{
		if (Superheat == SSet)
		{
			return( -2 * .00001 * ds * ds + 1.406 * ds)* k;
		}

		if (Superheat > SSet && Superheat <= SSet + 1)
		{
			return( 0.062 * ds * ds + 0.570 * ds + 3.280)* k;
		}

		if (Superheat > SSet + 1 && Superheat <= SSet + 2)
		{
			return( 0.099 * ds * ds - 0.019 * ds + 6.338)* k;
		}

		if (Superheat > SSet + 2 && Superheat <= SSet + 3)
		{
			return( 0.121 * ds * ds - 0.409 * ds + 9.164)* k;
		}

		if (Superheat > SSet + 3 && Superheat <= SSet + 4)
		{
			return( 0.128 * ds * ds - 0.340 * ds + 11.4)* k;
		}

		if (Superheat > SSet + 4 && Superheat <= SSet + 5)
		{
			return( 0.128 * ds * ds - 0.382 * ds + 15.51)* k;
		}

		if (Superheat > SSet + 5 && Superheat <= SSet + 6)
		{
			return( 0.128 * ds * ds - 0.382 * ds + 15.51)* k;
		}

		if (Superheat > SSet + 6 && Superheat <= SSet + 7)
		{
			return( 0.137 * ds * ds - 0.652 * ds + 16.63)* k;
		}

		if (Superheat > SSet + 7 && Superheat <= SSet + 8)
		{
			return( 0.142 * ds * ds - 0.630 * ds + 17.7)* k;
		}

		if (Superheat > SSet + 8 && Superheat <= SSet + 9)
		{
			return( 0.126 * ds * ds - 0.683 * ds + 20.4)* k;
		}

		if (Superheat > SSet + 9)
		{
			return( 0.027 * ds * ds - 0.277 * ds + 25.29)* k;
		}
	}

	else if (ds > 10)
	{
		if (Superheat == SSet)
		{
			return( 0.137 * ds * ds - 3.245 * ds + 34.51)* k;
		}

		if (Superheat > SSet && Superheat <= SSet + 1)
		{
			return( 0.137 * ds * ds - 3.245 * ds + 34.51)* k;
		}

		if (Superheat > SSet + 1 && Superheat <= SSet + 2)
		{
			return( 0.158 * ds * ds - 4.086 * ds + 42.41)* k;
		}

		if (Superheat > SSet + 2 && Superheat <= SSet + 3)
		{
			return( 0.027 * ds * ds * ds - 1.086 * ds * ds + 14.32 * ds - 44.48)* k;
		}

		if (Superheat > SSet + 3 && Superheat <= SSet + 4)
		{
			return( 0.026 * ds * ds * ds - 1.084 * ds * ds + 14.27 * ds - 40.99)* k;
		}

		if (Superheat > SSet + 4)
		{
			return 25* k;
		}
	}

	
	return 0;

}
