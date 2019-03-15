/* Adc_Driver,
	 Author Mohamed Ahmed Gebril 
   Faculty of engineering Alexandria university */
/* Most of the comments are description from the autosar document */
#ifndef ADC_H
#define ADC_H


/* Id for the company in the AUTOSAR
  * Gebro-u-motive's company ID = 0x0076 :) 
  * Gebro-u-motive is a fictional company owned by
	* Mohamed Ahmed Gebril: vendor of the module*/
#define ADC_VENDOR_ID    (0x0076)

/* ADC Module Id */
#define ADC_MODULE_ID    (123U)

/* ADC Instance Id */
#define ADC_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define ADC_SW_MAJOR_VERSION           (1U)
#define ADC_SW_MINOR_VERSION           (0U)
#define ADC_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.3.1
 */
#define ADC_AR_RELEASE_MAJOR_VERSION   (4U)
#define ADC_AR_RELEASE_MINOR_VERSION   (3U)
#define ADC_AR_RELEASE_PATCH_VERSION   (1U)


#include "../Std_Types/Std_Types.h"
/* AUTOSAR checking between Std Types and ADC Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != ADC_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != ADC_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != ADC_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif
  
#include "Adc_TypeDefs.h"


/* AUTOSAR checking between Adc_TypeDef and ADC Modules */
#if ((ADC_AR_RELEASE_MAJOR_VERSION != ADC_TYPE_DEFS_AR_RELEASE_MAJOR_VERSION)\
 ||  (ADC_AR_RELEASE_MINOR_VERSION != ADC_TYPE_DEFS_AR_RELEASE_MINOR_VERSION)\
 ||  (ADC_AR_RELEASE_PATCH_VERSION != ADC_TYPE_DEFS_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Adc_Typedefs.h does not match the expected version"
#endif

/*  pre compile Configuration files */
#include "Adc_Cfg.h"
/* AUTOSAR checking between Adc_Cfg and ADC Modules */
#if ((ADC_AR_RELEASE_MAJOR_VERSION != ADC_CFG_AR_RELEASE_MAJOR_VERSION)\
 ||  (ADC_AR_RELEASE_MINOR_VERSION != ADC_CFG_AR_RELEASE_MINOR_VERSION)\
 ||  (ADC_AR_RELEASE_PATCH_VERSION != ADC_CFG_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Adc_Cfg.h does not match the expected version"
#endif

/* clocks and prioity Not an autosar file for the whole controller TM4C123GH6PM */
#include "M4MemMap.h"

/* API Ids from ADC_SWS document */
#define ADC_INIT_SID													0x00
#define ADC_SETUP_RESULT_BUFFER_SID 					0x0c
#define ADC_DEINIT_SID												0x01
#define ADC_START_GROUP_CONVERSION_SID				0x02
#define ADC_STOP_GROUP_CONVERSION_SID					0x03
#define ADC_READ_GROUP_SID										0x04
#define ADC_ENABLE_HARDWARE_TRIGGER_SID				0x05
#define ADC_DISABLE_HARDWARE_TRIGGER_SID			0x06
#define ADC_ENABLE_GROUP_NOTIFICATION_SID			0x07
#define ADC_DISABLE_GROUP_NOTIFICATION_SID 		0x08
#define ADC_GET_GROUP_STATUS_SID							0x09
#define ADC_GET_STREAM_LAST_POINTER_SID				0x0b
#define ADC_GET_VERSION_INFO									0x0a
#define ADC_SET_POWER_STATE										0x10
#define ADC_GET_CURRENT_POWER_STATE						0x11
#define ADC_GET_TARGET_POWER_STATE						0x12
#define ADC_PREPARE_POWER_STATE								0x13



/* Errors IDs P.48 Adc_SWS */
#define ADC_E_UNINIT						0x0A
#define ADC_E_BUSY							0x0B
#define ADC_E_IDLE							0x0C
#define ADC_E_ALREADY_INITIALIZED 			0x0D
#define ADC_E_PARAM_POINTER					0x0E
#define ADC_E_PARAM_GROUP					0x15
#define ADC_E_WRONG_CONV_MODE				0x16
#define ADC_E_WRONG_TRIGG_SRC				0x17
#define ADC_E_NOTIF_CAPABILITY				0x18
#define ADC_E_BUFFER_UNINIT					0x19
#define ADC_E_NOT_DISENGAGED				0x1A
#define ADC_E_POWER_STATE_NOT_SUPPORTED		0x1B
#define ADC_E_TRANSITION_NOT_POSSIBLE		0x1C
#define ADC_E_PERIPHERAL_NOT_PREPARED		0x1D
 
extern const Adc_ConfigType Adc_Config;

/************************************************************************************************************
 * This function is used in Initializing  ADC Groups and modules
 ***********************************************************************************************************/
void Adc_Init(const Adc_ConfigType* ConfigPtr);

/* Initializes ADC driver with the group specific result buffer start address where the conversion results will be stored.
	The application has to ensure that the application buffer, where DataBufferPtr points to, 
	can hold all the conversion results of the specified group.
	The initialization with Adc_SetupResultBuffer is required after reset, before a group conversion can be started.*/
Std_ReturnType Adc_SetupResultBuffer(
							 Adc_GroupType Group, 
							 Adc_ValueGroupType* DataBufferPtr
							 );

/* Deinit Function 
	Returns all ADC HW Units to a state comparable to their power on reset state. 
	This Api is not supported for now.*/
void Adc_DeInit(void);

/* Starts the conversion of all channels of the requested ADC Channel group. */
#if(ADC_ENABLE_START_STOP_GROUP_API==STD_ON)
void Adc_StartGroupConversion(Adc_GroupType Group);

/* Stops the conversion of the requested ADC Channel group.
	This Api is not supported for now. */
void Adc_StopGroupConversion(void);
#endif

/*Reads the group conversion result of the last completed conversion round of the requested group 
and stores the channel values starting at the DataBufferPtr address. 
The group channel values are stored in ascending channel number order*/
#if(ADC_READ_GROUP_API==STD_ON)
Std_ReturnType Adc_ReadGroup(
								Adc_GroupType Group,
								Adc_ValueGroupType* DataBufferPtr
							);

/* Returns the conversion status of the requested ADC Channel group. */
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);
#endif


/* Enables the notification mechanism for the requested ADC Channel group. */
#if(ADC_GRP_NOTIF_CAPABILITY==STD_ON)
void Adc_EnableGroupNotification(Adc_GroupType Group);
void Adc_DisableGroupNotification(Adc_GroupType Group);
#endif


#endif