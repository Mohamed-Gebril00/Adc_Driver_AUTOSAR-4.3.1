/**
  * ADC Configuration (.c) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */

#include "ADC_Confg.h"


AdcGroup Adc_GroupParameters[NUMBER_OF_ADC_GROUPS]=
{
{1, ADC_MODULE0,0,1, ADC_SEQ3 ,Adc_Continous,ADC_IDLE,ADC_GROUP_NOTIFICATION_DISABLED,ADC_GROUP_PRI_0,
	AdcGroup0_CallBack, {ADC_CH0}, NULL
}};
