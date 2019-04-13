#ifndef SPI_CFG_H
#define SPI_CFG_H
#include "../Std_Types/Std_Types.h"


/* This defines the range of data to be transmitted every time the main function is called
 max is 8, min = 1*/
#define SPI_DATA_TRANSMIT_PER_SLOT		8

#define DIO_GROUP_JOB_0						4

/* Number of hw units in the tm4c123gh6pm */
#define SPI_HW_UNITS 4

/* this defines the clk frequency used in the controller
	used in configuring the spi_baudrate */
#define SYSTEM_CLK									16000000

/* Id of spi module 0 */
#define SPI_HW_UNIT_0								0

/* Id of Spi module 1 */
#define SPI_HW_UNIT_1								1
	
/* Id of Spi Module 2 */	
#define SPI_HW_UNIT_2								2

/* Id of spi module 3 */
#define SPI_HW_UNIT_3								3

/* Number of all channels used */
#define SPI_MAX_CHANNEL								4

/* Number Of All Jobs Used */
#define SPI_MAX_JOB									1

/* Number of all sequences used */
#define SPI_MAX_SEQUENCE							3

/* Parent Container: SpiJob
	Description: Maximum Number of channels that can be 
				 included in one job */
#define MAX_NUM_CH_PER_JOB							4

/* Parent Container: SpiSeq
	Description: Maximum Number of Jobs that can be 
				 included in one sequence */
#define MAX_NUM_JOB_PER_SEQ							4

/*  Parent Container: SpiGeneral
	Description: Switches the Spi_Cancel function ON or OFF. */
#define SPI_CANCEL_API								STD_OFF

/*  Parent Container:  SpiGeneral
	Description: Selects the SPI Handler/Driver Channel Buffers usage allowed and
				 delivered. IB = 0; EB = 1; IB/EB = 2; */
#define SPI_CHANNEL_BUFFER_ALLOWED					1

/* Parent Container: SpiGeneral.
   Description: Switches the development error detection and notification on or off.
   true: detection and notification is enabled.
   false: detection and notification is disabled.
   */
#define SPI_DEV_ERROR_DETECT						STD_OFF

/* Parent Container: SpiGeneral.
   Description: Switches the Spi_GetHWUnitStatus function ON or OFF.
   */
#define SPI_HW_STATUS_API							STD_ON

/* Parent Container: SpiGeneral.
   Description: Switches the Interruptible Sequences handling functionality ON or OFF. */
#define SPI_INTERRUPTIBLE_SEQ_ALLOWED				STD_OFF

/* Parent Container: SpiGeneral.
	Description: Selects the SPI Handler/Driver level of scalable functionality that is
				 available and delivered. */
#define SPI_LEVEL_DELIVERED							1

/* Parent Container: SpiGeneral.
   Description: This parameter defines the cycle time of the function Spi_MainFunction_Handling in seconds.
   The parameter is not used by the driver it self, but it is used by upper layer. */
#define SPI_MAIN_FUNCTION_PERIOD					0.01

/* Parent Container: SpiGeneral.
   Description: Specifies whether concurrent Spi_SyncTransmit() calls
   for different se-quences shall be configurable.*/
#define SPI_SUPPORT_CONCURRENT_SYNC_TRANSMIT		STD_ON

/* Parent Container: SpiGeneral.
   Description: Header file name which will be included by the Spi.
   The value of this pa-rameter shall be used as h-char-sequence or q-char-sequence according to ISO C90 section 6.10.2 "source file inclusion". 
   The parameter value MUST NOT represent a path, since ISO C90 does not specify how such a path is treated
   (i.e., this is implementation defined (and additionally de-pends on the operating system and the underlying file system)).
   Type: String.*/
#define SPI_USER_CALL_BACK_HEADER_FILER				Spi_Irq.h

/* Parent Container: SpiGeneral.
   Description: Switches the Spi_GetVersionInfo function ON or OFF.*/
#define SPI_VERSION_INFO_API						STD_OFF

#endif
