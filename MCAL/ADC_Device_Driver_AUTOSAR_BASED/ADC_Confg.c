/**
  * ADC Configuration (.c) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */

#include "ADC_Confg.h"
void sample();
AdcGroup Adc_GroupParameters[NUMBER_OF_GROUPS]=
{
{1, ADC_MODULE0,  ADC_SEQ3 ,Adc_Continous,ADC_IDLE,
	{ADC_CH0}, 
},


{1, ADC_MODULE0,  ADC_SEQ3, Adc_Continous, ADC_IDLE,
	{ADC_CH1}},

};
