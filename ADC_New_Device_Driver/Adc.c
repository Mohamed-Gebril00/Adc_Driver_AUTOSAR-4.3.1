#include "Adc.h"

// ***************************************************************************************************************************
// This Function is used to initialize ADC Modules and Initialize group states with ADC_IDLE
// *************************************************************************************************************************** 
Adc_FunctionValidation Adc_Init(void)
{
		Adc_FunctionValidation Function_ValidationReturn = ADC_OK;
		uint32_t delay;
		uint8_t Adc_LoopIdx=0;
		RCGCADC_REG |= ADC0_BITFIELD + ADC1_BITFIELD; // enable ADC Modules Clock
		delay = RCGCADC_REG;
		
		// Adjust sampling speed for module 1 and module 2
		RCGC0 |= (ADC_MAX_CONVERSION_SPEED << MAXADC0SPD_BITNUM) + (ADC1_MAX_CONVERSION_SPEED << MAXADC1SPD_BITNUM);
		
		const AdcGroup *GrpPtr;
		
		for ( Adc_LoopIdx = 0; Adc_LoopIdx < NUMBER_OF_GROUPS; Adc_LoopIdx++)
		{
			GrpPtr = &AdcGroup[Adc_LoopIdx];
			GrpPtr -> AdcStatus = ADC_IDLE;
		}
		return Function_ValidationReturn;
}

// ***************************************************************************************************************************
// This Function is used to Start group conversion assigning channels to sample sequencers and set sequencers control
// ***************************************************************************************************************************
Adc_FunctionValidation Adc_StartGroupConversion(void)
{
	
}