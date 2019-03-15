#include "Spi_TypeDefs.h"

SpiChannel Channel_Cfg[SPI_MAX_CHANNEL]=
{
	{
		/* SPI Channel ID, used as parameter in SPI API functions. */
	    /* this is a symbolic name generated for this parameter */
		0,
		
		/* Description: The default data to be transmitted when 
	   (for internal buffer or external buffer) 
	   the pointer passed to Spi_WriteIB 
	   (for internal buffer) or to Spi_SetupEB 
	   (for external buffer) is NULL. */
		0,
		
		
		/* This parameter contains the maximum size 
		(number of data elements) of data buffers 
		in case of EB Channels and only. */
		8
	},
	{
	
		1,
		0,
		8
	},
	{
	
		2,
		0,
		8
	},
		{
	
		3,
		0,
		8
	}
};

SpiJob Job_Cfg[SPI_MAX_JOB]=
{
	{
		/* SPI Job ID, used as parameter in SPI API functions.*/
		0,		
	
	    /* Range: CS_VIA_GPIO: chip select handled via gpio by Spi driver.
		   CS_VIA_PERIPHERAL_ENGINE: chip select is handled automatically
		   by Peripheral HW engine.*/
		CS_VIA_PERIPHERAL_ENGINE, 
		
		/* This parameter is the symbolic name to identify the Chip Select (CS) 
		allo-cated to this Job.*/
		DIO_GROUP_JOB_0,
		
		/* Determines whether the start bit of the channels in this job 
	   start by a least signficant bit or a most signficant bit */
		SPI_LSB,
		
		/* This parameter defines the active polarity of Chip Select. 
	   STD_HIGH active polarity is high,
	   STD_LOW active polarity is low*/
		STD_LOW,
		
		/* This parameter defines how many channels 
	   has been included in the included job */
		4,
		
		/* Index of the current running Channel within the ChannelList to keep track of the channels */
		0,
		
		/* This member hold the address for the call back function called when Job finished */
		NULL_PTR,
		
		/* reference to the channels included in the current job 
		with maximum number of channels can be included 
		equals to MAX_NUM_CH_PER_JOB defined in Spi_Cfg.h*/
		{0,1,2,3}
	}
};


SpiSequence Sequence_Cfg[SPI_MAX_SEQUENCE]=
{
	{
		/* ID of the SPI sequence */ 
		0,
		
		/* Spi hardware unit assignment */
		0,
		
		/* number of jobs included in this sequence */
		1,
		
		/* Reference to the jobs included in the current sequence */
		{0}
	}
};

HWUnit HWUnit_Cfg[SPI_HW_UNITS]=
{
	{
		/* Id of the hw unit in TM4C123GH6PM 
	    in tiva c we have four modules 
		0 for module 1
		 1 for module 2
		 2 for module 3
		 4 for module 4 
		 table 15-1 page 951 pin description.*/
		0,
		
		/* True: Configure this hw unit to be used
	    false: Don't configure */
		TRUE,
		
		/* mode for spi hw unit 
	    master as SpiHandle-SWS says
	    you can configure it as a slave 
	    but this is not the case in sws spi-handler document */
		SPI_MASTER,
		
		/* Choosing the source that generates the spi baud rate */
		SPI_SYSTEM_CLOCK,
		
		/* Specifing the bit_rate of a Hardware unit */
		500000,
		
		/* Configure the data size per hwunit */
		SPI_8_BITS,
		
		/* This parameter defines the SPI data shift edge. */
		/* Range: LEADING data is captured in the first clock edge 
		   SPI_TRAILING data is captured in the second clock edge. 
			  */
		SPI_TRAILING,
		
		/* This parameter defines the SPI shift clock idle level. */
		SPI_CLK_LOW
		},
	{
		0,
		FALSE,
		SPI_MASTER,
		SPI_SYSTEM_CLOCK,
		1000000,
		SPI_8_BITS,
		SPI_TRAILING,
		SPI_CLK_LOW
	},
	{
		0,
		FALSE,
		SPI_MASTER,
		SPI_SYSTEM_CLOCK,
		1000000,
		SPI_8_BITS,
		SPI_TRAILING,
		SPI_CLK_LOW
	},
	{
		0,
		FALSE,
		SPI_MASTER,
		SPI_SYSTEM_CLOCK,
		1000000,
		SPI_8_BITS,
		SPI_TRAILING,
		SPI_CLK_LOW
	}
};

const Spi_ConfigType Spi_Config=
{
	{
		{
		/* Id of the hw unit in TM4C123GH6PM 
	    in tiva c we have four modules 
		0 for module 1
		 1 for module 2
		 2 for module 3
		 4 for module 4 
		 table 15-1 page 951 pin description.*/
		0,
		
		/* True: Configure this hw unit to be used
	    false: Don't configure */
		TRUE,
		
		/* mode for spi hw unit 
	    master as SpiHandle-SWS says
	    you can configure it as a slave 
	    but this is not the case in sws spi-handler document */
		SPI_MASTER,
		
		/* Choosing the source that generates the spi baud rate */
		SPI_SYSTEM_CLOCK,
		
		/* Specifing the bit_rate of a Hardware unit */
		1000000,
		
		/* Configure the data size per hwunit */
		SPI_8_BITS,
		
		/* This parameter defines the SPI data shift edge. */
		/* Range: LEADING data is captured in the first clock edge 
		   SPI_TRAILING data is captured in the second clock edge. 
			  */
		SPI_TRAILING,
		
		/* This parameter defines the SPI shift clock idle level. */
		SPI_CLK_LOW
		},
		{
			0,
			FALSE,
			SPI_MASTER,
			SPI_SYSTEM_CLOCK,
			1000000,
			SPI_8_BITS,
			SPI_TRAILING,
			SPI_CLK_LOW
		},
		{
			0,
			FALSE,
			SPI_MASTER,
			SPI_SYSTEM_CLOCK,
			1000000,
			SPI_8_BITS,
			SPI_TRAILING,
			SPI_CLK_LOW
		},
		{
			0,
			FALSE,
			SPI_MASTER,
			SPI_SYSTEM_CLOCK,
			1000000,
			SPI_8_BITS,
			SPI_TRAILING,
			SPI_CLK_LOW
		}
	}
};
