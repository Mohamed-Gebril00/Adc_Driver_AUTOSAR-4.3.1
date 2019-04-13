/* Spi Handler according to SPI_SWS 4.3.1 */
/* Vendor: Mohmaed Ahmed Gebril 
   Faculty of engineering 
*/

#include "Spi_Reg.h"
#include "Spi.h"
#include "GPIO.h"


#define PUSH_REQ(HW_UNIT,ID)	  do{ \
																Spi_SeqPendingReq[HW_UNIT].Spi_Req	\
																[((Spi_SeqPendingReq[HW_UNIT].Front%SPI_MAX_SEQUENCE)+ \
																(Spi_SeqPendingReq[HW_UNIT].Size%SPI_MAX_SEQUENCE))%SPI_MAX_SEQUENCE]=ID; \
															  (Spi_SeqPendingReq[HW_UNIT].Size)++;} \
																while(0)
																
															
											
#define POP_REQ(HW_UNIT)	  Spi_SeqPendingReq[HW_UNIT].Spi_Req[(Spi_SeqPendingReq[HW_UNIT].Front%SPI_MAX_SEQUENCE)]; \
														do{ \
													  Spi_SeqPendingReq[HW_UNIT].Front++; \
													  Spi_SeqPendingReq[HW_UNIT].Size--;} \
													  while(0);
													

#define TRANSMIT_FIFO_NOT_FULL(HW_UNIT)				((SPISR_REG(HW_UNIT)&(TNF))>>TNF_BIT)
#define TRANSMIT_FIFO_EMPTY(HW_UNIT)				  ((SPISR_REG(HW_UNIT)&(TFE))>>TFE_BIT)

#define FINISHED(HW_UNIT)											((SPIRIS_REG(HW_UNIT)&(TXRIS))>>(TXRIS_BIT))											
											
#define SPI_MAX_MASTER_CLK 										25000000
#define SPI_MAX_SLAVE_CLK  										6000000


/* static functions used within the module only
 This Function is used to configure the spi baud rate */
static void Spi_ConfigureClockScalar(const HWUnit* CfgPtr);


/* static globacl variables used only in this file */
/* external buffer address holder for buffer used in transmission */
static Spi_DataBufferType* Spi_SrcEbAddressPtr[SPI_MAX_CHANNEL];

/* external buffer address holder for buffer used in receive */
static Spi_DataBufferType* Spi_DesEbAddressPtr[SPI_MAX_CHANNEL];

/* used to hold the length of the data to be transmitted or received per channel. */ 
static uint8 Spi_EbDataLength[SPI_MAX_CHANNEL];

/* Status of the HWnits during the run time */
static Spi_StatusType Spi_HWUnitStatus[SPI_HW_UNITS]={
	SPI_UNINIT,
	SPI_UNINIT,
	SPI_UNINIT,
	SPI_UNINIT};

/* 
	this variable defines the state of the sequence result 
	its range is:
	SPI_SEQ_OK:	The last transmission of the Sequence has been finished successfully.
	SPI_SEQ_PENDING: The SPI Handler/Driver is performing a SPI Sequence. The meaning of this status is equal to SPI_BUSY.
	SPI_SEQ_FAILED: The last transmission of the Sequence has failed.
	SPI_SEQ_CANCELED: The last transmission of the Sequence has been canceled by user. 
	*/
static Spi_SeqResultType spiSeqResult[SPI_MAX_SEQUENCE] = {SPI_SEQ_OK};

static Spi_JobResultType spiJobResult[SPI_MAX_JOB]={SPI_JOB_OK};

/* globa; variable holds the idx of the job within the running sequence that is in the tranmsition process */
static uint8 seqRunningJobIdx[SPI_MAX_SEQUENCE]={0};

/* state of the whole handler initialized or not */
static Spi_HandlerStateType Spi_HandlerState = SPI_HANDLER_NOT_INITIALIZED;
	
/* Holds the current requests to asynch-transmit */
static Spi_SeqPendingReqType Spi_SeqPendingReq[SPI_HW_UNITS] =
{
	{{0},0,0},
	{{0},0,0},
	{{0},0,0},
	{{0},0,0}
};	

/* Global array hold the running sequence for each hw_unit to be able to retrieve it every cyclic call */
static uint8 Spi_SeqRunning[SPI_HW_UNITS]={0};
	
/* Global array hold the running Job for each hw_unit to be able to retrieve it every cyclic call */
static uint8 Spi_JobRunning[SPI_HW_UNITS]={0};

/* Global array hold the running Channel for each hw_unit to be able to retrieve it every cyclic call */
static uint8 Spi_ChRunning[SPI_HW_UNITS]={0};

/* Global array hold the number of finished data unit inside every channel */ 
static uint8 Spi_ChFinishedTrans[SPI_MAX_CHANNEL]={0};

/* extern the configuration for channel, job and sequence */
extern SpiChannel Channel_Cfg[SPI_MAX_CHANNEL];
extern SpiJob Job_Cfg[SPI_MAX_JOB];
extern SpiSequence Sequence_Cfg[SPI_MAX_SEQUENCE];

	
/* Pointer to configuration set
   Service for SPI initialization. */
void Spi_Init(
				const Spi_ConfigType* Spi_ConfigPtr
)
{
	#if(SPI_DEV_ERROR_DETECT==STD_ON)
		/* Check the Adc driver state Configure */
		if(Spi_HandlerStateType == SPI_HANDLER_INITIALIZED)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_INIT_SID, SPI_E_ALREADY_INITIALIZED);
		}
		if(ConfigPtr == NULL_PTR)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_INIT_SID, SPI_E_PARAM_POINTER);
		}
		else
	#endif
		
	{
		uint8_least spiGebroLoopIdx=0;
		const HWUnit* CfgPtr;
		uint32 delay;
		for(spiGebroLoopIdx=SPI_HW_UNIT_0; spiGebroLoopIdx<=SPI_HW_UNIT_3;spiGebroLoopIdx++)
		{
			CfgPtr = (&(Spi_ConfigPtr->HwUnit[spiGebroLoopIdx]));
			if((CfgPtr->ConfigureHWUnit) == TRUE)
			{
				/* Enable clock for this module */
				RCGC_SPI |= (1<<(CfgPtr->SpiHWUnitId));
				delay = RCGC_SPI;
				/* ensure that bit sse is cleard before any changes */
				SPICR1_REG(CfgPtr->SpiHWUnitId)&=~(SSE);
				
				/* Select whether the SPI is a master or slave */
				SPICR1_REG(CfgPtr->SpiHWUnitId)=(CfgPtr->Spi_HWUnitMode);
				
				/* Configuraing the source that generates the spi baud rate */
				SPICC_REG(CfgPtr->SpiHWUnitId)=((CfgPtr->Spi_ClockConfiguration)&CS);
				
				/* setting the clock serail (CS) and Pre-scalar value) */
				Spi_ConfigureClockScalar(CfgPtr);
				
				/* This parameter defines the SPI data shift edge. 
				Range: LEADING data is captured in the first clock edge 
				TRAILING data is captured in the second clock edge.*/ 
				SPICR0_REG(CfgPtr->SpiHWUnitId) &= ~(SPH);
				SPICR0_REG(CfgPtr->SpiHWUnitId) |= ((CfgPtr->Spi_DataShiftEdge)<<SPH_BIT);
				
				/*This parameter defines the SPI shift clock idle level.*/
				SPICR0_REG(CfgPtr->SpiHWUnitId) &= ~(SPO);
				SPICR0_REG(CfgPtr->SpiHWUnitId) |= ((CfgPtr->Spi_ShiftClockIdle)<<SPO_BIT);
				
				/* adjusting the format of the SPI module within tiva c
				to SPI, as this is a handler for the SPI peripheral */
				SPICR0_REG(CfgPtr->SpiHWUnitId) &= ~(FRF);

				SPICR0_REG(CfgPtr->SpiHWUnitId) &= ~(DSS);
				SPICR0_REG(CfgPtr->SpiHWUnitId) |= ((CfgPtr->SpiDataFrameSize)<<DSS_BIT);
				
				/* Set TX EOT to 1 to know that transmission has finished */
				SPICR1_REG(CfgPtr->SpiHWUnitId) |= (1<<EOT_BIT);
				
				// enable the module 
				SPICR1_REG(CfgPtr->SpiHWUnitId) |= (SSE);
				
				/* set the state of the hwunit to idle */
				Spi_HWUnitStatus[CfgPtr->SpiHWUnitId]=SPI_IDLE;
			}
		}
		
		/* Handler already initialized */
		Spi_HandlerState = SPI_HANDRLER_INITIALIZED;
	}
}

/* Service for SPI de-initialization.
   Return value: E_OK: de-initialisation command has been accepted 
				 E_NOT_OK: de-initialisation command has not been accepted*/
Std_ReturnType Spi_DeInit(
				void 
)
{
		/* Not implemented yet */
	Std_ReturnType Std_Return=E_OK;
	return Std_Return;
}

/* Service to setup the buffers and the length of data 
for the EB SPI Handler/Driver Channel specified. 
Parameters:   Channel: Channel ID,
			  SrcDataBufferPtr: Pointer to source data buffer.
			  length: Length (number of data elements) of the data to be transmitted from SrcDataBufferPtr 
					  and/or received from DesDataBufferPtr Min.: 1 Max.: Max of data 
					  specified at configuration for this channel
			  DesDataBufferPtr: Pointer to destination data buffer in RAM.
			  Return value: Std_ReturnType: E_OK: Setup command has been accepted 
				E_NOT_OK: Setup command has not been accepted*/
#if(SPI_CHANNEL_BUFFER_ALLOWED==1)
Std_ReturnType Spi_SetupEB( 
							Spi_ChannelType Channel, 
							Spi_DataBufferType* SrcDataBufferPtr, 
							Spi_DataBufferType* DesDataBufferPtr,
							Spi_NumberOfDataType Length
						  )
{
	Std_ReturnType Std_Return=E_OK;
	#if(SPI_DEV_ERROR_DETECT==STD_ON)
		/* Check the Adc driver state Configure */
		if(Spi_HandlerStateType == SPI_HANDLER_NOT_INITIALIZED)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_SETUP_EB_SID, SPI_E_UNINIT);
			Std_Return=E_NOT_OK;
		}
		if(Channel >= SPI_MAX_CHANNEL)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_SETUP_EB_SID, SPI_E_PARAM_CHANNEL);
			Std_Return=E_NOT_OK;
		}
		else
	#endif
		
	{
		Spi_SrcEbAddressPtr[Channel] = SrcDataBufferPtr;
		Spi_DesEbAddressPtr[Channel] = DesDataBufferPtr;
		Spi_EbDataLength[Channel] = Length;
	}
	return Std_Return;
}
#endif

/* Re-entrant function 
	Asynchronous function */
Std_ReturnType Spi_AsyncTransmit( 
				Spi_SequenceType Sequence 
)
{
	uint16 spiGebroLoopIdx=0;
	Std_ReturnType Std_Return=E_OK;
	
		#if(SPI_DEV_ERROR_DETECT==STD_ON)
		/* Check the Adc driver state Configure */
		if(Spi_HandlerStateType == SPI_HANDLER_NOT_INITIALIZED)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_ASYNC_TRANSMIT_SID, SPI_E_UNINIT);
			Std_Return=E_NOT_OK;
		}
		if(Sequence >= SPI_MAX_SEQUENCE)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_ASYNC_TRANSMIT_SID, SPI_E_PARAM_SEQUENCE);
			Std_Return=E_NOT_OK;
		}
		else
	#endif
		
	{
		SpiSequence* SeqPtr=(&Sequence_Cfg[Sequence]);
		/* Disable interrupt because it's a re-entrant function 
		to protect tht global variable used */
		DISABLE_INTERRUPT();
		/* check the state of the sequence first 
		if it pending then return without doing thing */
		if((spiSeqResult[Sequence]!=SPI_SEQ_PENDING)){
			PUSH_REQ(SeqPtr->Spi_HWUnitId,Sequence);
			spiSeqResult[Sequence]=SPI_SEQ_PENDING;
			for(spiGebroLoopIdx=0; spiGebroLoopIdx<SeqPtr->SpiJobsNumber; spiGebroLoopIdx++)
			{
				spiJobResult[Sequence_Cfg[Sequence].SpiJobAssignment[spiGebroLoopIdx]] = SPI_JOB_QUEUED;
			}
		}
		ENABLE_INTERRUPT();
	}
	return Std_Return;
}


/*
Description: Service returns the SPI Handler/Driver software module status.
*/

/***************************************************************************************************************************/
void Spi_MainFunction_Handling(void)
{
		
		uint8 spiHwUnitGebroLoopIdx;
		uint32 seqIdx=0;
		uint32	jobIdx=0;
		uint32 chIdx=0;
		uint8 spiDataGebroLoopIdx;
  	DISABLE_INTERRUPT();
		/* check if there is no running sequence. */

		for(spiHwUnitGebroLoopIdx=SPI_HW_UNIT_0; spiHwUnitGebroLoopIdx<=SPI_HW_UNIT_3; spiHwUnitGebroLoopIdx++)
		{
		switch(Spi_HWUnitStatus[spiHwUnitGebroLoopIdx])
		{
			case(SPI_IDLE):
				if(Spi_SeqPendingReq[spiHwUnitGebroLoopIdx].Size > 0)
				{
					// hw unit state is busy.
					Spi_HWUnitStatus[spiHwUnitGebroLoopIdx]=SPI_BUSY;
					
					// pop sequence to be transmitted.
					Spi_SeqRunning[spiHwUnitGebroLoopIdx]=POP_REQ(spiHwUnitGebroLoopIdx);
					seqIdx = Spi_SeqRunning[spiHwUnitGebroLoopIdx];
					
					//start transmit from the first job at idx 0 at the job-list.
					seqRunningJobIdx[seqIdx]=0;
					
					// save the idx that is transmitted.
					Spi_JobRunning[spiHwUnitGebroLoopIdx]=Sequence_Cfg[jobIdx].SpiJobAssignment[seqRunningJobIdx[jobIdx]];
					jobIdx = Spi_JobRunning[spiHwUnitGebroLoopIdx];
					
					// job state to pending.
					spiJobResult[jobIdx]=SPI_JOB_PENDING;
					
					// channel-idx within the job to be transmitted equals to 0.
					// indicate first channel.
					Job_Cfg[jobIdx].SpiRunningChIdx=0;
					Spi_ChRunning[spiHwUnitGebroLoopIdx]=Job_Cfg[jobIdx].ChannelList[Job_Cfg[jobIdx].SpiRunningChIdx];
					chIdx=Spi_ChRunning[spiHwUnitGebroLoopIdx];
					
					// count the finished data-bytes=0.
					Spi_ChFinishedTrans[Spi_ChRunning[spiHwUnitGebroLoopIdx]]=0;
				}
				else 
				{
					// do_nothing.
				}
			break;
		
		  case (SPI_BUSY):
			/* check channel finished all data in it or not if finished 
			check EOT bit to make sure all data has been transmitted
			check if job finished all channels in it or not if finished 
			check if sequence finished all jobs in it or not 
			if finished make hwunit idle */
				seqIdx = Spi_SeqRunning[spiHwUnitGebroLoopIdx];
				jobIdx = Spi_JobRunning[spiHwUnitGebroLoopIdx];
				chIdx = Spi_ChRunning[spiHwUnitGebroLoopIdx];
				if(Spi_EbDataLength[chIdx] == Spi_ChFinishedTrans[chIdx])
				{
						Job_Cfg[jobIdx].SpiRunningChIdx++;
				
						// if job has finished 
						if(Job_Cfg[jobIdx].SpiRunningChIdx == Job_Cfg[jobIdx].SpiNumberOfChannels)
						{
							spiJobResult[jobIdx]=SPI_JOB_OK;
							seqRunningJobIdx[seqIdx]++;
							
							// check if all sequence has finished 
							if(seqRunningJobIdx[seqIdx] == Sequence_Cfg[jobIdx].SpiJobsNumber)
							{
								spiSeqResult[seqIdx]=SPI_SEQ_OK;
								Spi_HWUnitStatus[spiHwUnitGebroLoopIdx]=SPI_IDLE;
							}
							// not finished then insert the other job
							else 
							{
								Spi_JobRunning[spiHwUnitGebroLoopIdx]=Sequence_Cfg[seqIdx].SpiJobAssignment[seqRunningJobIdx[seqIdx]]; 
								spiJobResult[jobIdx]=SPI_JOB_PENDING;
								Job_Cfg[Spi_JobRunning[spiHwUnitGebroLoopIdx]].SpiRunningChIdx=0;
								Spi_ChRunning[spiHwUnitGebroLoopIdx]=Job_Cfg[jobIdx].ChannelList[Job_Cfg[jobIdx].SpiRunningChIdx];
								Spi_ChFinishedTrans[Spi_ChRunning[spiHwUnitGebroLoopIdx]]=0;
							}
						}
						else 
						{
							 Spi_ChRunning[spiHwUnitGebroLoopIdx]=Job_Cfg[jobIdx].ChannelList[Job_Cfg[jobIdx].SpiRunningChIdx];
							 Spi_ChFinishedTrans[Spi_ChRunning[spiHwUnitGebroLoopIdx]]=0;
						}
				}
				else			// channel not finished send the coming bytes
				{
					for(spiDataGebroLoopIdx=0; 
					(spiDataGebroLoopIdx<SPI_DATA_TRANSMIT_PER_SLOT)&&(TRANSMIT_FIFO_NOT_FULL(spiHwUnitGebroLoopIdx))
					&&(Spi_EbDataLength[chIdx]!= Spi_ChFinishedTrans[chIdx]);spiDataGebroLoopIdx++)
					{
						if(NULL_PTR == Spi_SrcEbAddressPtr[chIdx])
						{
							SPIDR_REG(spiHwUnitGebroLoopIdx)=Channel_Cfg[chIdx].SpiDefaultData;
						}
						else
						{
							SPIDR_REG(spiHwUnitGebroLoopIdx)=(*(Spi_SrcEbAddressPtr[chIdx]+Spi_ChFinishedTrans[chIdx]));
						}
						if(Spi_DesEbAddressPtr != NULL_PTR)
						{
							(*(Spi_DesEbAddressPtr[chIdx]+Spi_ChFinishedTrans[chIdx]))= SPIDR_REG(spiHwUnitGebroLoopIdx);
						}
						else 
						{
							// do-nothing
						}
						Spi_ChFinishedTrans[chIdx]=Spi_ChFinishedTrans[chIdx]+1;
					}
				}
			break;
	
			default: // Do_Nothing
			break;
		}
}
	ENABLE_INTERRUPT();
}

/* This service returns the last transmission result of the specified Job.
Return value: Spi_JobResultType */							
Spi_JobResultType Spi_GetJobResult( 
									Spi_JobType Job
								  )
{
	 Spi_JobResultType Spi_JobResult = SPI_JOB_OK;
		#if(SPI_DEV_ERROR_DETECT==STD_ON)
		/* Check the Adc driver state Configure */
		if(Spi_HandlerStateType == SPI_HANDLER_NOT_INITIALIZED)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_GET_JOB_RESULT_SID, SPI_E_UNINIT);
		}
		if(Job >= SPI_MAX_Job)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_GET_JOB_RESULT_SID, SPI_E_PARAM_SEQUENCE);
		}
		else
	#endif
	{
		Spi_JobResult=spiJobResult[Job];
	}
	
	return Spi_JobResult;
}


/*  Return value: Spi_SeqResultType 
	Parameters: Sequence ID. An invalid sequence ID will return an undefined result.
	This service returns the last transmission result of the specified Sequence. */
Spi_SeqResultType Spi_GetSequenceResult( 
											Spi_SequenceType Sequence
									    )
{
	 Spi_SeqResultType Spi_SeqResult = SPI_SEQ_OK;
		#if(SPI_DEV_ERROR_DETECT==STD_ON)
		/* Check the Adc driver state Configure */
		if(Spi_HandlerStateType == SPI_HANDLER_NOT_INITIALIZED)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_GET_JOB_RESULT_SID, SPI_E_UNINIT);
		}
		if(Seqiemce >= SPI_MAX_SEQUENCE)
		{
			/* report the error to the det module */
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_GET_JOB_RESULT_SID, SPI_E_PARAM_SEQUENCE);
		}
		else
	#endif
	{
		Spi_SeqResult=spiSeqResult[Sequence];
	}
	
	return Spi_SeqResult;
}

/*****************************************************************************************************************************/
/* Configuring the bit rate for spi module by setting the values Serial clock rate
and the pre-scalling value */
static void Spi_ConfigureClockScalar(const HWUnit* CfgPtr)
{
	sint32 Pre_Scaling=0,SPI_ModuleClkMn=0,SPI_ModuleClkTemp=0;
	uint16 Temp_CPSDVSR=0;
	uint8 Temp_SCR=0,Ret_CPSDVSR=0,Ret_SCR=0,Err=0;
	#if(SPI_DEV_ERROR_DETECT==STD_ON)
	if(SPI_MASTER == (Spi_ConfigPtr->Spi_HWUnitMode))
	{	
		if((((Spi_ConfigPtr->SpiBitRate)*2) > SYSTEM_CLK)||((Spi_ConfigPtr->SpiBitRate)>SPI_MAX_MASTER_CLK))
		{
			Err = 1 ;
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_INIT_SID, SPI_E_MASTE_BAUD_RATE);
		}
		else 
		{
		}
	}
	else if(SPI_SLAVE==(Spi_ConfigPtr->Spi_HWUnitMode))
	{
		if(((Spi_ConfigPtr->SpiBitRate)*12>SYSTEM_CLK) || ((Spi_ConfigPtr->SpiBitRate)>SPI_MAX_SLAVE_CLK))
		{
			Err = 1;
			Det_ReportError(SPI_MODULE_ID, SPI_INSTANCE_ID, SPI_INIT_SID, SPI_E_SLAVE_BAUD_RATE);
		}
		else
		{
		}
	}
	#endif
	
	if( Err != 1)
	{
		Pre_Scaling=(SYSTEM_CLK/(CfgPtr->SpiBitRate));
		for(Temp_CPSDVSR = 2;Temp_CPSDVSR < 256; Temp_CPSDVSR+=2)
		{
			Temp_SCR = (Pre_Scaling / Temp_CPSDVSR) - 1;
			SPI_ModuleClkTemp = (SYSTEM_CLK)/(Temp_CPSDVSR*(1+Temp_SCR));
			SPI_ModuleClkTemp-= (CfgPtr->SpiBitRate);
			if(SPI_ModuleClkTemp < 0)
			{
				SPI_ModuleClkTemp*=-1;
			}
			else
			{
			}
			if( SPI_ModuleClkTemp <= SPI_ModuleClkMn && Temp_SCR <= 255)
			{
						SPI_ModuleClkMn = SPI_ModuleClkTemp;
						Ret_SCR = Temp_SCR;
						Ret_CPSDVSR = Temp_CPSDVSR;
			}
			else
			{
			}
		}
		// Pre-Scaling value clear then sit the new value.
		SPICPSR_REG(CfgPtr->SpiHWUnitId) &= ~(CPSDVSR);
		SPICPSR_REG(CfgPtr->SpiHWUnitId) |= (Ret_CPSDVSR&CPSDVSR);
		
		// Serial Clock Rate clear then set the new value.
		SPICR0_REG(CfgPtr->SpiHWUnitId) &= ~(SCR);
		SPICR0_REG(CfgPtr->SpiHWUnitId) |= (Ret_SCR<<SCR_BIT);
	}
	else 
	{
	}
}
