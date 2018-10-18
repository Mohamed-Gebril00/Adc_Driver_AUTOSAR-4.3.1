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
#include <stdarg.h>
#include "ADC_REG.h"
#include "ADC_Confg.h" 
#include "../interrupt/interrupt.h"



// Errors' names that maybe encountred
typedef enum{ADC_OK=0, Invalid_GroupNumber, Invalid_SS0SampleNumbers, Invalid_SS1SampleNumbers,
	Invalid_SS2SampleNumbers, Invalid_SS3SampleNumbers,ADC_NOK} ADC_FunctionReturn;

// Configure to initialie the module
typedef enum{ADC_ModuleOff=0, ADC_ModuleOn} ADC_ConfigureEnDis;

// This enum too choose the input channel. 
typedef enum {ADC_CH0=0, ADC_CH1, ADC_CH2, ADC_CH3, ADC_CH4, ADC_CH5, ADC_CH6, ADC_CH7, ADC_CH8, ADC_CH9, ADC_CH10, 
ADC_CH11} ADC_Channel;

// This enum is used the trigger event for each sequencer. 
typedef enum {ADC_Processor=0, ADC_AnalogComparator0, ADC_AnalogComparator1, ADC_GPIOPins=4, ADC_Timer, 
ADC_PWMGeneratot0, ADC_PWMGenerator1, ADC_PWMGenerator2, ADC_PWMGenerator3, ADC_Always=15} ADC_TriggerEvent;

// This enum used in specifying the maximum sampling speed for the ADC modules (used in RCGC0 bits 8, 9 (for module 0), 10, 11 (for module 1)).
typedef enum {ADC_125K=0,ADC_250K, ADC_500K, ADC_1M} ADC_MaxSamplingPerSecond;

// This enum contains the 16 different probabilities that (a 4 bit sample control may have).
typedef enum {NotempNointNoendNodiff=0, NoTempNoIntNoEndDiff,NotempNointEndNodiff,NotempNointEndDiff,
			 NotempIntNoendNodiff,NotempIntNoendDiff,NotempIntEndNodiff,NotempIntEndDiff,
			 TempNointNoendNodiff,TempNointNoendDiff,TempNointEndNodiff,TempNointEndDiff,
			 TempIntNoendNodiff,TempIntNoendDiff,TempIntEndNodiff,TempIntEndDiff}ADC_SampleSeqControlBitsCombination;

			 
typedef struct 
{
	uint8_t ADC_ModuleId;
	uint8_t ADC_SSId;
	
	ADC_TriggerEvent ADC_TriggeringEvent;
	
	ADC_SampleSeqControlBitsCombination sample0;
	ADC_SampleSeqControlBitsCombination sample1;
	ADC_SampleSeqControlBitsCombination sample2;
	ADC_SampleSeqControlBitsCombination sample3;
	ADC_SampleSeqControlBitsCombination sample4;
	ADC_SampleSeqControlBitsCombination sample5;
	ADC_SampleSeqControlBitsCombination sample6;
	ADC_SampleSeqControlBitsCombination sample7;	
	
	ADC_Channel Sample0_Channel;
	ADC_Channel Sample1_Channel;
	ADC_Channel Sample2_Channel;
	ADC_Channel Sample3_Channel;
	ADC_Channel Sample4_Channel;
	ADC_Channel Sample5_Channel;
	ADC_Channel Sample6_Channel;
	ADC_Channel Sample7_Channel;
}ADC_Group;
	
extern const ADC_Group ADC_GroupConfg[ADCNUMBEROFGROUPS];



/********************************************************************************************************
 * Initializing the ADC modules and sequencer
 ********************************************************************************************************/
ADC_FunctionReturn ADC_Init(void);

/*********************************************************************************************************
 * Variadic function used in Configuring the sequencer input channels.
 *********************************************************************************************************/
ADC_FunctionReturn ADC_SSnChooseInCh(uint8_t ADC_GroupIdx,uint8_t ADC_NoSamples, ... );

/*********************************************************************************************************
 * Variadic function used in Configuring the sequencer Control bits in ADCSSCTLn Registers.
 *********************************************************************************************************/
ADC_FunctionReturn ADC_SSnControl(uint8_t ADC_GroupIdx,uint8_t ADC_NoSamples, ... );

/*********************************************************************************************************
 * Get Sample From FIFO
 *********************************************************************************************************/
ADC_FunctionReturn ADC_SSFIFO(uint8_t ADC_GroupIdx,uint32_t* ADC_SampleRes);


/*********************************************************************************************
  * This Function is used in getting trigger state for the running sequence 
  * return 0 completed successfully
	* return 1 terminated unsuccessful
	* takes the adc group idx, a pointer to a variable
  * return the trigger state in the variable pointed to it by the second parameter
  ********************************************************************************************/
ADC_FunctionReturn ADC_GetTriggerState(uint8_t ADC_GroupIdx,uint8_t *ADC_triggerState);


/**********************************************************************************************
 * Initiate sequence Conversion by setting the SSn bit in ADCPSSI register
 **********************************************************************************************/
ADC_FunctionReturn ADC_ProcessorInitiateSampling(uint8_t ADC_GroupIdx);

/**************************************************************************************************************************
 * Clear ADIF Bit (in Polling)
 **************************************************************************************************************************/
ADC_FunctionReturn ADC_SampleAcknowledge(uint8_t ADC_GroupIdx);


/*************************************************************************************************
 * This Function Enable the interrupt 
 *************************************************************************************************/
ADC_FunctionReturn ADC_EnableInterrupt(uint8_t ADC_GroupIdx, uint8_t PRI);

	
 
#endif