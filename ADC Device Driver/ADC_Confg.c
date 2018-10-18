/**
  * ADC Configuration (.c) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */

#include "ADC_Confg.h"

/**
  * uint8_t ADC_ModuleId;
	* uint8_t ADC_SSId;
	
	* ADC_TriggerEvent ADC_TriggeringEvent;
	
	* ADC_SampleSeqControlBitsCombination sample0;
	* ADC_SampleSeqControlBitsCombination sample1;
	* ADC_SampleSeqControlBitsCombination sample2;
	* ADC_SampleSeqControlBitsCombination sample3;
	* ADC_SampleSeqControlBitsCombination sample4;
	* ADC_SampleSeqControlBitsCombination sample5;
	* ADC_SampleSeqControlBitsCombination sample6;
	* ADC_SampleSeqControlBitsCombination sample7;	
	
	* ADC_Channel Sample0_Channel;
	* ADC_Channel Sample1_Channel;
	* ADC_Channel Sample2_Channel;
	* ADC_Channel Sample3_Channel;
	* ADC_Channel Sample4_Channel;
	* ADC_Channel Sample5_Channel;
	* ADC_Channel Sample6_Channel;
	* ADC_Channel Sample7_Channel;
	
  */


const ADC_Group ADC_GroupConfg[ADCNUMBEROFGROUPS]=
{
{0,3,ADC_Processor,
NotempIntEndNodiff,NotempNointEndNodiff,NotempNointEndNodiff,
NotempNointEndNodiff,NotempNointEndNodiff,NotempNointEndNodiff,
NotempNointEndNodiff,NotempNointEndNodiff, ADC_CH0, 
ADC_CH0, ADC_CH0, ADC_CH0, 
ADC_CH0, ADC_CH0, ADC_CH0, 
ADC_CH0},
{1,3,ADC_Processor,
NotempIntEndNodiff,NotempNointEndNodiff,NotempNointEndNodiff,
NotempNointEndNodiff,NotempNointEndNodiff,NotempNointEndNodiff,
NotempNointEndNodiff,NotempNointEndNodiff, ADC_CH1, 
ADC_CH0, ADC_CH0, ADC_CH0, 
ADC_CH0, ADC_CH0, ADC_CH0, 
ADC_CH0},

};
