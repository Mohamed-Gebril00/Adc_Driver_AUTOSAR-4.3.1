#ifndef SPI_REG_H
#define SPI_REG_H

#include "../Std_Types/Std_Types.h"

/* number of spi modules existing in TM4C123GH6PM */
#define SPI_HW_UNITS 4

typedef volatile uint32* const SPI_RegAddType;

#define	RCGC_SPI (*(SPI_RegAddType)0x400FE61C)

#define SPI_MODULE0_BASEADDRESS	0x40008000
#define SPI_MODULE1_BASEADDRESS	0x40009000
#define SPI_MODULE2_BASEADDRESS	0x4000A000
#define SPI_MODULE3_BASEADDRESS	0x4000B000

static const uint32 SPI_ModulesBaseAddressLut[SPI_HW_UNITS]=
{
	SPI_MODULE0_BASEADDRESS,
	SPI_MODULE1_BASEADDRESS,
	SPI_MODULE2_BASEADDRESS,
	SPI_MODULE3_BASEADDRESS
};

#define SPI_REG_ADDRESS(MODULE,REG_OFFSET) ((SPI_ModulesBaseAddressLut[MODULE]+REG_OFFSET))

//**********************************************************************************************************************************************
// 
// Register Map Defined for SPI Modules in TM4C123GH6PM
//
//***********************************************************************************************************************************************
#define SPICR0_REG(MODULE)				(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x000))		/// SPI Control 0.
#define SPICR1_REG(MODULE)				(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x004))		/// SPI COntrol 1.
#define SPIDR_REG(MODULE)					(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x008))		/// SPI Data.
#define SPISR_REG(MODULE)					(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x00C))		/// SPI Status.
#define SPICPSR_REG(MODULE)				(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x010))		/// SPI clock Pre-scale.
#define SPIIM_REG(MODULE) 				(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x014))		/// SPI Interrupt Mask.
#define SPIRIS_REG(MODULE)				(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x018))		/// SPI raw interrupt status
#define SPIMIS_REG(MODULE)				(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x01C))		/// SPI Masked Interrupt status
#define SPIICR_REG(MODULE)				(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x020))		/// SPI Interrupt Clear
#define SPIDMACTL_REG(MODULE)			(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x024))		/// SPI DMA Control
#define SPICC_REG(MODULE)					(*(SPI_RegAddType)SPI_REG_ADDRESS(MODULE,0x028))		/// SPI Clock Configuration



//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPICR0 register.
//
//*************************************************************************************************************************************************
#define 		DSS				0x0000000F																	/// SPI Data Size Select
#define 		FRF				0x00000030																	/// SPI Frame Format Select
#define 		SPO				0x00000040																	/// SPI Clock Polarity
#define 		SPH				0x00000080																	/// SPI Serial Clock Phase
#define 		SCR				0x0000FF00																	/// SPI Serial Clock Rate 
#define			DSS_BIT			0																		/// Start Location of DSS
#define			FRF_BIT			5																		/// Start location of FRF
#define 		SPO_BIT			6																		
#define			SPH_BIT			7
#define 		SCR_BIT			8
	

//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPICR1 register.
//
//*************************************************************************************************************************************************
#define 	  	LBM 	   	0x00000001																		/// SPI loop_back mood.
#define 	  	SSE		   	0x00000002																		/// SPI Synchronous Serial Port Enable.
#define 		MS		   	0x00000004																		/// SPI Master/ Slave Select.
#define 		SOD		   	0x00000008																		/// SPI Slave mode output disable.
#define 		EOT		   	0x00000010																		/// End of transmiSPIon.
#define 		LBM_BIT    	0
#define 		SSE_BIT	   	1
#define			MS_BIT	   	2
#define 		SOD_BIT	   	3	
#define 		EOT_BIT	   	4


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPISR register.
//
//************************************************************************************************************************************************
#define 		BUSY	  0x000000010																		/// SPI Busy BIT.
#define 		RFF		  0x000000008																		/// SPI Receive FIFO full.
#define 		RNE		  0x000000004																		/// SPI receive FIFO Not Empty.
#define 		TNF	  0x000000002																		/// SPI transmit FIFO not full.
#define 		TFE		  0x000000001																		/// SPI transmit FIFO empty.
#define			BUSY_BIT  4
#define 		RFF_BIT	  3
#define			RNE_BIT   2
#define 		TNF_BIT	  1
#define 		TFE_BIT	  0	


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPICPSR register.
//
//************************************************************************************************************************************************
#define 		CPSDVSR	 0x000000FF																			/// SPI Clock Pre-Scalar Divider



//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPIIM register.
//
//************************************************************************************************************************************************
#define 		RORIM		0x00000001																			///	SPI Receive Overrun interrupt mask
#define 		RTIM		0x00000002																			/// SPI Receive time out interrupt mask
#define 		RXIM		0x00000004																			/// SPI Receive FIFO Interrupt mask
#define 		TXIM		0x00000008																			/// SPI Transmit FIFO Interrupt mask.
#define 		RORIM_BIT	0
#define 		RTIM_BIT	1
#define 		RXIM_BIT	2
#define 		TXIM_BIT	3



//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPIRIS register.
//
//************************************************************************************************************************************************
#define 	RORRIS		0x00000001																				/// SPI Receive raw interrupt status
#define		RTRIS		0x00000002 																				/// SPI Receive time out raw interrupt status
#define 	RXRIS		0x00000004																				/// SPI Receive FIFO Raw Interrupt Status
#define 	TXRIS		0x00000008 																				/// SPI Transmit FIFO Raw Interrupt Status
#define 	RORRIS_BIT	0																						
#define 	RTRIS_BIT	1
#define 	RXRIS_BIT	2
#define 	TXRIS_BIT	3



//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPIMIS register.
//
//************************************************************************************************************************************************
#define 	RORMIS		0x00000001																				/// SPI receive over-run masked interrupt status
#define 	RTMIS		0x00000002																				/// SPI Receive time-out masked interrupt status
#define 	RXMIS		0x00000004																				/// SPI Receive FIFO Masked Interrupt Status
#define 	TXMIS		0x00000008																				/// SPI Transmit FIFO Masked Interrupt Status
#define 	RORMIS_BIT	0	
#define 	RTMIS_BIT	1
#define		RXMIS_BIT	2
#define 	TXMIS_BIT	3


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPIICR register.
//
//************************************************************************************************************************************************
#define 	RORIC		0x00000001																				/// SPI Receive Overrun Interrupt clear
#define 	RTIC 		0x00000002																				/// SPI Receive over-run Interrupt clear


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPIDMACTL register.
//
//************************************************************************************************************************************************
#define     RXDMAE		0x00000001																				/// SPI Receive DMA Enable.
#define 	TXDMAE		0x00000002																				/// SPI Transmit DMA Enable.


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SPICC register.
//
//************************************************************************************************************************************************
#define 	CS			0x0000000F																				/// SPI Baud Clock Source

#endif
