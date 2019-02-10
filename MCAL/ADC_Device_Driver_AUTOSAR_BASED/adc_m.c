#include "adc_m.h"

// flags to know that the corrsponding groups has finished
static uint8_t AdcGroup_ConversionComplete[256] = {0};

//**************************************************************************************************
// Buffers for Groups.
uint16_t AdcGroup0_Buffer[ADC_GROUP0_BUFFER_SIZE]; 
uint16_t AdcGroup1_Buffer[ADC_GROUP1_BUFFER_SIZE]; 
uint16_t AdcGroup2_Buffer[ADC_GROUP2_BUFFER_SIZE]; 
uint16_t AdcGroup3_Buffer[ADC_GROUP3_BUFFER_SIZE]; 
uint16_t AdcGroup4_Buffer[ADC_GROUP4_BUFFER_SIZE]; 
uint16_t AdcGroup5_Buffer[ADC_GROUP5_BUFFER_SIZE]; 
uint16_t AdcGroup6_Buffer[ADC_GROUP6_BUFFER_SIZE]; 
uint16_t AdcGroup7_Buffer[ADC_GROUP7_BUFFER_SIZE]; 


uint16_t* AdcGroup_BufferPtr[8]={
	AdcGroup0_Buffer,
	AdcGroup1_Buffer,
	AdcGroup2_Buffer,
	AdcGroup3_Buffer,
	AdcGroup4_Buffer,
	AdcGroup5_Buffer,
	AdcGroup6_Buffer,
	AdcGroup7_Buffer
};
	

//**************************************************************************************************
// Call Backs for Groups
void AdcGroup0_CallBack(void);
void AdcGroup1_CallBack(void);
void AdcGroup2_CallBack(void);
void AdcGroup3_CallBack(void);
void AdcGroup4_CallBack(void);
void AdcGroup5_CallBack(void);
void AdcGroup6_CallBack(void);
void AdcGroup7_CallBack(void);


// Initialization function used in intialize the buffers for the group.
AdcManager_FunctionReturn AdcManager_Init()
{
	 uint8_t loop_idx; 
	 AdcManager_FunctionReturn Function_Validation=ADC_M_OK;
	 for(loop_idx = 0; loop_idx < NUMBER_OF_ADC_GROUPS; loop_idx++)
	 {
			Adc_SetupResultBuffer(loop_idx, AdcGroup_BufferPtr[loop_idx]);
	 }
	 return Function_Validation;
}

//***************************************************************************************************
// Cyclic function used in start the conversion of the Adc_Groups one at a time cyclic.
AdcManager_FunctionReturn AdcManager_StartConversion()
{
	static uint8_t ADCGroup_To_StartConversion = 0;
	AdcManager_FunctionReturn ADCManager_ValidationCheck = ADC_M_OK;
	Adc_FunctionValidation ADC_FunctionCheck = ADC_OK;
	ADC_FunctionCheck = Adc_StartGroupConversion(ADCGroup_To_StartConversion);
	if(ADC_FunctionCheck != ADC_OK)
	{
		ADCManager_ValidationCheck = ADC_M_E_START_CONVERSION;
	}
	else
	{
		 ADCGroup_To_StartConversion++;
		 if(ADCGroup_To_StartConversion >= NUMBER_OF_ADC_GROUPS)
		 {
			 ADCGroup_To_StartConversion=0;
		 }
	 }
	return ADCManager_ValidationCheck;
 }

 
 
AdcManager_FunctionReturn Adc_GetData(uint8_t Adc_GroupIdx,uint8_t ADC_DefinitionIdx,uint16_t* Adc_DataBuffer)
{
	 Adc_FunctionValidation Function_Validation = ADC_OK;
	 AdcManager_FunctionReturn Manager_Function_Validation = ADC_M_OK;
	 Adc_StatusType Adc_Status;
	 uint8_t loop_idx = 0;
	 const AdcGroup* GrpPtr = &Adc_GroupParameters[Adc_GroupIdx];
	 uint8_t Number_Of_Samples =  GrpPtr->AdcStreamingNumSamples;
	 
	 if(Adc_GroupIdx >= NUMBER_OF_ADC_GROUPS){
		 Manager_Function_Validation = ADC_M_E_PARAM_GROUP;
	 }
	 
	 Adc_Status = Adc_GetGroupStatus(Adc_GroupIdx);
	 
	 if(Adc_Status == ADC_STREAM_COMPLETED)
	 {
		 for(loop_idx = 0;loop_idx < Number_Of_Samples;loop_idx++)
		 {
			 *(Adc_DataBuffer+loop_idx) = *((GrpPtr->Group_Buffer) + (loop_idx+Number_Of_Samples));
		 }
		 Manager_Function_Validation = ADC_M_OK;
	 }
	 
	 else 
	 {
		 Manager_Function_Validation = ADC_M_NOK;
	 }
	 
	
	 return Manager_Function_Validation;
}
	 

//**************************************************************************************************
// Call Backs for Groups
void AdcGroup0_CallBack(void)
{
	AdcGroup_ConversionComplete[0] = 1;
}
void AdcGroup1_CallBack(void)
{
	AdcGroup_ConversionComplete[1] = 1;
}

void AdcGroup2_CallBack(void)
{
	AdcGroup_ConversionComplete[2] = 1;
}

void AdcGroup3_CallBack(void)
{
	AdcGroup_ConversionComplete[3] = 1;
}
void AdcGroup4_CallBack(void)
{
	AdcGroup_ConversionComplete[4] = 1;
}

void AdcGroup5_CallBack(void)
{
	AdcGroup_ConversionComplete[5] = 1;
}

void AdcGroup6_CallBack(void)
{
	AdcGroup_ConversionComplete[6] = 1;
}

void AdcGroup7_CallBack(void)
{
	AdcGroup_ConversionComplete[7] = 1;
}

			
