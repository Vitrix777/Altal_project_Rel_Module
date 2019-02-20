#include "Errors.h"

extern  bool def;
int Check_Erors(xCompressor *comp,xEEdata *EElimit)
{
		if( comp->Dhw_Temp<-100){return (10+comp->Index);}
		else if(comp->Bac_Temp<-100) {return (20+comp->Index);}
		else if (comp->Heat_Out<-100) {return (30+comp->Index);}
		else if (comp->Heat_In<-100){return (40+comp->Index);}
		else if(comp->Src_In<-100) {return (50+comp->Index);}
		else if (comp->Src_Out<-100) {return (60+comp->Index);}
		else if (comp->Ex_Comp<-100) {return (70+comp->Index);}
		else if(comp->Suc_Comp<-100) {return (80+comp->Index);}
		else if (comp->Cond_Temp<-100){return (90+comp->Index);}
		////
		else if ((PIN_HEAT_FLOW_1>0) && comp->srcPOn==1 && comp->Index==1) {return (100+comp->Index);} 
		else if ((PIN_HEAT_FLOW_2>0) && comp->srcPOn==1 && comp->Index==2) {return (100+comp->Index);} 	
		else if ((PIN_SOURCE_FLOW>0) && comp->srcPOn==1 && comp->Index==1) {return (110+comp->Index);} 
		else if ((PIN_SOURCE_FLOW>0) && comp->srcPOn==1 && comp->Index==2) {return (110+comp->Index);}	
		 //else if ( comp->High_Press > 0){return (140+comp->Index);}
		 //else if ( comp->Low_Press>0 ) {return (120+comp->Index);}
		 else if ( comp->Low_Press< EElimit->LP_Min ) {return (120+comp->Index);}
		 else if ( comp->High_Press > EElimit->HP_Max){return (140+comp->Index);}	
		 else if ( comp->Bac_Temp > EElimit->HEAT_Max && comp->Heating==1){return (160+comp->Index);}
		//else if ( comp->Bac_Temp < EElimit->HEAT_Min && comp->Cooling==1) {return (170+comp->Index);}	
		//else if ( comp->Bac_Temp < EElimit->HEAT_Min && comp->Heating==1) {return (170+comp->Index);}
		//else if ( comp->Bac_Temp > EElimit->Cool_Max && comp->Cooling==1){return (180+comp->Index);}
		else if ( comp->Bac_Temp < EElimit->Cool_Min && comp->Cooling==1){return (170+comp->Index);}
		//else if ( comp->SuperHeat < EElimit->MinSuperheat && comp->checkTrv) {return (180+comp->Index);}
		//else if ( comp->SuperHeat > EElimit->MaxSuperheat && comp->checkTrv && !comp->def) {return (190+comp->Index);}
		//else if ( comp->SuperCool < EElimit->MinSupercool && comp->checkTrv){return (200+comp->Index);}
		//else if ( comp->SuperCool > EElimit->MaxSupercool && comp->checkTrv && !comp->def){return (210+comp->Index);}
		else if ( comp->Ex_Comp > EElimit->ExhaustMax){return 220;}	
		//else if (abs(comp->Src_In - comp->Src_Out) > EElimit->Delta_source_max && comp->checkErrors && EEpromData.AireToWater==0) {return (230+comp->Index);}
		//else if (abs(comp->Src_In - comp->Src_Out) <  EElimit->Delta_source_min && comp->checkErrors && EEpromData.AireToWater==0) {return (240+comp->Index);}
		else if (abs(comp->Heat_In - comp->Heat_Out)  > EElimit->Delta_heating_max && comp->checkErrors) {return (250+comp->Index);}
		else if (abs(comp->Heat_In - comp->Heat_Out) < EElimit->Delta_heating_min &&  comp->checkErrors) {return (260+comp->Index);} 
		else if (!(ADE7758_getStatus() & ZXA) && EElimit->Power_380==1 ){return 271;} // ADE7758_resetStatus();
		else if (!(ADE7758_getStatus() & ZXB) && EElimit->Power_380==1 ){return 281;}//ADE7758_resetStatus();
		else if (!(ADE7758_getStatus() & ZXC) && EElimit->Power_380==1 ){return 291;}//ADE7758_resetStatus();
		else if (!(ADE7758_getStatus() & SEQERR) && EElimit->Power_380==1){return 301;}//ADE7758_resetStatus();
		//else if (comp->Src_In > EElimit->Source_Max){return (400+comp->Index);}
		//else if (comp->Src_In < EElimit->Source_Min){return (410+comp->Index);}			
		else {return 0;}
}  