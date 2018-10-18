#ifndef SPI_H
#define SPI_H


#include <stdint.h>
#include "SSI_Confg.h"
#include "M4MemMap.h"


#define GROUP_0 0
#define SSI_MODULES	4
#define SSI_MAX_MASTER_CLK 25000000
#define SSI_MAX_SLAVE_CLK  6000000	


typedef enum{ SSI_OK=0, SSI_NOK, ModuleId_Err, ClkScalar_Err} SSI_FunctionReturn;
typedef enum{ SSI_Master=0x00000000, SSI_SlaveOutputEn=0x00000004, SSI_SlaveOutputDis=0x0000000C} Operational_Mode;
typedef enum {SystemClk=0, PIOSC=0x05} Clock_Source;			
typedef enum {Clk_IdleLow=0, Clk_IdleHigh}Clock_Polarity;		// Page 955 data sheet
typedef enum {First_ClkEdge=0, Second_ClkEdge}Phase_Control;	// Page 955	data sheet
typedef enum {FreeScale_SPIForm=0, TI_FrameForm, MICROWARE_FrameForm} FrameFormat_Select;
typedef enum {Data_4Bit=3, Data_5Bit, Data_6Bit, Data_7Bit, Data_8Bit, Data_9Bit, Data_10Bit, Data_11Bit, Data_12Bit, Data_13Bit, Data_14Bit, Data_15Bit, Data_16Bit} Data_Size; 


typedef struct {
	uint8_t  			SSI_ModuleId;
	Clock_Source 		SSI_ClockSource;
	Operational_Mode 	SSI_OperationalMode;
	Clock_Polarity 		SSI_ClockPolarity;
	Phase_Control		SSI_PhaseControl;
	FrameFormat_Select  SSI_FrameFormat;
	Data_Size			SSI_DataSize;
	uint32_t 			SSI_ModuleClk;
}SSI_CfgType;

extern const SSI_CfgType SSI_ConfigParam[NUMBER_OF_GROUPS]; 

SSI_FunctionReturn SSI_Init(void);
#endif