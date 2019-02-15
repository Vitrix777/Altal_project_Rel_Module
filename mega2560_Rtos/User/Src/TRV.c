#include  "Trv.h"

extern xCompressor Comp_1,Comp_2;
unsigned char cw_dir_1 [4]={0b00000011,0b00000110,0b00001100,0b00001001};//0b00000001,0b00000010,0b00000100,0b00001000//0b00000001,0b00000011,0b00000010,0b00000110,0b00000100,0b00001100,0b00001000,0b00001001
unsigned char ccw_dir_1[4]={0b00001001,0b00001100,0b00000110,0b00000011};//0b00001000,0b00000100,0b00000010,0b00000001
unsigned char cw_dir_2 [4]={0b00110000,0b01100000,0b11000000,0b10010000};//0b00000001,0b00000010,0b00000100,0b00001000//0b00000001,0b00000011,0b00000010,0b00000110,0b00000100,0b00001100,0b00001000,0b00001001
unsigned char ccw_dir_2[4]={0b10010000,0b11000000,0b01100000,0b00110000};//0b00001000,0b00000100,0b00000010,0b00000001	
	
const portBASE_TYPE uxTrv_1_ID =26;
const portBASE_TYPE uxTrv_2_ID =27;

void activateTrvTimer(xCompressor *comp)
{
	if(comp->Index==1)
	{		
		xComutateTrv_1   = xTimerCreate("ComTrv_1",   50, pdTRUE, (void*) &uxTrv_1_ID, ComutateTrv);
	}
	  else if(comp->Index==2)
	{	
		xComutateTrv_2   = xTimerCreate("ComTrv_2",  50, pdTRUE, (void*) &uxTrv_2_ID, ComutateTrv);	
	}
	
}
//----------------------------------------------------------------------------------------------------------
void startTrvTimer(xCompressor *comp)
{
	if(comp->Index==1)
	{
		xTimerStart(xComutateTrv_1, 0);
	}
	else if(comp->Index==2)
	{
		xTimerStart(xComutateTrv_2, 0);
	}
	
}
void deleteTrvTimer(xCompressor *comp)
{
	if(comp->Index==1)
	{
		xTimerDelete(xComutateTrv_1,0);
	}
	if(comp->Index==2)
	{
		xTimerDelete(xComutateTrv_2,0);
	}
}
void stopTrvTimer(xCompressor *comp)
{
	if(comp->Index==1)
	{
		if( xTimerIsTimerActive( xComutateTrv_1 ) != pdFALSE )	xTimerStop(xComutateTrv_1, 1);
	}
	if(comp->Index==2)
	{
		if( xTimerIsTimerActive( xComutateTrv_2 ) != pdFALSE )	xTimerStop(xComutateTrv_2, 1);
	}
	
}
//-----------------------------------------------------------------------------------------------------------
void ComutateTrv(TimerHandle_t xTimer)
{
portBASE_TYPE *pxTimerID;
pxTimerID = pvTimerGetTimerID(xTimer);

	if((*pxTimerID) == uxTrv_1_ID)
	{
		static unsigned char step_index=0;
						PORTA &= 0xf0;
						if(Comp_1.Steps>0)		
						{ 
							PORTA|=cw_dir_1[step_index++];						
							Comp_1.Steps--;  Comp_1.Trv_position++; if(Comp_1.Trv_position>240){Comp_1.Trv_position=240;}//xSerialxPrintf(&xSerial3Port,"Trv_position_1 = %d \r\n", Comp_1.Trv_position);//		
						}
						
						 else if(Comp_1.Steps<0)
						 {	PORTA=ccw_dir_1 [step_index++];
						
						 Comp_1.Steps++; Comp_1.Trv_position--; if(Comp_1.Trv_position<0){Comp_1.Trv_position=0;}//xSerialxPrintf(&xSerial3Port,"Trv_position_1 = %d \r\n", Comp_1.Trv_position);//
						 }
						 else if(Comp_1.Steps==0 && Comp_1.calibrate){Comp_1.Steps=240;Comp_1.calibrate=false;}
							 
						 else  {PORTA &= 0xf0; Comp_1.calibrate=false;}
							 
						if(step_index >= 4){step_index=0;}
		
	
	}
	if((*pxTimerID) == uxTrv_2_ID)
	{
	  static unsigned char step_index=0;
			PORTA &= 0x0f;
		if(Comp_2.Steps>0)		
		{
			PORTA|= cw_dir_2 [step_index++];
			
			Comp_2.Steps--;  Comp_2.Trv_position++; if(Comp_2.Trv_position>240){Comp_2.Trv_position=240;}//avrSerialPrintf("Trv_position_2 = %d \r\n", Comp_2.Trv_position);
		}//PORTA = cw_dir_2[step_index++];
		else if(Comp_2.Steps<0)			
			{
				PORTA=ccw_dir_2 [step_index++];
				 Comp_2.Steps++; Comp_2.Trv_position--; if(Comp_2.Trv_position<0)Comp_2.Trv_position=0;//avrSerialPrintf("Trv_position_2 = %d \r\n", Comp_2.Trv_position);
			}//PORTA = ccw_dir_2[step_index++];
		else if(Comp_2.Steps==0 && Comp_2.calibrate)  {Comp_2.Steps=240;Comp_2.calibrate=false;}
			
		else  {PORTA &= 0x0f; Comp_2.calibrate=false;}	
			
		if(step_index >= 4)step_index=0;
		
	}
}
//----------------------------------------------------------------------------------------------------------
int Fuzzy(xCompressor *comp,int corection)
{
	int steps=0;
	//static int old_corection=0;
	//static int old_corection_1=0;
	if(comp->Index==2)xSerialxPrintf(&xSerial3Port,"corection_2 = %d \r\n", corection);
	if(comp->Src_In<=10 && comp->Heating==1 ){steps=(comp->Src_In)/10 * 100 ;}
	else if(comp->Src_Out<=10 && comp->Cooling==1){steps=(comp->Src_Out)/10 * 100 ;}
		//if(old_corection!=corection){comp->trvCorection=corection-steps;old_corection=corection;}
		//xSerialxPrintf(&xSerial3Port,"steps_%d = %d \r\n",comp->Index, steps); }//comp->trvCorection;
	else if(comp->Src_In>10 && comp->Src_In<=20 && comp->Heating==1)  {steps = (comp->Src_In-10)/10 * 100 + 100;}
	else if(comp->Src_Out>10 && comp->Src_Out<=20 && comp->Cooling==1)  {steps = (comp->Src_Out-10)/10 * 100 + 100;}	
		//if(old_corection!=corection){comp->trvCorection=corection-steps;old_corection=corection;}
		//xSerialxPrintf(&xSerial3Port,"steps_%d = %d \r\n",comp->Index, steps); }//;
	else if(comp->Src_In >20 && comp->Src_In<=30 && comp->Heating==1) {steps = (comp->Src_In-20)/10 * 100 + 150;}
	else if(comp->Src_Out >20 && comp->Src_Out<=30 && comp->Cooling==1) {steps = (comp->Src_Out-20)/10 * 100 + 150;}	
		
		//if(old_corection!=corection){comp->trvCorection=corection-steps;old_corection=corection;}
		//xSerialxPrintf(&xSerial3Port,"steps_%d = %d \r\n",comp->Index, steps);}// comp->trvCorection;
	else if(comp->Src_In>30 && comp->Src_In<=40 && comp->Heating==1)  {steps = (comp->Src_In-30)/10 * 100 + 200;}
	else if(comp->Src_Out>30 && comp->Src_Out<=40 && comp->Cooling==1)  {steps = (comp->Src_Out-30)/10 * 100 + 200;}	
		//if(old_corection!=corection){comp->trvCorection=corection-steps;old_corection=corection;}		
		//xSerialxPrintf(&xSerial3Port,"steps_%d = %d \r\n",comp->Index, steps);}//comp->trvCorection;
		
	//if(old_corection!=corection && comp->Index==1){comp->trvCorection=corection-steps;old_corection=corection;}
		 //if(old_corection_2!=corection && comp->Index==2){comp->trvCorection=corection-steps;old_corection_2=corection;}
	//xSerialxPrintf(&xSerial3Port,"TRVcorection_%d = %d \r\n",comp->Index, corection);
			
	
	
	comp->trvCorection=corection-steps;
	if(comp->Index==2)xSerialxPrintf(&xSerial3Port,"TRVcorection_2 = %d \r\n", comp->trvCorection);
	if(comp->Index==1)xSerialxPrintf(&xSerial3Port,"TRVcorection_1 = %d \r\n", comp->trvCorection);
	steps=steps+comp->trvCorection;
	comp->Steps=comp->Trv_position;		
	//comp->Steps=steps - comp->Trv_position;
xSerialxPrintf(&xSerial3Port,"Steps = %d \r\n", comp->Steps);
xSerialxPrintf(&xSerial3Port,"Trv_position = %d \r\n", comp->Trv_position);	
return (steps - comp->Trv_position);//comp->Steps;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void calibration(xCompressor *comp)
{
	comp->calibrate=true;
	comp->Steps=-240;
	comp->Trv_position=240;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
