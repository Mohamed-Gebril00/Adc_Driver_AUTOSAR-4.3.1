#include "Adc.h"

const AdcGroup Adc_GroupParameters[NUMBER_OF_GROUPS]={
	{
		ADC_MODULE0,
		ADC_CONV_MODE_ONE_SHOT,
		ADC_ONE_SAMPLE,
		{ADC_CH0, ADC_CH1, -1, -1},
		ADC_IDLE,
		0
	}
};