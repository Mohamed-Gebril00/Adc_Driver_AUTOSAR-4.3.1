#ifndef SPI_TYPE_DEFS_H
#define SPI_TYPE_DEFS_H
/* Vendor of this module is Mohamed Ahmed Gebrils
 * Module Version 1.0.0
 */
#define SPI_TYPE_DEFS_SW_MAJOR_VERSION                             (1U)
#define SPI_TYPE_DEFS_SW_MINOR_VERSION                             (0U)
#define SPI_TYPE_DEFS_SW_PATCH_VERSION                         	   (0U)

#include "Spi_Cfg.h"


/* Name: Spi_StatusType.
   Enumberation.
   SPI_UNINIT: The SPI Handler/Driver is not initialized or not usable.
   SPI_IDLE: The SPI Handler/Driver is not currently transmitting any Job.
   SPI_BUSY: The SPI Handler/Driver is performing a SPI Job (trans-mit).
   Description: This type defines a range of specific status of SPI Handler/Driver. */
typedef enum {SPI_UNINIT=0, SPI_IDLE=0x01, SPI_BUSY=0x02} Spi_StatusType;

/* Mode for spi module 
   * note that AUTOSAR support only the master mode 
   * However I added the slave mode for the use of 
   * graduation project only */
typedef enum {SPI_MASTER = 0x0, SPI_SLAVE=0x4}Spi_HWUnitModeType;

typedef enum {SPI_SYSTEM_CLOCK=0, SPI_PIOSC=0x5}Spi_ClockConfigurationType;

/* Name: Spi_JobResultType.	
   Type: Enumeration
   Description: This type defines a range of specific jobs status for spi handler/driver.
   SPI_JOB_OK: The last transmission of the Job has been finished success-fully.
   SPI_JOB_PENDING: The SPI Handler/Driver is performing a SPI Job. The mean-ing of this status is equal to SPI_BUSY.
   SPI_JOB_FAILED: The last transmission of the Job has failed.
   SPI_JOB_QUEUED: An asynchronous transmit Job has been accepted, while actual transmission for this Job has not started yet.
   */
typedef enum {SPI_JOB_OK=0, SPI_JOB_PENDING, SPI_JOB_FAILED, SPI_JOB_QUEUED} Spi_JobResultType;

/* Name: Spi_SeqResultType.
   Type: Enumeration.
   Range: SPI_SEQ_OK -> The last transmission of the Sequence has been finished successfully.
		  SPI_SEQ_PENDING -> The SPI Handler/Driver is performing a SPI Sequence. The meaning of this status is equal to SPI_BUSY.
		  SPI_SEQ_FAILED -> The last transmission of the Sequence has failed.
		  SPI_SEQ_CANCELED -> The last transmission of the Sequence has been canceled by user.
   Description: This type defines a range of specific Sequences status for SPI Handler/Driver.
   */
typedef enum {SPI_SEQ_OK=0, SPI_SEQ_PENDING, SPI_SEQ_FAILED, SPI_SEQ_CANCELED} Spi_SeqResultType;

typedef enum{SPI_HANDRLER_INITIALIZED=0, SPI_HANDLER_NOT_INITIALIZED} Spi_HandlerStateType;

/* Definition of the bit_rate of the spi modules */
typedef uint32	Spi_BitRateType;
/* Type: uint8.
   Description: Type of application data buffer elements.
   */
typedef uint8 Spi_DataBufferType;

/* Name: Spi_NumberOfDataType.
   Type: uint16
   Description: Type for defining the number of data elements of the type Spi_DataBufferType to send and / or receive by Channel.
   */
typedef uint16 Spi_NumberOfDataType;

/* Name: Spi_ChannelType.
   Type: uint8
   Description: Specifies the ID for a channel 
   */
typedef uint8 Spi_ChannelType;

/* Name: Spi_JobType.
   Type: uint16
   Description: Specifies the ID for a job.
   */
typedef uint16 Spi_JobType;

/* Name: Spi_SequenceType.
   Type: uint8
   Description: Specifies the identification (ID) for a sequence of jobs.
   */
typedef uint8 Spi_SequenceType;

/* Name: Spi_HWUnitType.
   Type: uint8
   Descri[tion: Specifies the identification (ID) for a SPI Hardware microcontroller peripheral (unit).*/
typedef uint8 Spi_HWUnitType;

/* Name: Spi_AsyncModeType.
   Type: Enumeration
   Range: SPI_POLLING_MODE -> The asynchronous mechanism is ensured by polling,
							  so interrupts related to SPI busses handled asynchronously 
							  are disabled.
		  SPI_INTERRUPT_MODE -> The asynchronous mechanism is ensured by interrupt, 
								so interrupts related to SPI busses handled asynchronously are enabled.
							  
   Description: Specifies the asynchronous mechanism mode for SPI busses handled asynchro-nously in LEVEL 2. */
typedef enum{SPI_POLLING_MODE=0, SPI_INTERRUPT_MODE} Spi_AsyncModeType;

/* Description: When the Chip select handling is enabled (see SpiEnableCs), 
then this parameter specifies if the chip select is handled automatically
by Peripheral HW engine or via general purpose IO by Spi driver.*/ 
typedef enum{CS_VIA_GPIO=0, CS_VIA_PERIPHERAL_ENGINE} Spi_CsSelectionType;


/* This parameter defines the first starting bit for transmission. */
/* LSB: Transmission starts with the Least Significant Bit first 
   MSB: Transmission starts with the Most Signifi-cant Bit first */
typedef enum {SPI_LSB=0, SPI_MSB} Spi_TransferStartType;

/* This parameter defines the SPI shift clock idle level. */
/* SPI_CLK_LOW: idle level is low,
   SPI_CLK_HIGH: idle level is high.*/
typedef enum{SPI_CLK_LOW=0, SPI_CLK_HIGH}Spi_ShiftClockIdleType;

/* Notifican function type for spi */
typedef  void(*Spi_NotificationType)(void);

/* enum for defining the data frame size per module */
typedef enum {SPI_4_BITS=3, SPI_5_BITS, SPI_6_BITS, 
SPI_7_BITS, SPI_8_BITS, SPI_9_BITS, SPI_10_BITS, SPI_11_BITS, 
SPI_12_BITS, SPI_13_BITS, SPI_14_BITS, SPI_15_BITS, 
SPI_16_BITS} Spi_DataFrameSizeType; 


typedef enum{SPI_LEADING=0, SPI_TRAILING} Spi_DataShiftEdgeType;

/* structure defines the ChannelId and default data
   with default data width one byte. */ 
typedef struct {
	/* SPI Channel ID, used as parameter in SPI API functions. */
	/* this is a symbolic name generated for this parameter */
	Spi_ChannelType SpiChannelId;
	
	/* Description: The default data to be transmitted when 
	(for internal buffer or external buffer) 
	the pointer passed to Spi_WriteIB 
	(for internal buffer) or to Spi_SetupEB 
	(for external buffer) is NULL. */
	uint8 SpiDefaultData;
	
	/* This parameter contains the maximum size 
	(number of data elements) of data buffers 
	in case of EB Channels and only. */
	Spi_NumberOfDataType SpiEbMaxLength;

}SpiChannel;

/* Spi Job is a list of channels that share the same chip_select and the same HW unit */
typedef struct{
	/* SPI Job ID, used as parameter in SPI API functions. */
	Spi_JobType SpiJobId;
	
	/* Description: When the Chip select handling is enabled (see SpiEnableCs), 
				 then this parameter specifies if the chip select is handled automatically by Peripheral HW engine
				 or via general purpose IO by Spi driver
	Range: CS_VIA_GPIO: chip select handled via gpio by Spi driver.\
		   CS_VIA_PERIPHERAL_ENGINE: chip select is handled automatically by Peripheral HW engine.*/
	Spi_CsSelectionType Spi_CsSelection;
	
	/* This parameter is the symbolic name to identify the Chip Select (CS) allo-cated to this Job. 
	DIO_GROUP_0
	DIO_GROUP_1
	DIO_GROUP_2
	...*/
	uint8 SpiCsIdentifier;
	
	/* Determines whether the start bit of the channels in this job 
	   start by a least signficant bit or a most signficant bit */
	Spi_TransferStartType SpiTransferStart;
	
	/* This parameter defines the active polarity of Chip Select. 
	   STD_HIGH active polarity is high,
	   STD_LOW active polarity is low*/
	uint8 SpiCsPolarity;
	
	/* This parameter defines how many channels 
	   has been included in the included job */
	uint8 SpiNumberOfChannels;
	
	/* Index of the current running Channel within the ChannelList to keep track of the channels */
	uint8 SpiRunningChIdx;
	
	/* This member hold the address for the call back function called when Job finished */
	Spi_NotificationType SpiJobEndNotification;
	
	/* reference to the channels included in the current job */
	Spi_ChannelType ChannelList[MAX_NUM_CH_PER_JOB];
}SpiJob;

/* structure hold the information related to the sequences 
	sequences consist of different jobs */
typedef struct{
	/* ID of the SPI sequence */ 
	Spi_SequenceType SpiSequenceId;
	
	/* Spi hardware unit assignment */
	Spi_HWUnitType Spi_HWUnitId;
	
	/* number of jobs included in this sequence */
	uint8 SpiJobsNumber;
	
	/* Reference to the jobs included in the current sequence */
	Spi_JobType SpiJobAssignment[MAX_NUM_JOB_PER_SEQ];
	
}SpiSequence;

/* structure used to configure the hw units(HW modules) used */
typedef struct
{
	/* Id of the hw unit in TM4C123GH6PM 
	    in tiva c we have four modules 
		0 for module 1
		 1 for module 2
		 2 for module 3
		 4 for module 4 
		 table 15-1 page 951 pin description.*/
	Spi_HWUnitType SpiHWUnitId;
	
	/* True: Configure this hw unit to be used
	   false: Don't configure */
	boolean ConfigureHWUnit;

	/* mode for spi hw unit 
	master as SpiHandle-SWS says
	you can configure it as a slave 
	but this is not the case in sws spi-handler document */
	Spi_HWUnitModeType Spi_HWUnitMode;
	
	/* Choosing the source that generates the spi baud rate */
	Spi_ClockConfigurationType Spi_ClockConfiguration;
	
	/* Specifing the bit_rate of a Hardware unit */
	Spi_BitRateType	SpiBitRate;
	
	/* Configure the data size per hwunit */
	Spi_DataFrameSizeType SpiDataFrameSize;
	
	/* This parameter defines the SPI data shift edge. */
	/* Range: LEADING data is captured in the first clock edge 
       TRAILING data is captured in the second clock edge. 
			  */
	Spi_DataShiftEdgeType Spi_DataShiftEdge;
	
	/* This parameter defines the SPI shift clock idle level. */
	Spi_ShiftClockIdleType Spi_ShiftClockIdle;
	
}HWUnit;

/* Description: Structure hold the requirement to initialize the HW units for spi in TM4C123GH6PM 
				initialize all the reqgisters that is initialized once */
typedef struct
{
	/* Configuration of the four HWunits */
	HWUnit HwUnit[SPI_HW_UNITS];
}Spi_ConfigType;

/* Holds the current requests to asynch-transmit */
typedef struct 
{
	Spi_SequenceType Spi_Req[SPI_MAX_SEQUENCE];
	uint32 Front;
	uint32 Size;
}Spi_SeqPendingReqType;
#endif