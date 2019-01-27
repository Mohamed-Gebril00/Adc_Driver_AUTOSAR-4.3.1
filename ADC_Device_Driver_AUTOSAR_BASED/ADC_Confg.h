/**
  * ADC Configuration (.h) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */



#ifndef ADC_CONFG_H
#define ADC_CONFG_H
#define NUMBER_OF_GROUPS 2

#include "ADC.h"



// Number of sample sequencers in the two modules to be configured

#define CONFIGUREMODULE0  ADC_ModuleOn
#define CONFIGUREMODULE1  ADC_ModuleOn

/**********************************Sampling speed****************************************************/
/**
  * the coming two macros used in specifying the sample speed of the modules
  * less speed less power consume, longer sampling time and more accurate conversion
  */
#define ADC0MAXSAMPLING ADC_500K
#define ADC1MAXSAMPLING ADC_250K
/**************************************************************************************/
 
 
 
/*********************************Sample Sequencer Priority*****************************************************/
/** 
  * User Specify the priority of the four sequencer 
  * The priority should be unique and takes values
  * From 0 to 3 ( 0 Highest, 3 Lowest) or consistent results will not be provided
  */
// Module 0
#define SAMPLESEQ0M0PRI	3
#define SAMPLESEQ1M0PRI	2
#define SAMPLESEQ2M0PRI	1
#define SAMPLESEQ3M0PRI	0

// Module 1
#define SAMPLESEQ0M1PRI	3
#define SAMPLESEQ1M1PRI	2
#define SAMPLESEQ2M1PRI	1
#define SAMPLESEQ3M1PRI	0
/*****************************************************************************************/


/********************************Sample Sequencers Triggering event************************************************/

#endif