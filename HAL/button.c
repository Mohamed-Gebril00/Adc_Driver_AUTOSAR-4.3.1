/*
 * button.c
 *
 *  Created on: Jan 27, 2019
 *      Author: Gebro
 */
#include "button.h"
#define MAX_DIGITAL_VALUE 4095
#define MAX_BUTTONS_NUMBER 8


extern ButtonGroup Button_GroupParameters[NUMBER_OF_BUTTONS_GROUPS];

Button_FunctionValidation Read_Button(uint8_t ButtonGroupIdx, Button_StateType* Button_PTRState)
{
  Adc_StatusType Adc_Statues;
	Adc_FunctionValidation AdcFunc_Validate=ADC_OK;
	Button_FunctionValidation ButtonFunc_Validate = Button_OK;
	
	ButtonGroup* GrpPtr= &Button_GroupParameters[ButtonGroupIdx];
	
	uint16_t Adc_ReadingsBuffer[MAX_BUTTONS_NUMBER]={0};
	uint8_t i = 0;
    if(ButtonGroupIdx >= NUMBER_OF_GROUPS)
        return Button_E_PARAM_GROUP;
    else
    {
			AdcFunc_Validate = Adc_ReadGroup(GrpPtr->AdcGroupIdx,Adc_ReadingsBuffer);
			if(AdcFunc_Validate != ADC_OK)
			{
				ButtonFunc_Validate = Button_E_ADC;
			}
			else 
			{
				for(i = 0;i < GrpPtr->Buttons_Number;i++)
				{
					Adc_ReadingsBuffer[i] = (Adc_ReadingsBuffer[i]*MAX_VOLT)/MAX_DIGITAL_VALUE;
				}
				AdcFunc_Validate = Adc_StartGroupConversion(GrpPtr->AdcGroupIdx);
			}
    }
	return ButtonFunc_Validate;
}


