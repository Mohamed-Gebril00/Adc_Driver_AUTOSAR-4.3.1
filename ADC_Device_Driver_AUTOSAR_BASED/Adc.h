/**
  * ADC interface file (prototypes)(.h) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */


#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#include "ADC_Confg.h" 
#include "M4MemMap.h"
#include "Adc_MemMap.h"


typedef enum{ADC_OK=0, ADC_E_PARAM_GROUP, ADC_E_WRONG_TRIGG_SRC, ADC_E_BUSY, ADC_NOK} Adc_FunctionValidation;
typedef enum{ADC_ModuleOff=0, ADC_ModuleOn} ADC_ConfigureEnDis;
typedef enum {ADC_CH0=0, ADC_CH1, ADC_CH2, ADC_CH3, ADC_CH4, ADC_CH5, ADC_CH6, ADC_CH7, ADC_CH8, ADC_CH9, ADC_CH10, 
ADC_CH11} Adc_Channels;
typedef enum {Adc_OneShot=0, ADC_AnalogComparator0, ADC_AnalogComparator1, ADC_GPIOPins=4, ADC_Timer, 
ADC_PWMGeneratot0, ADC_PWMGenerator1, ADC_PWMGenerator2, ADC_PWMGenerator3, Adc_Continous=15} Adc_TriggerSourceType;
typedef enum {ADC_125K=0,ADC_250K, ADC_500K, ADC_1M} ADC_MaxSamplingPerSecond;
typedef enum {ADC_IDLE=0, ADC_BUSY, ADC_COMPLETED, ADC_STREAM_COMPLETED} Adc_StatusType;
typedef enum {ADC_MODULE0=0, ADC_MODULE1} Adc_ModuleType;
typedef enum {ADC_SEQ0=0, ADC_SEQ1, ADC_SEQ2, ADC_SEQ3} Adc_SequencerType;


			 
typedef struct 
{
	uint8_t AdcChannelsNum;
	Adc_ModuleType AdcModule;
	Adc_SequencerType AdcSequencer;
	Adc_TriggerSourceType Adc_TriggerSource;	
	Adc_StatusType Adc_Status;
	Adc_Channels AdcGroupDefinition[8];
	
}AdcGroup;
	



/********************************************************************************************************
 * Initializing the ADC modules and sequencer
 ********************************************************************************************************/
Adc_FunctionValidation ADC_Init(void);




/*********************************************************************************************************
 * Get Sample From FIFO
 *********************************************************************************************************/
Adc_FunctionValidation  Adc_ReadGroup(uint8_t AdcGroupIdx, uint16_t *DataBufferPtr);


/*********************************************************************************************
  * This Function is used in getting trigger state for the running sequence 
  * return 0 completed successfully
	* return 1 terminated unsuccessful
	* takes the adc group idx, a pointer to a variable
  * return the trigger state in the variable pointed to it by the second parameter
  ********************************************************************************************/
Adc_StatusType Adc_GetGroupStatus(uint8_t AdcGroupIdx);


/**********************************************************************************************
 * Initiate sequence Conversion by setting the SSn bit in ADCPSSI register
 **********************************************************************************************/
Adc_FunctionValidation Adc_StartGroupConversion(uint8_t AdcGroupIdx);

/*************************************************************************************************
 * This Function Enable the interrupt 
 *************************************************************************************************/
void Adc_EnableGroupNotification(uint8_t AdcGroupIdx, uint8_t PRI);

	
 
#endif