#ifndef SPI_H
#define SPI_H
/* Vendor of this module is Mohamed Ahmed Gebrils
 * Module Version 1.0.0
 */
#define SPI_SW_MAJOR_VERSION                           (1U)
#define SPI_SW_MINOR_VERSION                           (0U)
#define SPI_SW_PATCH_VERSION                           (0U)

#include "Spi_TypeDefs.h"
#include "Spi_Cfg.h"
#include "Spi_PBCfg.h"

	
#define SPI_MODULE_ID									083		
#define SPI_INSTANCE_ID									0


/* API Service-Ids from SPI_SWS document */
#define SPI_INIT_SID													0x00
#define SPI_SETUP_EB_SID											0x05
#define SPI_GET_JOB_RESULT_SID								0x07
#define SPI_GET_SEQUENCE_RESULT_SID						0x08


/* Errors IDs P.48 SPI_SWS */
#define SPI_E_UNINIT						0x0A
#define SPI_E_BUSY							0x0B
#define SPI_E_IDLE							0x0C
#define SPI_E_ALREADY_INITIALIZED 			0x0D
#define SPI_E_PARAM_POINTER					0x0E
#define SPI_E_PARAM_CHANNEL					0x15
#define SPI_E_PARAM_SEQUENCE				0x16
#define SPI_E_NOTIF_CAPABILITY				0x18
#define SPI_E_BUFFER_UNINIT					0x19
#define SPI_E_NOT_DISENGAGED				0x1A
#define SPI_E_POWER_STATE_NOT_SUPPORTED		0x1B
#define SPI_E_TRANSITION_NOT_POSSIBLE		0x1C
#define SPI_E_PERIPHERAL_NOT_PREPARED		0x1D
#define SPI_E_MASTER_BAUD_RATE				0x1E
#define SPI_E_SLAVE_BAUD_RATE				0x1F

/* Parameter: Pointer to configuration set
   Description: Service for SPI initialization. */
void Spi_Init(
				const Spi_ConfigType* Spi_ConfigPtr
);

/* Service for SPI de-initialization.
   Return value: E_OK: de-initialisation command has been accepted 
				 E_NOT_OK: de-initialisation command has not been accepted*/
Std_ReturnType Spi_DeInit(
				void 
);

/* Service for writing one or more data
 to an IB SPI Handler/Driver Channel specified by parameter.
 Return value: Std_ReturnType: E_OK: write command has been accepted
							   E_NOT_OK: write command has not been accepted */  
Std_ReturnType Spi_WriteIB(
				Spi_ChannelType Channel, 
				const Spi_DataBufferType* DataBufferPtr 
);

/* Service to transmit data on the SPI bus. 
   Std_ReturnType: E_OK: Transmission command has been accepted
				   E_NOT_OK: Transmission command has not been accepted*/
Std_ReturnType Spi_AsyncTransmit( 
				Spi_SequenceType Sequence 
);

/* Pointer to destination data buffer in RAM
   Service for reading synchronously one or more data from an IB SPI Handler/Driver 
   Channel specified by parameter. */
Std_ReturnType Spi_ReadIB( 
				Spi_ChannelType Channel, 
				Spi_DataBufferType* DataBufferPointer
);

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
Std_ReturnType Spi_SetupEB( 
							Spi_ChannelType Channel, 
							Spi_DataBufferType* SrcDataBufferPtr, 
							Spi_DataBufferType* DesDataBufferPtr,
							Spi_NumberOfDataType Length
						  );

/* Service returns the SPI Handler/Driver software module status. 
   Return value: Spi_StatusType.*/
Spi_StatusType Spi_GetStatus(
							void 
							);
							
/* This service returns the last transmission result of the specified Job.
Return value: Spi_JobResultType */							
Spi_JobResultType Spi_GetJobResult( 
									Spi_JobType Job
								  );

/*  Return value: Spi_SeqResultType 
	Parameters: Sequence ID. An invalid sequence ID will return an undefined result.
	This service returns the last transmission result of the specified Sequence. */
Spi_SeqResultType Spi_GetSequenceResult( 
											Spi_SequenceType Sequence
									    );

/* Description: Service to transmit data on the SPI bus
   Return value: Std_ReturnType: E_OK: Transmission command has been accepted 
				                 E_NOT_OK: Transmission command has not been accepted*/	
Std_ReturnType Spi_SyncTransmit( 
								 Spi_SequenceType Sequence
							   );

/* main function */
void Spi_MainFunction_Handling(
void
);

#endif
