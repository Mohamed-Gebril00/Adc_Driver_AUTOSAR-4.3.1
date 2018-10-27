#ifndef ADC_H
#define ADC_H
#include <stdint.h>
#include "Adc_Reg.h"

typedef enum {ADC_OK=0 ,ADC_NOK} Adc_FunctionValidation;
typedef enum {ADC_ONE_SAMPLE=0, ADC_TWO_SAMPLE, ADC_THREE_SAMPLE, ADC_FOUR_SAMPLE, ADC_FIVE_SAMPLE, ADC_SIX_SAMPLE, ADC_SEVEN_SAMPLE, ADC_EIGHT_SAMPLES}Adc_StreamingNumSamplesType;
typedef enum{ADC_ACCESS_MODE_SINGLE = 0, ADC_ACCESS_MODE_STREAMING} Adc_GroupAccessModeType;
typedef enum{ADC_IDLE=0, ADC_BUSY, ADC_COMPLETED, ADC_STREAM_COMPLETED} Adc_StatusType;
typedef enum{ ADC_CONV_MODE_ONE_SHOT=0, ADC_CONV_MODE_CONTINUOUS} Adc_GroupConvModeType;
typedef enum { ADC_CH0, ADC_CH1, ADC_CH2, ADC_CH3, ADC_CH4, ADC_CH5, ADC_CH6, ADC_CH7, ADC_CH8, ADC_CH9, ADC_CH10, ADC_11} Adc_Channels;
typedef enum {ADC_125K=0,ADC_250K, ADC_500K, ADC_1M} Adc_MaxConversionSpeedType;
typedef enum {ADC_MODULE0=0, ADC_MODULE1} Adc_ModuleType;
	



typedef struct {
	Adc_ModuleType AdcModule;
	Adc_GroupConvModeType AdcGroupConversionMode;
	Adc_StreamingNumSamplesType AdcStreamingNumSamples;
	Adc_Channels AdcGroupDefinition[4];
	Adc_StatusType AdcStatus;
	
	uint8_t AdcGroupId;
	
}AdcGroup;

Adc_FunctionValidation Adc_Init(void);
Adc_FunctionValidation Adc_StartGroupConversion(void);

#endif