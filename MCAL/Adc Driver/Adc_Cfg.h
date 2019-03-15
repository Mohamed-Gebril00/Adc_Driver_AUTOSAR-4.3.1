/*
 * Pre-compile configuration for adc driver
 * author: Mohamed Ahmed Gebril
 */
#ifndef ADC_CFG_H
#define ADC_CFG_H
#include "Adc_TypeDefs.h"

/*
 * Module Version 1.0.0
 */
#define ADC_CFG_SW_MAJOR_VERSION              (1U)
#define ADC_CFG_SW_MINOR_VERSION              (0U)
#define ADC_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define ADC_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define ADC_CFG_AR_RELEASE_MINOR_VERSION     (3U)
#define ADC_CFG_AR_RELEASE_PATCH_VERSION     (1U)




/* Adc_Channels id for TM4C123GH6PM */
#define ADC_CHANNEL_0                           0u
#define ADC_CHANNEL_1							1u
#define ADC_CHANNEL_2							2u
#define ADC_CHANNEL_3 							3u
#define ADC_CHANNEL_4							4u
#define ADC_CHANNEL_5							5u
#define ADC_CHANNEL_6 							6u
#define ADC_CHANNEL_7							7u
#define ADC_CHANNEL_8							8u	
#define ADC_CHANNEL_9							9u
#define ADC_CHANNEL_10							10u
#define ADC_CHANNEL_11							11u

/* Adc Modules for TM4C123GH6PM */
#define ADC_MODULE_0							(0u)
#define ADC_MODULE_1							(1u)

/* Adc Sequencers for TM4C123GH6PM */
#define ADC_SEQUENCER_0							(0u)
#define ADC_SEQUENCER_1							(1u)
#define ADC_SEQUENCER_2							(2u)
#define ADC_SEQUENCER_3							(3u)



/* used to enable or disable deinit api */
#define ADC_DEINIT_API 							STD_OFF		

/* used to enable or disable Development error detection for Adc driver */
#define ADC_DEV_ERROR_DETECT					STD_OFF

/* Enable or diable the limit checking feature in the adc driver */ 
#define ADC_ENABLE_LIMIT_CHECK					STD_OFF		

/* Determines, if the queuing mechanism is active in case of priority mechanism disabled,
Note: if priority mechanism is  is enabled, queuing mechanism is always active and the parameter ADC_ENABLE_QUEUING
is not evaluated.*/
#define ADC_ENABLE_QUEUING						STD_OFF		

/* Adds / removes the services Adc_StartGroupConversion() and Adc_StopGroupConversion() from the code. */
#define ADC_ENABLE_START_STOP_GROUP_API			STD_ON

/* Determines, if the group notification mechanism (the functions to enable and disable the notifications) is available at runtime.*/
#define ADC_GRP_NOTIF_CAPABILITY 				STD_ON

/* Adds / removes the services Adc_EnableHardwareTrigger() and Adc_DisableHardwareTrigger() from the code. */ 
#define ADC_HW_TRIGGER_API						STD_OFF

/* Adds / removes all power state management related APIs (ADC_SetPowerState, ADC_GetCurrentPowerState,
 ADC_GetTargetPowerState, ADC_PreparePowerState, ADC_Main_PowerTransitionManager),
 indicating if the HW offers low power state management.*/
#define ADC_LOW_POWER_STATES_SUPPORT			STD_OFF

/* Enables / disables support of the ADCDriver to the asynchronous power state transition. */
#define ADC_POWER_STAT_ASYNCH_TRANSITION_MODE	STD_OFF

/*Determines whether a priority mechanism is available for prioritization of the conversion requests and if available, 
the type of prioritization mechanism. The selection applies for groups with 
trigger source software and trigger source hardware. 
Two types of prioritization mechanism can be selected.
The hardware prioritization mechanism (AdcPriorityHw) uses the ADC hardware features for prioritization of the software conversion requests 
and hardware trigger signals for groups with trigger source hardware. 
The mixed hardware and software prioritization mechanism (AdcPriorityHwSw) 
uses the ADC hardware features for prioritization of ADC hardware trigger for groups with trigger source hardware and 
a software implemented prioritization mechanism for groups with trigger source software. 
The group priorities for software triggered groups are typically configured 
with lower priority levels than the group priorities for hardware triggered groups.*/
#define ADC_PRIORITY_IMPLEMENTATION				ADC_PRIORITY_HW

/* Adds / removes the service Adc_ReadGroup() and from the code. */
#define ADC_READ_GROUP_API						STD_ON

/* Alignment of ADC raw results in ADC result buffer (left/right alignment).*/
/* mplementation Type: Adc_ResultAlignmentType */
#define ADC_RESULT_ALIGNMENT					ADC_ALIGN_RIGHT

/* Adds / removes the service Adc_GetVersionInfo() from the code. 
true: Adc_GetVersionInfo() can be used. false: Adc_GetVersionInfor() can not be used. */
#define ADC_VERSION_INFO_API					STD_OFF

/* Hw Specs two adc_modules each with 4 sequencers */


/* Number of Adc_channel Group Maximum is 8 */
#define NUMBER_OF_CHANNEL_GROUPS (1u)


/*********************************Sample Sequencer Priority in the adc module*****************************************************/
/** 
  * User Specify the priority of the four sequencer 
  * The priority should be unique and takes values
  * From 0 to 3 ( 0 Highest, 3 Lowest) or consistent results will not be provided
  */	
// Module 0
#define ADC_0_SS_0_MODULE_PRI	3
#define ADC_0_SS_1_MODULE_PRI	2
#define ADC_0_SS_2_MODULE_PRI	1
#define ADC_0_SS_3_MODULE_PRI	0

// Module 1
#define ADC_1_SS_0_MODULE_PRI	0
#define ADC_1_SS_1_MODULE_PRI	1
#define ADC_1_SS_2_MODULE_PRI	2
#define ADC_1_SS_3_MODULE_PRI	3


/****************************** ISRs Priority for the NVTable from 0 to 7 **************************************************/
#define ADC_0_SS0_HANDLER_PRI			0
#define ADC_0_SS1_HANDLER_PRI			0
#define ADC_0_SS2_HANDLER_PRI			0
#define ADC_0_SS3_HANDLER_PRI			0
#define ADC_1_SS0_HANDLER_PRI			0
#define ADC_1_SS1_HANDLER_PRI			0
#define ADC_1_SS2_HANDLER_PRI			0
#define ADC_1_SS3_HANDLER_PRI			0
#endif