#ifndef ADC_TYPEDEFS_H
#define ADC_TYPEDEFS_H

/*
 * Module Version 1.0.0
 */
#define ADC_TYPE_DEFS_SW_MAJOR_VERSION           (1U)
#define ADC_TYPE_DEFS_SW_MINOR_VERSION           (0U)
#define ADC_TYPE_DEFS_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.3.1
 */
#define ADC_TYPE_DEFS_AR_RELEASE_MAJOR_VERSION   (4U)
#define ADC_TYPE_DEFS_AR_RELEASE_MINOR_VERSION   (3U)
#define ADC_TYPE_DEFS_AR_RELEASE_PATCH_VERSION   (1U)


#include "../Std_Types/Std_Types.h"
/* AUTOSAR checking between Std Types and ADC typedefs Module */

#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != ADC_TYPE_DEFS_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != ADC_TYPE_DEFS_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != ADC_TYPE_DEFS_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

#define ADC_MAXIMUM_CHANNELS_PER_GROUP (8u)

/* Numeric ID of an ADC channel. */ 
typedef uint8 Adc_ChannelType;

/* Numeric ID of an ADC channel group. */
typedef uint8 Adc_GroupType;

/* Numeric Id of Adc_HwUnit (one module and one sequencer) */
typedef uint8 Adc_HwUnitType;

/* (Numeric Id of sequencer) */
typedef uint8 Adc_SequencerType;

/* (Numeric Id of Adc_Module) */
typedef uint8 Adc_ModuleType;

/* Type for reading the converted values of a channel group 
(raw, without further scaling, alignment according precompile switch ADC_RESULT_ALIGNMENT).*/
typedef sint16 Adc_ValueGroupType;

/* Type of clock prescaler factor. (This is not an API type). */
typedef uint8 Adc_PrescaleType;

/* Not used in TM4C123GH6PM */
/* Type of conversion time, i.e.
the time during which the sampled analogue value is converted into digital representation */
/* typedef uint Adc_ConversionTimeType; */ 

/* Not used in TM4C123GH6PM */
/* Type of sampling time, i.e. the time during which the value is sampled, (in clock-cycles). */ 
/* typedef uint Adc_SamplingTimeType*/

/* Not used in TM4C123GH6PM */
/* Type of channel resolution in number of bits. */
typedef uint8 Adc_ResolutionType;

/* Current status of the conversion of the requested ADC Channel group. */
/* ADC_IDLE: The conversion of the specified group has not been started. - No result is available.*/
/* ِADC_BUSY: The conversion of the specified group has been started and is still going on. - So far no result is available.*/
/* ADC_COMPLETED: A conversion round (which is not the final one) of the specified group has been finished. -
 A result is available for all channels of the group.*/
/* ADC_STREAM_COMPLETED: The result buffer is completely filled - For each channel of the selected group the
 number of samples to be acquired is available*/
typedef enum {ADC_IDLE=0, ADC_BUSY, ADC_COMPLETED, ADC_STREAM_COMPLETED} Adc_StatusType;

/* Type for configuring the trigger source for an ADC Channel group. */
/* ADC_TRIGG_SRC_SW: Group is triggered by a software API call. */
/* ADC_TRIGG_SRC_HW: Group is triggered by a hardware event.*/
typedef enum {ADC_TRIGG_SRC_SW=0, ADC_TRIGG_SRC_HW} Adc_TriggerSourceType;

typedef enum{ANALOG_COMPARATOR_0=1, ANALOG_COMPARATOR_1, EXTERNAL=4, TIMER, PWM_GENERATOR_0, PWM_GENERATOR_1, 
PWM_GENERATOR_2, PWM_GENERATOR_3}Adc_HwTriggerSourceType;

/* ADC_CONV_MODE_ONESHOT: Exactly one conversion of each channel in an ADC channel group is performed after the configured trigger event.
 In case of 'group trigger source software', a started One-Shot conversion can be stopped by a software API call.
 In case of 'group trigger source hardware',
 a started One-Shot conversion can be stopped by disabling the trigger event (if supported by hardware).*/
 /* ADC_CONV_MODE_CONTINUOUS: Repeated conversions of each ADC channel in an ADC channel group are performed. 
 'Continuous conversion mode' is only available for 'group trigger source software'.
 A started 'Continuous conversion' can be stopped by a software API call.*/
typedef enum {ADC_CONV_MODE_ONESHOT=0, ADC_CONV_MODE_CONTINUOUS=0xF}Adc_GroupConvModeType;

/* Priority level of the channel. Lowest priority is 0.*/
typedef uint8 Adc_GroupPriorityType;

/* Type for configuring the number of group conversions in streaming access mode */
typedef uint8 Adc_StreamNumSampleType;

/* Typee for Configuring the number of input channels per hw unit */
typedef uint8 Adc_InputNumChType;


/* Type for configuring the access mode to group conversion results. */
typedef enum {ADC_STREAM_BUFFER_LINEAR, ADC_STREAM_BUFFER_CIRCULAR}Adc_StreamBufferModeType;

typedef enum {ADC_ACCESS_MODE_SINGLE=0, ADC_ACCESS_MODE_STREAMING}Adc_GroupAccessModeType;

/* Type for configuring on which edge of the hardware trigger signal the driver should */
typedef enum {ADC_HW_TRIG_RISING_EDGE, ADC_HW_TRIG_FALLING_EDGE, ADC_HW_TRIG_BOTH_EDGES}Adc_HwTriggerSignalType;

/* Type for configuring the prioritization mechanism. */
typedef enum {ADC_PRIORITY_NONE=0, ADC_PRIORITY_HW, ADC_PRIORITY_HW_SW}Adc_PriorityImplementationType; 

/* Replacement mechanism, which is used on ADC group level, if a group conversion is interrupted by a group which has a higher priority. */
/* ADC_GROUP_REPL_ABORT_RESTART: Abort/Restart mechanism is used on group level, if a group is interrupted by a higher priority group. 
The complete conversion round of the interrupted group (all group channels)is restarted after the higher priority group conversion is finished.
 If the group is configured in streaming access mode, only the results of the interrupted conversion round are discarded.
 Results of previous conversion rounds which are already written to the result buffer are not affected.*/
 /* Suspend/Resume mechanism is used on group level, if a group is interrupted by a higher priority group.
 The conversion round of the interrupted group is completed after the higher priority group conversion is finished. 
 Results of previous conversion rounds which are already written to the result buffer are not affected.
 ADC_GROUP_REPL_SUSPEND_RESUME:*/
typedef enum {ADC_GROUP_REPL_ABORT_RESTART=0, ADC_GROUP_REPL_SUSPEND_RESUME}Adc_GroupReplacementType;

/* In case of active limit checking: defines which conversion values are taken into
   account related to the boardes defineed with AdcChannelLowLimit and
   AdcChannelHighLimit.*/
/* ADC_RANGE_UNDER_LOW: Range below low limit - low limit value included */
/* ADC_RANGE_BETWEEN: Range between low limit and high limit - high limit value included */
/* ADC_RANGE_OVER_HIGH: Range above high limit*/
/* ADC_RANGE_ALWAYS: Complete range - independent from channel limit settings */
/* ADC_RANGE_NOT_UNDER_LOW: Range above low limit */
/* ADC_RANGE_NOT_BETWEEN: Range above high limit or below low limit - low limit value included*/
/* ADC_RANGE_NOT_OVER_HIGH: Range below high limit - high limit value included*/
typedef enum {ADC_RANGE_UNDER_LOW=0, ADC_RANGE_BETWEEN, ADC_RANGE_OVER_HIGH, ADC_RANGE_ALWAYS, ADC_RANGE_NOT_UNDER_LOW, ADC_RANGE_NOT_BETWEEN,
ADC_RANGE_NOT_OVER_HIGH}Adc_ChannelRangeSelectType;

/* Type for alignment of ADC raw results in ADC result buffer (left/right alignment). */
typedef enum {ADC_ALIGN_LEFT=0, ADC_ALIGN_RIGHT}Adc_ResultAlignmentType;

/* Result of the requests related to power state transitions. */
/* ADC_SERVICE_ACCEPTED: Power state change executed. */
/* ADC_NOT_INIT: ADC Module not initialized. */
/* ADC_SEQUENCE_ERROR: Wrong API call sequence. */
/* ADC_HW_FAILURE: The HW module has a failure which prevents it to enter the required power state. */
/* ADC_POWER_STATE_NOT_SUPP: ADC Module does not support the requested power state. */
/* ADC_TRANS_NOT_POSSIBLE: ADC Module cannot transition directly from the current power state to the requested power state or the HW peripheral is still busy. */
typedef enum{ADC_SERVICE_ACCEPTED=0, ADC_NOT_INIT, ADC_SEQUENCE_ERROR, ADC_HW_FAILURE, ADC_POWER_STATE_NOT_SUPP, ADC_TRANS_NOT_POSSIBLE}Adc_PowerStateRequestResultType;



/* pointer to function to use it for call backs */
typedef  void(*Adc_NotificationType)(void);

/* Type for assignment of channels to a channel group */
typedef uint8 Adc_GroupDefType;

/* Sampling speed of the modules */ 
typedef enum {ADC_125_K_SAMPLING_SPEED=0, ADC_500_K_SAMPLING_SPEED, ADC_1_M_SAMPLING_SPEED} Adc_ModuleSamplingSpeedType;

/* type to access adc peripheral register */
typedef volatile uint32* const Adc_RegAddType;

typedef enum{ADC_DRIVER_NOT_INIT=0, ADC_DRIVER_INITIALIZED}Adc_DriverStateType;


/* Type for assignment of channels to a channel group (this is not an API type). */
typedef struct {
	Adc_GroupType					      AdcGroupId;
	Adc_GroupAccessModeType			AdcGroupAccessMode;
	Adc_GroupConvModeType		   	AdcGroupConversionMode;
	

		Adc_GroupPriorityType			AdcGroupPriority;
	
	
	Adc_TriggerSourceType			AdcGroupTriggSrc;
	
		Adc_HwTriggerSourceType			AdcHwTriggerSource;
		Adc_HwTriggerSignalType			AdcHwTriggerSignal;
	
	
  Adc_StreamBufferModeType		AdcStreamingBufferMode;
	Adc_StreamNumSampleType			AdcStreamingNumSamples;
	
	
		Adc_NotificationType			AdcNotification;

	
	Adc_GroupReplacementType 		AdcGroupReplacement;
	Adc_GroupDefType				AdcGroupDefinition[ADC_MAXIMUM_CHANNELS_PER_GROUP];
}Adc_ChannelGroupConfigType;

/* Data structure contains HW unit PbCfg */
typedef struct {
	/* Numeric ID of the HW Unit. This symbolic name allows accessing Hw Unit data. Enumeration literals are defined vendor specific. */
	Adc_HwUnitType AdcHwUnitId;
	
	/* Numeric Id of the Adc_Module */
	Adc_ModuleType AdcModuleId;
	
	/* Numberic Id of the Adc_Sequencer */
	Adc_SequencerType AdcSequencerId;
	
	/* True: use Adc_Sequencer false: don't configure */
	boolean Adc_Sequencer;
	
	/* Hold the number of input channels per hw unit*/
	Adc_InputNumChType Adc_NumberOfInputChannels;
	
	/* Pointer to the Adc_ChannelGroup*/
	Adc_GroupType Adc_Group;
	
}Adc_HwUnitConfigType;

/* Data structure containing the set of configuration parameters required for initializing the ADC Driver and ADC HW Unit(s). */
typedef struct 
{
	/* True: Configure and enable clock for module 0, False: Don't configure */
	boolean Cfg_Module0;
	
	/* Type for coniguring sampling speed for Module 0 
	ADC_125_K_SAMPLING_SPEED for 125 kilo sample per second.
	ADC_250_K_SAMPLING_SPEED for 250 kilo sample per second.
	ADC_500_K_SAMPLING_SPEED for 500 kilo sample per second.
	ADC_1_M_SAMPLING_SPEED for 1 milion sample per second.
	*/
	Adc_ModuleSamplingSpeedType Adc_Module0SamplingSpeed;	
	
	/* True: Configure and enable clock for module 1, False: Don't configure */
	boolean Cfg_Module1;

	/* Type for coniguring sampling speed for Module 1
	ADC_125_K_SAMPLING_SPEED for 125 kilo sample per second.
	ADC_250_K_SAMPLING_SPEED for 250 kilo sample per second.
	ADC_500_K_SAMPLING_SPEED for 500 kilo sample per second.
	ADC_1_M_SAMPLING_SPEED for 1 milion sample per second.
	*/
	Adc_ModuleSamplingSpeedType Adc_Module1SamplingSpeed;
	

	/* pointer to Adc_HwUnitType to Configure Adc Module Required sequncers and Adc Channel Group. */ 
	Adc_HwUnitConfigType* Adc_HwUnitPtr;
}Adc_ConfigType;
#endif