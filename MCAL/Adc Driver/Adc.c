#include "Adc_Reg.h"
#include "Adc.h"
#include "../GPIO.h"


/* defines used in reporting development errors to the Development Error module */
#define ADC_MODULE_ID 					(uint16)123		/* List of Basic Software Modules */
#define ADC_INSTANCE_ID 				(uint8)0		/* on;y one adc driver */

/* Number of hw units not configurable hw limitations */
#define NUMBER_OF_HW_UNITS       (8u)
#define HW_UNIT_0								 (0u)
#define HW_UNIT_1								 (1u)
#define HW_UNIT_2								 (2u)
#define HW_UNIT_3								 (3u)
#define HW_UNIT_4								 (4u)
#define HW_UNIT_5								 (5u)
#define HW_UNIT_6								 (6u)
#define HW_UNIT_7								 (7u)
/* base address for modules to facilitate accessing the registers by adding the required register offset */
static const uint32 Adc_ModuleBaseAddressLut[]=
{
	ADC0BASEADDRESS,
	ADC1BASEADDRESS
};


/* constants depends on hardware limitations for TM4C123GH6PM */
/* Maximum Number of Adc_Channel_Group TM4C123GH6PM */
#define ADC_MAXIMUM_NUMBER_OF_CHANNEL_GROUPS			(8u)

/* Maximum Number of Adc_Channels (Hardware limitations TM4C123GH6PM */
#define ADC_MAXIMUM_CHANNELS_PER_GROUP					(8u)

/* Hardware limitation for TM4C123GH6PM, Number of input channels for sampling and hold seq 0 */
#define MAXIMUM_NUMBER_OF_CHANNELS_SEQUENCER0			(8u)
					
/* Hardware limitation for TM4C123GH6PM, Number of input channels for sampling and hold seq 1 */
#define MAXIMUM_NUMBER_OF_CHANNELS_SEQUENCER1   		(4u)

/* Hardware limitation for TM4C123GH6PM, Number of input channels for sampling and hold seq 2 */
#define MAXIMUM_NUMBER_OF_CHANNELS_SEQUENCER2			(4u)

/* Hardware limitation for TM4C123GH6PM, Number of input channels for sampling and hold seq 3 */
#define MAXIMUM_NUMBER_OF_CHANNELS_SEQUENCER3			(1u)

/* Post build configuration extern*/
extern Adc_ChannelGroupConfigType Adc_ChannelGroupConfig[NUMBER_OF_CHANNEL_GROUPS];
extern Adc_HwUnitConfigType Adc_HwUnitConfig[NUMBER_OF_HW_UNITS];

/* static arrays to hold information belongs to one adc channnel group */
/* Id of the hardware unit */
static uint8 Adc_ChannelGroupHwUnitId[8]={0};

/* Input channels number corresponding to a channel group */
static uint8 Adc_ChannelGroupNumInputChannel[8]={0};

/* Number of samples That finished conversions */
static uint8 Adc_ChannelGroupFinishedSamples[8]={0};

/* Array to store whether the notification has been enabled to th corresponding 
	Group or not */
static boolean Adc_ChannelGroupNotificEnabled[8];

/* array to check whether the result buffer has been initialized for the corresponding group or not */
static boolean Adc_resultBufferInit[8];

/* Array of pointers to hold the address of the result buffer given by the user for each group. */
static Adc_ValueGroupType* DataBufferPtrAddr[8];

/* Driver state: ADC_Driver_INITIALIZED Adc_init has been called suuccessfully
				 ADC_Driver_NOT_INIT  han't been called successfully*/
static Adc_DriverStateType Adc_DriverState = ADC_DRIVER_NOT_INIT; 


/* Current status of the conversion of the requested ADC Channel group. */
static Adc_StatusType Adc_GroupStatus[ADC_MAXIMUM_NUMBER_OF_CHANNEL_GROUPS]=
{
	/* ADC_IDLE */ 
	/* The conversion of the specified group has not been started. - 
	No result is available.*/
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE,
	ADC_IDLE
};


/* offset for sample_sequencer_mux */
static const uint32 Adc_SsMuxOffset[]=
{
	ADCSSMUX0, 
	ADCSSMUX1, 
	ADCSSMUX2, 
	ADCSSMUX3};
	
/* offset for sample_sequencer_control */
static const uint32 Adc_SsCtlOffset[]=
{
	ADCSSCTL0, 
	ADCSSCTL1, 
	ADCSSCTL2, 
	ADCSSCTL3};

/* offset for sample_sequencer_fifo */
static const uint32 Adc_SsFIFO[]=
{
	ADCSSFIFO0, 
	ADCSSFIFO1, 
	ADCSSFIFO2, 
	ADCSSFIFO3};


/* define these arrays only if group notification configuration is enabled */
#if(ADC_GRP_NOTIF_CAPABILITY == STD_ON)
/* 2-D array to hold Interrupt number for each sequencer in each module 
	module ID is the row number where clumn number is the sequencer ID */
static const uint32 Adc_SSIRQ[][4]=
{
	{ADC_0_SS_0_PRI_NUMBER, ADC_0_SS_1_PRI_NUMBER, ADC_0_SS_2_PRI_NUMBER, ADC_0_SS_3_PRI_NUMBER},
	{ADC_1_SS_0_PRI_NUMBER, ADC_1_SS_1_PRI_NUMBER, ADC_1_SS_2_PRI_NUMBER, ADC_1_SS_3_PRI_NUMBER}
};

/* 2D array contains the handlers priority */
/* for every sequencer */
/* preconfigured in Adc_Cfg.h file */
static const uint32 Adc_SSHandlerPri[][4]=
{
	{ADC_0_SS0_HANDLER_PRI, ADC_0_SS1_HANDLER_PRI, ADC_0_SS2_HANDLER_PRI, ADC_0_SS3_HANDLER_PRI},
	{ADC_0_SS0_HANDLER_PRI, ADC_1_SS1_HANDLER_PRI, ADC_1_SS2_HANDLER_PRI, ADC_1_SS3_HANDLER_PRI}
};

/* The driver used this array to pass the idx
   to the ISR handler */
static uint8 GroupToHandler[2][4]={{0}};

/* Sample sequencer ISRs */
void ADC0SS0_Handler(void);
void ADC0SS1_Handler(void);
void ADC0SS2_Handler(void);
//void ADC0Seq3_Handler(void);
void ADC1SS0_Handler(void);
void ADC1SS1_Handler(void);
void ADC1SS2_Handler(void);
void ADC1SS3_Handler(void);
	
#endif


/* Static Functions used within this file to configure hw units used only one time */
static void Adc_ConfigureSequencern(Adc_GroupType Group);

/************************************************************************************************************
 * This function is used in Initializing  ADC Groups and modules
 ***********************************************************************************************************/
void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
	uint8_least Adc_GebroLoopIdx;
	uint32 delay;    /* used in waiting 3 cycles till clock is on */
	Adc_HwUnitConfigType* HwUnitPtr;
	Adc_ChannelGroupConfigType* ChannelGroupPtr;
	
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
		/* Check the Adc driver state Configure */
		if(Adc_DriverState == ADC_DRIVER_INITIALIZED)
		{
			/* report the error to the det module */
			Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_INIT_SID, ADC_E_ALREADY_INITIALIZED);
		}
	#endif

	if ((ConfigPtr->Cfg_Module0) == TRUE)
	{
		/* enable module zero clock */
		RCGC0_REG |= (ADC_MODULE0_RCGC0_FIELD);
	    delay = RCGC0_REG;
		
		/* configure sampling speed */
		RCGC0_REG |= ((ConfigPtr->Adc_Module0SamplingSpeed) << MAXADC0SPD);
		
		/* Configure sequencer priorities values in register */
		ADCSSPRI_REG(ADC_MODULE_0)= CONC(ADC_0_SS_0_MODULE_PRI, ADC_0_SS_1_MODULE_PRI, ADC_0_SS_2_MODULE_PRI, ADC_0_SS_3_MODULE_PRI);

	}
	else 
	{
		// Do Nothing
	}
	/* module 1*/
	if ((ConfigPtr->Cfg_Module1) == TRUE)
	{
		/* enable module zero clock */
		RCGC0_REG |= (ADC_MODULE1_RCGC0_FIELD);
	    delay = RCGC0_REG;
		
		/* configure sampling speed */
		RCGC0_REG |= ((ConfigPtr->Adc_Module1SamplingSpeed) << MAXADC1SPD);
		
		/* Configure sequencer priorities values in register */
		ADCSSPRI_REG(ADC_MODULE_1) = CONC(ADC_1_SS_0_MODULE_PRI, ADC_1_SS_1_MODULE_PRI, ADC_1_SS_2_MODULE_PRI, ADC_1_SS_3_MODULE_PRI);
		
	}
	else 
	{
		// Do Nothing
	}
	for( Adc_GebroLoopIdx = 0 ; Adc_GebroLoopIdx < NUMBER_OF_HW_UNITS ; Adc_GebroLoopIdx++)
	{
		HwUnitPtr = &Adc_HwUnitConfig[Adc_GebroLoopIdx];
		// if sequencer of this hw unit is configured to use
		if((HwUnitPtr->Adc_Sequencer) == TRUE)
		{
			
			
			//ChannelGroupPtr=&Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
			/* initialize the static global variables corresponding to the Adc_channel_group */
			Adc_ChannelGroupNumInputChannel[(HwUnitPtr->Adc_Group)] = (HwUnitPtr->Adc_NumberOfInputChannels);
			Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group]=0;
			Adc_ChannelGroupNotificEnabled[HwUnitPtr->Adc_Group]=FALSE;
			Adc_resultBufferInit[HwUnitPtr->Adc_Group]=FALSE;
			Adc_ChannelGroupHwUnitId[(HwUnitPtr->Adc_Group)] = Adc_GebroLoopIdx;
			/* configure and enable the sequencer */
			Adc_ConfigureSequencern(HwUnitPtr->Adc_Group);
		}
		}
		Adc_DriverState = ADC_DRIVER_INITIALIZED;
}

/* Initializes ADC driver with the group specific result buffer start address where the conversion results will be stored.
	The application has to ensure that the application buffer, where DataBufferPtr points to, 
	can hold all the conversion results of the specified group.
	The initialization with Adc_SetupResultBuffer is required after reset, before a group conversion can be started.*/
Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
	void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
	Std_ReturnType Std_Return=E_OK;
	/* Developement error reporting */
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_PARAM_GROUP);
						}
						if(Adc_GroupStatus[Group] != ADC_IDLE)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_BUSY);
						}
						if(Adc_DriverState == ADC_DRIVER_NOT_INIT)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_UNINIT);
						}
	#endif
	/* take the address of the result buffer and save it in global variable */
	DataBufferPtrAddr[Group] = DataBufferPtr;
						
	/* Result buffer has been setup successfully for the required group */
	Adc_resultBufferInit[Group]=TRUE;

	return Std_Return;
}

/* Deinit Function 
	Returns all ADC HW Units to a state comparable to their power on reset state. */
#if(ADC_DEINIT_API==STD_ON)
void Adc_DeInit(void)
{
	/* This Feature is not supported for now */
}
#endif

/* Starts the conversion of all channels of the requested ADC Channel group. */
#if(ADC_ENABLE_START_STOP_GROUP_API==STD_ON)
void Adc_StartGroupConversion(Adc_GroupType Group)
{
	Adc_ChannelGroupConfigType* ChannelGroupPtr;
	Adc_HwUnitType		Adc_HwUnitId;
	Adc_HwUnitConfigType* HwUnitPtr;
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_START_GROUP_CONVERSION_SID, ADC_E_PARAM_GROUP);
						}
						else 
						{/* Do nothing */}
	#endif
	ChannelGroupPtr = &Adc_ChannelGroupConfig[Group];
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if((ChannelGroupPtr->AdcGroupTriggSrc)==ADC_TRIGG_SRC_HW)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_START_GROUP_CONVERSION_SID, ADC_E_WRONG_TRIGG_SRC);
						}
						else 
						{/* Do nothing */}
	#endif
		Adc_HwUnitId = Adc_ChannelGroupHwUnitId[Group];
		HwUnitPtr = &Adc_HwUnitConfig[Adc_HwUnitId];
		/* change the sate of the group to busy */
		Adc_GroupStatus[Group]=ADC_BUSY;
		ADCPSSI_REG(HwUnitPtr->AdcModuleId) |= (1 << HwUnitPtr->AdcSequencerId);
}

/* Stops the conversion of the requested ADC Channel group. */
void Adc_StopGroupConversion(void)
{
	/* This Api is not supported for now */
}
#endif

/*Reads the group conversion result of the last completed conversion round of the requested group 
and stores the channel values starting at the DataBufferPtr address. 
The group channel values are stored in ascending channel number order*/
#if(ADC_READ_GROUP_API==STD_ON)
Std_ReturnType Adc_ReadGroup(
								Adc_GroupType Group, 
								Adc_ValueGroupType* DataBufferPtr
					)
{
	Std_ReturnType Std_Return = E_OK;
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_READ_GROUP_SID, ADC_E_PARAM_GROUP);
							Std_Return = E_NOT_OK;
						}
						if(Adc_DriverState == ADC_DRIVER_NOT_INIT)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_READ_GROUP_SID, ADC_E_UNINIT);
							Std_Return = E_NOT_OK;
						}
						else
	#endif
	{
	uint8_least Adc_GebroLoopIdx=0;
	Adc_HwUnitType Adc_HwUnitId=Adc_ChannelGroupHwUnitId[Group];
	Adc_HwUnitConfigType* HwUnitPtr=&Adc_HwUnitConfig[Adc_HwUnitId];
	Adc_ChannelGroupConfigType* ChannelGroupPtr=&Adc_ChannelGroupConfig[Group];
	Adc_InputNumChType NumberOfChannels=HwUnitPtr->Adc_NumberOfInputChannels;
	Adc_StreamNumSampleType NumberOfSamplesReq=ChannelGroupPtr->AdcStreamingNumSamples;

	for(Adc_GebroLoopIdx = 0; Adc_GebroLoopIdx<((NumberOfChannels)*(NumberOfSamplesReq)); Adc_GebroLoopIdx++)
	{
				(*(DataBufferPtr+Adc_GebroLoopIdx)) = (*((DataBufferPtrAddr[Group])+Adc_GebroLoopIdx));
	}
	if((ChannelGroupPtr->AdcGroupConversionMode) == ADC_CONV_MODE_ONESHOT)
	{
		Adc_GroupStatus[Group]=ADC_IDLE;
	}
	else 
	{
		Adc_GroupStatus[Group]=ADC_BUSY;
	}
}
	return Std_Return;
}
#endif


 /* Returns the conversion status of the requested ADC Channel group. */
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group)
{
	
	Adc_StatusType Adc_Status = ADC_IDLE;
#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_GET_GROUP_STATUS_SID, ADC_E_PARAM_GROUP);
							Adc_Status = ADC_IDLE;
						}
						if(Adc_DriverState == ADC_DRIVER_NOT_INIT)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_GET_GROUP_STATUS_SID, ADC_E_UNINIT);
								Adc_Status = ADC_IDLE;
						}
						else 
#endif
{
	
	uint8_least Adc_GebroLoopIdx=0;
	Adc_HwUnitType		Adc_HwUnitId = Adc_ChannelGroupHwUnitId[Group];
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[Adc_HwUnitId];
	Adc_ChannelGroupConfigType* ChannelGroupPtr=&Adc_ChannelGroupConfig[Group];
	Adc_InputNumChType Ch_Num=HwUnitPtr->Adc_NumberOfInputChannels;
	Adc_StreamNumSampleType NumberOfSamplesReq=ChannelGroupPtr->AdcStreamingNumSamples;
	Adc_ModuleType Adc_ModuleId = HwUnitPtr->AdcModuleId;
	Adc_SequencerType Adc_SequencerId= HwUnitPtr->AdcSequencerId;
	Adc_StreamNumSampleType Samples_Finished = Adc_ChannelGroupFinishedSamples[Group];
				Adc_Status = 	Adc_GroupStatus[Group];
			  if(((ADCRIS_REG(Adc_ModuleId)) & (1<<(Adc_SequencerId))) != 0)
				{
				// Acknowledge that conversion by clearing the interrupt flag bit.in ADCISC Register.
			  ADCISC_REG(Adc_ModuleId) |= (1<<Adc_SequencerId);
			
				// Move the result in the group result buffer.
				for(Adc_GebroLoopIdx = (Samples_Finished*Ch_Num);
				Adc_GebroLoopIdx < (Ch_Num*(Samples_Finished+1)) ; Adc_GebroLoopIdx++)
				{
					*((DataBufferPtrAddr[Group])+Adc_GebroLoopIdx) =  ADC_SSFIFO((Adc_ModuleId), (Adc_SequencerId));
				}				
				// Increment the number of samples finished by one
				(Samples_Finished)++;
				// check if the stream has finished.
				if(Samples_Finished ==NumberOfSamplesReq)
				{
					    // put the status to stream completed.
							// start the number of samples finised from zero again.
								Adc_GroupStatus[Group] = ADC_STREAM_COMPLETED;
								Adc_Status = Adc_GroupStatus[Group];
							 Adc_ChannelGroupFinishedSamples[Group]=0;
				}
				else 
				{
					Adc_GroupStatus[Group] = ADC_COMPLETED;
					Adc_Status = Adc_GroupStatus[Group];
				}
			}
		}
	return 	Adc_GroupStatus[Group];
}

#if(ADC_GRP_NOTIF_CAPABILITY==STD_ON)
/***************************************************************************************
 * This function used to enable interrupt for specific group
 * takes group id.
 ***************************************************************************************/
void Adc_EnableGroupNotification(Adc_GroupType Group)
{
	
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_ENABLE_GROUP_NOTIFICATION_SID, ADC_E_PARAM_GROUP);
						}
						if(Adc_DriverState == ADC_DRIVER_NOT_INIT)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_ENABLE_GROUP_NOTIFICATION_SID, ADC_E_UNINIT);
						}
						else
						
	#endif
	{
	Adc_HwUnitType		Adc_HwUnitId = Adc_ChannelGroupHwUnitId[Group];
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[Adc_HwUnitId];
	Adc_ChannelGroupConfigType* ChannelGroupPtr=&Adc_ChannelGroupConfig[Group];
	Adc_ModuleType Adc_ModuleId = HwUnitPtr->AdcModuleId;
	Adc_SequencerType Adc_SequencerId= HwUnitPtr->AdcSequencerId;
	
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
	if((ChannelGroupPtr->AdcNotification) == NULL_PTR)
	{
		Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_ENABLE_GROUP_NOTIFICATION_SID, ADC_E_NOTIF_CAPABILITY);
	}
	else
	#endif
	{	
	// Group Notificaion is enabled.
	Adc_ChannelGroupNotificEnabled[Group] = TRUE;
	
	// Configure the interrupt by enabling and arming the interrupt.
	ADCIM_REG(Adc_ModuleId) |= (1 << Adc_SequencerId);
	SET_INT_PRI(Adc_SSIRQ[Adc_ModuleId][Adc_SequencerId], Adc_SSHandlerPri[Adc_ModuleId][Adc_SequencerId]);
	EN_INT(Adc_SSIRQ[Adc_ModuleId][Adc_SequencerId]);	
	}
}
}


/* Disables the notification mechanism for the requested ADC Channel group. */
void Adc_DisableGroupNotification(Adc_GroupType Group)
{
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
						if(Group >= NUMBER_OF_CHANNEL_GROUPS)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_ENABLE_GROUP_NOTIFICATION_SID, ADC_E_PARAM_GROUP);
						}
						if(Adc_DriverState == ADC_DRIVER_NOT_INIT)
						{
							Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_ENABLE_GROUP_NOTIFICATION_SID, ADC_E_UNINIT);
						}
						else
						
	#endif
	{
	Adc_HwUnitType		Adc_HwUnitId = Adc_ChannelGroupHwUnitId[Group];
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[Adc_HwUnitId];
	Adc_ChannelGroupConfigType* ChannelGroupPtr=&Adc_ChannelGroupConfig[Group];
	Adc_ModuleType Adc_ModuleId = HwUnitPtr->AdcModuleId;
	Adc_SequencerType Adc_SequencerId= HwUnitPtr->AdcSequencerId;
	
	#if(ADC_DEV_ERROR_DETECT==STD_ON)
	if((ChannelGroupPtr->AdcNotification) == NULL_PTR)
	{
		Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_ENABLE_GROUP_NOTIFICATION_SID, ADC_E_NOTIF_CAPABILITY);
	}
	else
	#endif
	{
	// Group Notificaion is Disabled.
	Adc_ChannelGroupNotificEnabled[Group] = FALSE;
	
	// Configure the interrupt by enabling and arming the interrupt.
	ADCIM_REG(Adc_ModuleId) &= (~(1<<Adc_SequencerId));
	}
}
}
#endif


/* reentrant function 
   Synchronus function used in init one sequencer */
static void Adc_ConfigureSequencern(Adc_GroupType Group)
{
	/* local variables defined and used inside the function */
	Adc_ChannelGroupConfigType *ChannelGroupPtr;
	Adc_HwUnitConfigType* HwUnitPtr;
	uint8_least Adc_GebroLoopIdx=0;
	/* Id of hw unit coressponding to the channel group */
	Adc_HwUnitType Adc_HwUnit = Adc_ChannelGroupHwUnitId[Group];
	Adc_SequencerType Adc_SequencerId;
	Adc_ModuleType Adc_ModuleId;
	Adc_ChannelType Adc_ChannelNumber;
	HwUnitPtr=&Adc_HwUnitConfig[Adc_HwUnit];
	ChannelGroupPtr = &Adc_ChannelGroupConfig[Group];
	Adc_SequencerId=(HwUnitPtr->AdcSequencerId);
	Adc_ModuleId=(HwUnitPtr->AdcModuleId);
	Adc_ChannelNumber=(HwUnitPtr->Adc_NumberOfInputChannels);
	
	
	/* disble the corresponding sequencer before configuration */
	ADCACTSS_REG(Adc_ModuleId) &= ~(1 << (Adc_SequencerId));
	
	// Adjust input channels of the corresponding sequencer. 
	for (Adc_GebroLoopIdx=0; Adc_GebroLoopIdx < Adc_ChannelNumber ; Adc_GebroLoopIdx++)
	{
		//clear the corresponding four bits
		ADC_SSMUX(Adc_ModuleId, Adc_SequencerId) &= ~(0x0F << (Adc_GebroLoopIdx*4)); 
		ADC_SSMUX(Adc_ModuleId, Adc_SequencerId) |= ((ChannelGroupPtr->AdcGroupDefinition[Adc_GebroLoopIdx]) << (Adc_GebroLoopIdx*4));
	}
	// first clear all control bits for the range of this channel-group samples.
	for (Adc_GebroLoopIdx=0; Adc_GebroLoopIdx < (Adc_ChannelNumber) ; Adc_GebroLoopIdx++)
	{
		ADC_SSCTL(Adc_ModuleId, Adc_SequencerId) &= ~(0x0F << (Adc_GebroLoopIdx*4));
	}
	
	// Set the IE bit and the end bit at the nibble of the last conversion. 
	ADC_SSCTL(Adc_ModuleId, Adc_SequencerId) |= (6 << (((Adc_ChannelNumber)-1)*4));
	
	/* configure the input event trigger */
	ADCEMUX_REG(Adc_ModuleId) &= ~(0xF) << (Adc_SequencerId*4);
	if((ChannelGroupPtr->AdcGroupTriggSrc) == ADC_TRIGG_SRC_SW)
	{
		ADCEMUX_REG(Adc_ModuleId) |= ((ChannelGroupPtr->AdcGroupConversionMode) << ((Adc_SequencerId)*4));
		/* enable the sequencer */
		ADCACTSS_REG(Adc_ModuleId) |= (1 << (Adc_SequencerId));	
	}
	else 
	{
		#if(ADC_HW_TRIGGER_API==STD_ON)
			ADCEMUX_REG(Adc_ModuleId) |= ((ChannelGroupPtr->AdcHwTriggerSource) << ((Adc_SequencerId)*4));
			/* we should use apis from functions to configure the hw event also here */
			/* enable sequencer in enable hw trigger api */
		#endif
	}
}



#if(ADC_GRP_NOTIF_CAPABILITY==STD_ON)
//*************************************************************************************************
//                                ADC_Handlers
//*************************************************************************************************
/**
 * In the ISR function I increment the number of conversion done till now.
 * transfer the result in the group buffer.
 * Check whether to fire the call back, this decision depends on the number of samples...
 * configured by the user.
 */
void ADC0SS0_Handler(void)
{
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[HW_UNIT_0];
	Adc_ChannelGroupConfigType* AdcChannelGroup= &Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
	Adc_StreamNumSampleType Samples_Finished =Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group];
	Adc_InputNumChType Ch_Num = HwUnitPtr->Adc_NumberOfInputChannels;
	uint8_least Adc_GebroLoopIdx=0;

	for(Adc_GebroLoopIdx = (Samples_Finished*(Ch_Num));(Adc_GebroLoopIdx<(Ch_Num*(Samples_Finished+1)));Adc_GebroLoopIdx++)
	{
		(*((DataBufferPtrAddr[HwUnitPtr->Adc_Group])+Adc_GebroLoopIdx)) = ADC_SSFIFO(ADC_MODULE_0,ADC_SEQUENCER_0);
	}
	Samples_Finished++;
	/* clear the INR bit in RIS by setting 1 in the ISC regiter */
	 ADCISC_REG(ADC_MODULE_0) |= (1<<ADC_SEQUENCER_0);
	if(Samples_Finished == (AdcChannelGroup->AdcStreamingNumSamples))
	{
		 Samples_Finished=0;
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_STREAM_COMPLETED;
		(AdcChannelGroup->AdcNotification());
	}
	else
	{
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_COMPLETED;
	}
	Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group] = Samples_Finished;
}

void ADC0SS1_Handler(void)
{
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[HW_UNIT_1];
	Adc_ChannelGroupConfigType* AdcChannelGroup= &Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
	Adc_StreamNumSampleType Samples_Finished =Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group];
	Adc_InputNumChType Ch_Num = HwUnitPtr->Adc_NumberOfInputChannels;
	uint8_least Adc_GebroLoopIdx=0;

	for(Adc_GebroLoopIdx = (Samples_Finished*(Ch_Num));(Adc_GebroLoopIdx<(Ch_Num*(Samples_Finished+1)));Adc_GebroLoopIdx++)
	{
		(*((DataBufferPtrAddr[HwUnitPtr->Adc_Group])+Adc_GebroLoopIdx)) = ADC_SSFIFO(ADC_MODULE_0, ADC_SEQUENCER_1);
	}
	Samples_Finished++;
	/* clear the INR bit in RIS by setting 1 in the ISC regiter */
	 ADCISC_REG(ADC_MODULE_0) |= (1<<ADC_SEQUENCER_1);
	if(Samples_Finished == (AdcChannelGroup->AdcStreamingNumSamples))
	{
		 Samples_Finished=0;
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_STREAM_COMPLETED;
		(AdcChannelGroup->AdcNotification());
	}
	else
	{
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_COMPLETED;
	}
	Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group] = Samples_Finished;
}	
void ADC0SS2_Handler(void)
{
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[HW_UNIT_2];
	Adc_ChannelGroupConfigType* AdcChannelGroup= &Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
	Adc_StreamNumSampleType Samples_Finished =Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group];
	Adc_InputNumChType Ch_Num = HwUnitPtr->Adc_NumberOfInputChannels;
	uint8_least Adc_GebroLoopIdx=0;

	for(Adc_GebroLoopIdx = (Samples_Finished*(Ch_Num));(Adc_GebroLoopIdx<(Ch_Num*(Samples_Finished+1)));Adc_GebroLoopIdx++)
	{
		(*((DataBufferPtrAddr[HwUnitPtr->Adc_Group])+Adc_GebroLoopIdx)) = ADC_SSFIFO(ADC_MODULE_0,ADC_SEQUENCER_2);
	}
	Samples_Finished++;
	/* clear the INR bit in RIS by setting 1 in the ISC regiter */
	 ADCISC_REG(ADC_MODULE_0) |= (1<<ADC_SEQUENCER_2);
	if(Samples_Finished == (AdcChannelGroup->AdcStreamingNumSamples))
	{
		 Samples_Finished=0;
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_STREAM_COMPLETED;
		(AdcChannelGroup->AdcNotification());
	}
	else
	{
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_COMPLETED;
	}
	Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group] = Samples_Finished;
}	

void ADC0Seq3_Handler(void)
{
  Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[HW_UNIT_3];
	Adc_ChannelGroupConfigType* AdcChannelGroup= &Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
	Adc_StreamNumSampleType Samples_Finished =Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group];
	Adc_InputNumChType Ch_Num = HwUnitPtr->Adc_NumberOfInputChannels;
	uint8_least Adc_GebroLoopIdx=0;

	for(Adc_GebroLoopIdx = (Samples_Finished*(Ch_Num));(Adc_GebroLoopIdx<(Ch_Num*(Samples_Finished+1)));Adc_GebroLoopIdx++)
	{
		(*((DataBufferPtrAddr[HwUnitPtr->Adc_Group])+Adc_GebroLoopIdx)) = ADC_SSFIFO(ADC_MODULE_0,ADC_SEQUENCER_3);
	}
	Samples_Finished++;
	/* clear the INR bit in RIS by setting 1 in the ISC regiter */
	 ADCISC_REG(ADC_MODULE_0) |= (1<<ADC_SEQUENCER_3);
	if(Samples_Finished == (AdcChannelGroup->AdcStreamingNumSamples))
	{
		 Samples_Finished=0;
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_STREAM_COMPLETED;
	}
	else
	{
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_COMPLETED;
	}
  (*(AdcChannelGroup->AdcNotification))();
	Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group] = Samples_Finished;

}	
//*************************************************************************************************************
void ADC1SS0_Handler(void)
{
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[HW_UNIT_4];
	Adc_ChannelGroupConfigType* AdcChannelGroup= &Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
	Adc_StreamNumSampleType Samples_Finished =Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group];
	Adc_InputNumChType Ch_Num = HwUnitPtr->Adc_NumberOfInputChannels;
	uint8_least Adc_GebroLoopIdx=0;

	
	for(Adc_GebroLoopIdx = (Samples_Finished*(Ch_Num));(Adc_GebroLoopIdx<(Ch_Num*(Samples_Finished+1)));Adc_GebroLoopIdx++)
	{
		(*((DataBufferPtrAddr[HwUnitPtr->Adc_Group])+Adc_GebroLoopIdx)) = ADC_SSFIFO(ADC_MODULE_1,ADC_SEQUENCER_0);
	}
	Samples_Finished++;
	/* clear the INR bit in RIS by setting 1 in the ISC regiter */
	 ADCISC_REG(ADC_MODULE_1) |= (1<<ADC_SEQUENCER_0);
	if(Samples_Finished == (AdcChannelGroup->AdcStreamingNumSamples))
	{
		 Samples_Finished=0;
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_STREAM_COMPLETED;
		(AdcChannelGroup->AdcNotification());
	}
	else
	{
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_COMPLETED;
	}
	Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group] = Samples_Finished;
}

void ADC1SS1_Handler(void)
{
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[HW_UNIT_5];
	Adc_ChannelGroupConfigType* AdcChannelGroup= &Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
	Adc_StreamNumSampleType Samples_Finished =Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group];
	Adc_InputNumChType Ch_Num = HwUnitPtr->Adc_NumberOfInputChannels;
	uint8_least Adc_GebroLoopIdx=0;


	for(Adc_GebroLoopIdx = (Samples_Finished*(Ch_Num));(Adc_GebroLoopIdx<(Ch_Num*(Samples_Finished+1)));Adc_GebroLoopIdx++)
	{
		(*((DataBufferPtrAddr[HwUnitPtr->Adc_Group])+Adc_GebroLoopIdx)) = ADC_SSFIFO(ADC_MODULE_1,ADC_SEQUENCER_1);
	}
	Samples_Finished++;
	/* clear the INR bit in RIS by setting 1 in the ISC regiter */
	 ADCISC_REG(ADC_MODULE_1) |= (1<<ADC_SEQUENCER_1);
	if(Samples_Finished == (AdcChannelGroup->AdcStreamingNumSamples))
	{
		 Samples_Finished=0;
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_STREAM_COMPLETED;
		(AdcChannelGroup->AdcNotification());
	}
	else
	{
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_COMPLETED;
	}
	Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group] = Samples_Finished;
}	


void ADC1SS2_Handler(void)
{
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[HW_UNIT_6];
	Adc_ChannelGroupConfigType* AdcChannelGroup= &Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
	Adc_StreamNumSampleType Samples_Finished =Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group];
	Adc_InputNumChType Ch_Num = HwUnitPtr->Adc_NumberOfInputChannels;
	uint8_least Adc_GebroLoopIdx=0;

	for(Adc_GebroLoopIdx = (Samples_Finished*(Ch_Num));(Adc_GebroLoopIdx<(Ch_Num*(Samples_Finished+1)));Adc_GebroLoopIdx++)
	{
		(*((DataBufferPtrAddr[HwUnitPtr->Adc_Group])+Adc_GebroLoopIdx)) = ADC_SSFIFO(ADC_MODULE_1,ADC_SEQUENCER_2);
	}
	Samples_Finished++;
	/* clear the INR bit in RIS by setting 1 in the ISC regiter */
	 ADCISC_REG(ADC_MODULE_1) |= (1<<ADC_SEQUENCER_2);
	if(Samples_Finished == (AdcChannelGroup->AdcStreamingNumSamples))
	{
		 Samples_Finished=0;
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_STREAM_COMPLETED;
		(AdcChannelGroup->AdcNotification());
	}
	else
	{
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_COMPLETED;
	}
	Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group] = Samples_Finished;
}	



void ADC1SS3_Handler(void)
{
	Adc_HwUnitConfigType* HwUnitPtr = &Adc_HwUnitConfig[HW_UNIT_7];
	Adc_ChannelGroupConfigType* AdcChannelGroup= &Adc_ChannelGroupConfig[HwUnitPtr->Adc_Group];
	Adc_StreamNumSampleType Samples_Finished =Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group];
	Adc_InputNumChType Ch_Num = HwUnitPtr->Adc_NumberOfInputChannels;
	uint8_least Adc_GebroLoopIdx=0;

	for(Adc_GebroLoopIdx = (Samples_Finished*(Ch_Num));(Adc_GebroLoopIdx<(Ch_Num*(Samples_Finished+1)));Adc_GebroLoopIdx++)
	{
		(*((DataBufferPtrAddr[HwUnitPtr->Adc_Group])+Adc_GebroLoopIdx)) = ADC_SSFIFO(ADC_MODULE_1,ADC_SEQUENCER_3);
	}
	/* clear the INR bit in RIS by setting 1 in the ISC regiter */
	 ADCISC_REG(ADC_MODULE_1) |= (1<<ADC_SEQUENCER_3);
	Samples_Finished++;
	if(Samples_Finished == (AdcChannelGroup->AdcStreamingNumSamples))
	{
		 Samples_Finished=0;
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_STREAM_COMPLETED;
		(AdcChannelGroup->AdcNotification());
	}
	else
	{
		 Adc_GroupStatus[HwUnitPtr->Adc_Group]=ADC_COMPLETED;
	}
	Adc_ChannelGroupFinishedSamples[HwUnitPtr->Adc_Group] = Samples_Finished;
}
#endif

