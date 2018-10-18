#include "SSI.h"



// This is define address of the registers inside macros
typedef volatile uint32_t* const SSI_RegAddType;

//***************************************************************************************************************
//
// SSI Modules base address.
// 
//***************************************************************************************************************
#define SSI_MODULE0_BASEADDRESS	0x40008000
#define SSI_MODULE1_BASEADDRESS	0x40009000
#define SSI_MODULE2_BASEADDRESS	0x4000A000
#define SSI_MODULE3_BASEADDRESS	0x4000B000

static const uint32_t SSI_ModulesBaseAddressLut[SSI_MODULES]=
{
	SSI_MODULE0_BASEADDRESS,
	SSI_MODULE1_BASEADDRESS,
	SSI_MODULE2_BASEADDRESS,
	SSI_MODULE3_BASEADDRESS
};

#define SSI_REG_ADDRESS(MODULE,REG_OFFSET) ((SSI_ModulesBaseAddressLut[MODULE]+REG_OFFSET))

//**********************************************************************************************************************************************
// 
// Register Map Defined for SSI Modules in TM4C123GH6PM
//
//***********************************************************************************************************************************************
#define SSICR0_REG(MODULE)				(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x000))		/// SSI Control 0.
#define SSICR1_REG(MODULE)				(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x004))		/// SSI COntrol 1.
#define SSIDR_REG(MODULE)					(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x008))		/// SSI Data.
#define SSISR_REG(MODULE)					(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x00C))		/// SSI Status.
#define SSICPSR_REG(MODULE)				(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x010))		/// SSI clock Pre-scale.
#define SSIIM_REG(MODULE) 				(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x014))		/// SSI Interrupt Mask.
#define SSIRIS_REG(MOCULE)				(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x018))		/// SSI raw interrupt status
#define SSIMIS_REG(MODULE)				(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x01C))		/// SSI Masked Interrupt status
#define SSIICR_REG(MODULE)				(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x020))		/// SSI Interrupt Clear
#define SSIDMACTL_REG(MODULE)			(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x024))		/// SSI DMA Control
#define SSICC_REG(MODULE)					(*(SSI_RegAddType)SSI_REG_ADDRESS(MODULE,0x028))		/// SSI Clock Configuration



//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSICR0 register.
//
//*************************************************************************************************************************************************
#define 		DSS				0x0000000F																	/// SSI Data Size Select
#define 		FRF				0x00000030																	/// SSI Frame Format Select
#define 		SPO				0x00000040																	/// SSI Clock Polarity
#define 		SPH				0x00000080																	/// SSI Serial Clock Phase
#define 		SCR				0x0000FF00																	/// SSI Serial Clock Rate 
#define			DSS_BIT			0																		/// Start Location of DSS
#define			FRF_BIT			5																		/// Start location of FRF
#define 		SPO_BIT			6																		
#define			SPH_BIT			7
#define 		SCR_BIT			8
	

//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSICR1 register.
//
//*************************************************************************************************************************************************
#define 	  LBM 	   	0x00000001																		/// SSI loop_back mood.
#define 		SSE		   	0x00000002																		/// SSI Synchronous Serial Port Enable.
#define 		MS		   	0x00000004																		/// SSI Master/ Slave Select.
#define 		SOD		   	0x00000008																		/// SSI Slave mode output disable.
#define 		EOT		   	0x00000010																		/// End of transmission.
#define 		LBM_BIT    	0
#define 		SSE_BIT	   	1
#define			MS_BIT	   	2
#define 		SOD_BIT	   	3	
#define 		EOT_BIT	   	4


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSISR register.
//
//************************************************************************************************************************************************
#define 		BUSY	  0x000000010																		/// SSI Busy BIT.
#define 		RFF		  0x000000008																		/// SSI Receive FIFO full.
#define 		RNE		  0x000000004																		/// SSI receive FIFO Not Empty.
#define 		TNF		  0x000000002																		/// SSI transmit FIFO not full.
#define 		TFE		  0x000000001																		/// SSI transmit FIFO empty.
#define			BUSY_BIT  4
#define 		RFF_BIT	  3
#define			RNE_BIT   2
#define 		TNE_BIT	  1
#define 		TFE_BIT	  0	


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSICPSR register.
//
//************************************************************************************************************************************************
#define 		CPSDVSR	 0x000000FF																			/// SSI Clock Pre-Scalar Divider



//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSIIM register.
//
//************************************************************************************************************************************************
#define 		RORIM		0x00000001																			///	SSI Receive Overrun interrupt mask
#define 		RTIM		0x00000002																			/// SSI Receive time out interrupt mask
#define 		RXIM		0x00000004																			/// SSI Receive FIFO Interrupt mask
#define 		TXIM		0x00000008																			/// SSI Transmit FIFO Interrupt mask.
#define 		RORIM_BIT	0
#define 		RTIM_BIT	1
#define 		RXIM_BIT	2
#define 		TXIM_BIT	3



//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSIRIS register.
//
//************************************************************************************************************************************************
#define 	RORRIS		0x00000001																				/// SSI Receive raw interrupt status
#define		RTRIS		0x00000002 																				/// SSI Receive time out raw interrupt status
#define 	RXRIS		0x00000004																				/// SSI Receive FIFO Raw Interrupt Status
#define 	TXRIS		0x00000008 																				/// SSI Transmit FIFO Raw Interrupt Status
#define 	RORRIS_BIT	0																						
#define 	RTRIS_BIT	1
#define 	RXRIS_BIT	2
#define 	TXRIS_BIT	3



//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSIMIS register.
//
//************************************************************************************************************************************************
#define 	RORMIS		0x00000001																				/// SSI receive over-run masked interrupt status
#define 	RTMIS		0x00000002																				/// SSI Receive time-out masked interrupt status
#define 	RXMIS		0x00000004																				/// SSI Receive FIFO Masked Interrupt Status
#define 	TXMIS		0x00000008																				/// SSI Transmit FIFO Masked Interrupt Status
#define 	RORMIS_BIT	0	
#define 	RTMIS_BIT	1
#define		RXMIS_BIT	2
#define 	TXMIS_BIT	3


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSIICR register.
//
//************************************************************************************************************************************************
#define 	RORIC		0x00000001																				/// SSI Receive Overrun Interrupt clear
#define 	RTIC 		0x00000002																				/// SSI Receive over-run Interrupt clear


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSIDMACTL register.
//
//************************************************************************************************************************************************
#define     RXDMAE		0x00000001																				/// SSI Receive DMA Enable.
#define 	TXDMAE		0x00000002																				/// SSI Transmit DMA Enable.


//************************************************************************************************************************************************
//
// The following are defined for the bit fields in the SSICC register.
//
//************************************************************************************************************************************************
#define 	CS			0x0000000F																				/// SSI Baud Clock Source


/***
   * Static function used to compute the SSI pre-scalar value,
   * and serial clock rate.
   */
static SSI_FunctionReturn SSI_ConfigureClockScalar(uint8_t SSI_GroupId, uint8_t *SSI_PtrCPSDVSR,uint8_t* SSI_PtrSCR)
{
	SSI_FunctionReturn Function_validationCheck = SSI_OK;
	int32_t Pre_Scaling=0,SSI_ModuleClkMn=0,SSI_ModuleClkTemp=0;
	uint16_t Temp_CPSDVSR=0;
	uint8_t Temp_SCR=0,Ret_CPSDVSR=0,Ret_SCR=0;
	const SSI_CfgType * CfgPtr;
	CfgPtr=&SSI_ConfigParam[SSI_GroupId];
	if(	CfgPtr -> SSI_OperationalMode == SSI_Master)
	{	
		if( CfgPtr -> SSI_ModuleClk*2 > SYSTEM_CLK ||  CfgPtr -> SSI_ModuleClk > SSI_MAX_MASTER_CLK )
		{
			Function_validationCheck = ClkScalar_Err ;
		}
		else 
		{
		}
	}
	else if(  CfgPtr -> SSI_OperationalMode == SSI_SlaveOutputEn ||  CfgPtr ->SSI_OperationalMode == SSI_SlaveOutputDis)
	{
		if(  CfgPtr -> SSI_ModuleClk*12 > SYSTEM_CLK ||  CfgPtr -> SSI_ModuleClk > SSI_MAX_SLAVE_CLK )
		{
			Function_validationCheck = ClkScalar_Err;
		}
		else
		{
		}
	}
	if( Function_validationCheck != ClkScalar_Err)
	{
		Pre_Scaling= ( SYSTEM_CLK / (CfgPtr ->SSI_ModuleClk));
		for(Temp_CPSDVSR = 2;Temp_CPSDVSR < 256; Temp_CPSDVSR+=2)
		{
			Temp_SCR = (Pre_Scaling / Temp_CPSDVSR) - 1;
			SSI_ModuleClkTemp = (SYSTEM_CLK)/(Temp_CPSDVSR*(1+Temp_SCR));
			SSI_ModuleClkTemp-= (CfgPtr ->SSI_ModuleClk);
			if(SSI_ModuleClkTemp < 0)
			{
				SSI_ModuleClkTemp*=-1;
			}
			else
			{
			}
			if( SSI_ModuleClkTemp <= SSI_ModuleClkMn && Temp_SCR <= 255)
			{
						SSI_ModuleClkMn = SSI_ModuleClkTemp;
						Ret_SCR = Temp_SCR;
						Ret_CPSDVSR = Temp_CPSDVSR;
			}
			else
			{
			}
		}
		*SSI_PtrCPSDVSR = Ret_CPSDVSR;
		*SSI_PtrSCR = Ret_SCR;
	}
	else 
	{
	}
	return Function_validationCheck;
}
 


SSI_FunctionReturn SSI_Init(void)
{
	SSI_FunctionReturn Fucntion_ValidationCheck= SSI_OK,Called_FunctionValidation = SSI_OK;
	uint8_t Groups_Idx=0;
	uint32_t delay;
	const SSI_CfgType * CfgPtr;
	for (Groups_Idx=GROUP_0; Groups_Idx<NUMBER_OF_GROUPS; Groups_Idx++)
	{
		uint8_t SSI_CPSDVSR=2, SSI_SCR=0;
			uint8_t x;
		CfgPtr=&SSI_ConfigParam[Groups_Idx];
	 x=(CfgPtr->SSI_ModuleId);
		if( (CfgPtr -> SSI_ModuleId) > 3)
		{
			Fucntion_ValidationCheck= ModuleId_Err;
		}
		else 
		{
			// first Enable the SSI module in run mode gating control register
			RCGCSSI_REG |= (1 << (CfgPtr -> SSI_ModuleId));
			delay = RCGCSSI_REG;
			// second clear the SSE before any changes in SSICR1 Register
			SSICR1_REG(CfgPtr->SSI_ModuleId) &= ~(SSE);
			
			// Master Mode SSICR1 =0x00000000 Configuration DATA_SHEET page 961
			// Slave Mode Output enable 0x00000004
			// Slave Mode Output Disable 0x0000000C
			SSICR1_REG(CfgPtr->SSI_ModuleId) = CfgPtr->SSI_OperationalMode;
			
			// Configure the clock source Sysclk or PIOSC
			SSICC_REG(CfgPtr->SSI_ModuleId) = ((CfgPtr->SSI_ClockSource)&CS);
			
			// Get Pre-scalar (CPSDVSR) and Serial clock rate (SCR) value  
			Called_FunctionValidation = SSI_ConfigureClockScalar(Groups_Idx, &SSI_CPSDVSR, &SSI_SCR);
			if(Called_FunctionValidation == ClkScalar_Err)
			{
				Fucntion_ValidationCheck = ClkScalar_Err;
			}
			else 
			{
				
				// Pre-Scaling value.
				SSICPSR_REG(CfgPtr->SSI_ModuleId) &= ~(CPSDVSR);
				SSICPSR_REG(CfgPtr->SSI_ModuleId) |= (SSI_CPSDVSR&CPSDVSR);
		
				// Serial Clock Rate.
				SSICR0_REG(CfgPtr->SSI_ModuleId) &= ~(SCR);
				SSICR0_REG(CfgPtr->SSI_ModuleId) |= (SSI_SCR << SCR_BIT);
				
				SSICR0_REG(CfgPtr->SSI_ModuleId) &= ~(SPH);
				SSICR0_REG(CfgPtr->SSI_ModuleId) |= ((CfgPtr -> SSI_PhaseControl) << SPH_BIT);

				SSICR0_REG(CfgPtr->SSI_ModuleId) &= ~(SPO);
				SSICR0_REG(CfgPtr->SSI_ModuleId) |= ((CfgPtr -> SSI_ClockPolarity) << SPO_BIT);
				
				SSICR0_REG(CfgPtr->SSI_ModuleId) &= ~(FRF);
				SSICR0_REG(CfgPtr->SSI_ModuleId) |= ((CfgPtr -> SSI_FrameFormat) << FRF_BIT);
				
				SSICR0_REG(CfgPtr->SSI_ModuleId) &= ~(DSS);
				SSICR0_REG(CfgPtr->SSI_ModuleId) |= ((CfgPtr -> SSI_DataSize) << DSS_BIT);
				
				// Finally Set SSE bit 
				SSICR1_REG(CfgPtr->SSI_ModuleId) |= (SSE);
			}
		}
	}
	return Fucntion_ValidationCheck;
}
				
		
		
	

