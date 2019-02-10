#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H

#include <stdint.h>
#include "ADC.h"
#include "ADC_Confg.h"
#include "adc_M_confg.h"

typedef enum {Manager_Uninit=0, Manager_Init, Manager_Busy, Manager_ConversionFinished, Manager_Idle}AdcManager_State;

typedef enum {ADC_M_OK=0, ADC_M_E_BUFFER_NOT_INITIALIZED, ADC_M_E_CALLBACK_NOT_INITIALIZED, 
ADC_M_E_START_CONVERSION, ADC_M_NOK, ADC_M_E_PARAM_GROUP} AdcManager_FunctionReturn;

extern const AdcGroup Adc_GroupParameters[NUMBER_OF_ADC_GROUPS];

AdcManager_FunctionReturn AdcManager_Init();
AdcManager_FunctionReturn AdcManager_StartConversion();
AdcManager_FunctionReturn Adc_GetData(uint8_t ADC_GroupIdx,uint8_t ADC_DefinitionIdx,uint16_t* ADCData_Buffer);


#endif