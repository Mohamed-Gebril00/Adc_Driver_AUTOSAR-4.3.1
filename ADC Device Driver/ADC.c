/**
  * ADC definitions file (.c) for ADC drivers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */



#include "ADC.h"
#include "ADC_Confg.h"

#define GROUP0 0
// The coming macro gives the exact register address, and cast it then dereference it 
#define REGISTER(port_base,offset)   (*((volatile uint32_t*) (port_base+offset)))
	
#define CONC(x0,x1,x2,x3) CONC_HELPER(x0,x1,x2,x3)
#define CONC_HELPER(x0,x1,x2,x3) 0##x##x3##x2##x1##x0
#define ADC0SS0PRINUMBER	14
#define ADC0SS1PRINUMBER	15
#define ADC0SS2PRINUMBER	16
#define ADC0SS3PRINUMBER	17
#define	ADC1SS0PRINUMBER	48
#define ADC1SS1PRINUMBER	49
#define ADC1SS2PRINUMBER	50	
#define ADC1SS3PRINUMBER	51



/***************************************************************************
 * Interrupt enable and priority registers
 ***************************************************************************/
	static const uint32_t ADC_InterruptNum[]={ADC0SS0PRINUMBER, ADC0SS1PRINUMBER, ADC0SS2PRINUMBER,
																						ADC0SS3PRINUMBER, ADC1SS0PRINUMBER, ADC1SS1PRINUMBER,
																						ADC1SS2PRINUMBER, ADC1SS3PRINUMBER};
	
	
/*****************************************************************************
 * ADC Modules Base address.
 *****************************************************************************/
static const uint32_t ADC_ModuleBase[]={ADC0BASEADDRESS, ADC1BASEADDRESS};

/*****************************************************************************
 * ADC sample sequncers offsets' array for choosing input channels. 
******************************************************************************/
static const uint32_t ADC_SSMUXn[]={ADCSSMUX0, ADCSSMUX1, ADCSSMUX2, ADCSSMUX3};

/*****************************************************************************
 * ADC sample sequncers offsets' array for choosing sample control.  
******************************************************************************/
static const uint32_t ADC_SSCTLn[]={ADCSSCTL0, ADCSSCTL1, ADCSSCTL2, ADCSSCTL3};

/*******************************************************************************************
 * ADC sample sequncers offsets' array containig the FIFO offset for every sample sequencer.  
********************************************************************************************/
static const uint32_t ADC_FIFOn[]={ADCSSFIFO0, ADCSSFIFO1, ADCSSFIFO2, ADCSSFIFO3};

/*************************************************************************************************************
 * The Following array are defined for the bit fields in ADCSSMUXn registers
 *************************************************************************************************************/
static const uint32_t MUX_BitFields[]={MUX0, MUX1, MUX2, MUX3, MUX4, MUX5, MUX6, MUX7};


/*************************************************************************************************************
 * The Following are defined for each nibble for each SSn 
 *************************************************************************************************************/
static const uint8_t SSn_BITShift[]={S0BITSHIFT, S1BITSHIFT, S2BITSHIFT, S3BITSHIFT,
																		S4BITSHIFT, S5BITSHIFT, S6BITSHIFT, S7BITSHIFT};

/**************************************************************************************************************
 * The Following array are defined for the bit fields in ADCSSCTLn registers
 **************************************************************************************************************/
static const uint32_t Control_BitFields[]={SAMPLE0, SAMPLE2, SAMPLE3, SAMPLE4, SAMPLE5, SAMPLE6, SAMPLE7};



/******************************************************************************
 * The Following array is defined for the bit fields in ADCEMUX register (page: 830).
 ******************************************************************************/
static const uint8_t ADC_EMUXBitFieldShift[]={EM0SHIFT, EM1SHIFT, EM2SHIFT, EM3SHIFT};


/*************************************************************************
 * The Following are bit fields in interrupt mask.
 ************************************************************************/
static const uint8_t ADC_MaskBits[]={MASK0, MASK1, MASK2, MASK3};


/******************************************************************************
 * The Following array is defined for the bit fields in ADCISC register (page: 825).
 ******************************************************************************/
static const uint8_t ADC_ISCBitFields[]={IN0, IN1, IN2, IN3};

static const uint32_t PRI_OFFSET[]={INTPRI0, INTPRI1, INTPRI2,
																		INTPRI3, INTPRI4, INTPRI5,
																		INTPRI6, INTPRI7, INTPRI8,
																		INTPRI9, INTPRI10, INTPRI11,
																		INTPRI12, INTPRI13, INTPRI14,
																		INTPRI15, INTPRI16};
static const uint32_t EN_OFFSET[]={INTEN0, INTEN1, INTEN2, INTEN3};

static const uint32_t DIS_OFFSET[]={INTDIS0, INTDIS1, INTDIS2, INTDIS3};

											
/** 
  * this function is used within this file only (static function)
  * This Function is used to check Sequencers priorities it has to be different and takes values from 0 to 3
  * returns 0 completed without error
  * return  completed with error
  * Prioritization look page 801
  * Multiple active sample sequencer with the same priority do not provide consistent results
  */
static ADC_FunctionReturn ADC_PrioritiesCheck(uint8_t Copy_Pri0, uint8_t Copy_Pri1, uint8_t Copy_pri2, uint8_t Copy_Pri3)
{
	
	ADC_FunctionReturn Function_Validationcheck = ADC_OK;
	int8_t ADC_PriorityCheck[]={0, 0, 0, 0}, ADC_LoopIdx, ADC_Err=ADC_OK;
	
	// Check that priority of the sample sequencers is different
	// First check right input 
	if( ((Copy_Pri0) < 0) || ((Copy_Pri0) > 3) || ((Copy_Pri1) < 0) || ((Copy_Pri1) > 3) || ((Copy_pri2) < 0) || ((Copy_pri2) > 3) || ((Copy_Pri3) < 0) || ((Copy_Pri3) > 3))
	{
		ADC_Err=ADC_NOK;
	}
	else
	{
		ADC_PriorityCheck[Copy_Pri0]++;
		ADC_PriorityCheck[Copy_Pri1]++;
		ADC_PriorityCheck[Copy_pri2]++;
		ADC_PriorityCheck[Copy_Pri3]++;
	
		for(ADC_LoopIdx = 0; ADC_LoopIdx < 4 && !(ADC_Err); ADC_LoopIdx++)
		{
			if(ADC_PriorityCheck[ADC_LoopIdx] > 1 || ADC_PriorityCheck[ADC_LoopIdx] < 0)
			{	
				ADC_Err = ADC_NOK;
			}
			else 
			{
				// Do Nothing
			}
		}
	}
	if(ADC_Err == ADC_NOK)
	{
			Function_Validationcheck = ADC_NOK;
	}
	else 
	{
		Function_Validationcheck = ADC_OK;
	}
	
	return Function_Validationcheck;
}



/**
  * This static function used to configure module 0 Run mode clock gating control
  * If user in ADC_Confg.h Choosed CONFIGUREMODULE0 as ADC_ModuleOn
  * Without calling this function in case the user choosed ADC_ModuleOff
  * The program can't access Module 0 and all its functions are disabled
  * This function is seen from this file only
  */
static ADC_FunctionReturn ADC_ConfigureModule0(void)
{
	uint32_t delay;
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK, ADC_CalledFunctionReturn;
	
	// enable run mode clock gating control (bit 16 in RCGC0) Page 455.
	SYSCTL_RCGC0_R |= ADC0;
	delay = SYSCTL_RCGC0_R;
	
	// Specify Max sampling speed for module 0 bit 8, 9 in RCGC0 page 455.
	SYSCTL_RCGC0_R |= (ADC0MAXSAMPLING << MAXADC0SPD);
	
	 // Check Priorities of sample sequencers Module 0.
	ADC_CalledFunctionReturn = ADC_PrioritiesCheck(SAMPLESEQ0M0PRI, SAMPLESEQ1M0PRI, SAMPLESEQ2M0PRI, SAMPLESEQ3M0PRI);
	if(ADC_CalledFunctionReturn == ADC_OK)
	{
		REGISTER(ADC0BASEADDRESS, ADCSSPRI) = CONC(SAMPLESEQ0M0PRI, SAMPLESEQ1M0PRI, SAMPLESEQ2M0PRI, SAMPLESEQ3M0PRI);
	}
	else 
	{
			Function_ValidationCheck=ADC_NOK;
	}
	
	return Function_ValidationCheck;
}
	
/**
  * This static function used to configure module 1 Run mode clock gating control
  * If user in ADC_Confg.h Chosed CONFIGUREMODULE1 as ADC_ModuleOn
  * Without calling this function in case the user chosed ADC_ModuleOff
  * The program can't access Module 1 and all its functions are disabled
  * This function is seen from this file only
  */
static ADC_FunctionReturn ADC_ConfigureModule1(void)
{
	uint32_t delay;
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK, ADC_CalledFunctionReturn;
	
	// enable run mode clock gating control (bit 16 in RCGC0) Page 455.
	SYSCTL_RCGC0_R |= ADC1;
	delay = SYSCTL_RCGC0_R;
	
	 // Specify Max sampling speed for module 1 bit 10, 11 in RCGC0 page 455.
	SYSCTL_RCGC0_R |= ( ADC1MAXSAMPLING << MAXADC1SPD );
	
	 // Check Priorities of sample sequencers Module 1.
	ADC_CalledFunctionReturn = ADC_PrioritiesCheck(SAMPLESEQ0M1PRI, SAMPLESEQ1M1PRI, SAMPLESEQ2M1PRI, SAMPLESEQ3M1PRI);
	
	if(ADC_CalledFunctionReturn == ADC_OK)
	{
		REGISTER(ADC1BASEADDRESS, ADCSSPRI) = CONC(SAMPLESEQ0M1PRI, SAMPLESEQ1M1PRI, SAMPLESEQ2M1PRI, SAMPLESEQ3M1PRI);
	}
	else 
	{
		Function_ValidationCheck=ADC_NOK;
	}
	return Function_ValidationCheck;
}



/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/
// SSnMUX function for choosing the inpput channel
/**
	* This Function used to choose the input channel for the coming sequence for sequencer three
	*/
// SS3
static ADC_FunctionReturn ADC_SS3MUX(uint8_t ADC_GroupIdx)
{
	// Function validation 
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
	
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
			Function_ValidationCheck = Invalid_GroupNumber;
	}
	else 
	{
	// Sample Sequencer 3 channel register page 872
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX3) &=
			 ~(SAMPLE0);
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX3) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample0_Channel << S0BITSHIFT) & MUX0);
	}
	//return the validation of the function
return Function_ValidationCheck;	
}

// SS2
static ADC_FunctionReturn ADC_SS2MUX(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
			Function_ValidationCheck = Invalid_GroupNumber;
	}
	else 
	{
		// Sample Sequencer 2 channel register page 864
		// MUX0
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX2) |=
				( (ADC_GroupConfg[ADC_GroupIdx].Sample0_Channel << S0BITSHIFT) & MUX0);
		//MUX1
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX2) |=
				(( ADC_GroupConfg[ADC_GroupIdx].Sample1_Channel << S1BITSHIFT) & MUX1);
		//MUX2
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX2) |=
				( (ADC_GroupConfg[ADC_GroupIdx].Sample2_Channel << S2BITSHIFT) & MUX2);
		//MUX3
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX2) |=
				( (ADC_GroupConfg[ADC_GroupIdx].Sample3_Channel << S3BITSHIFT)& MUX3);
	}
	return Function_ValidationCheck;
	
}

// SS1
static ADC_FunctionReturn ADC_SS1MUX(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck = Invalid_GroupNumber;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
			Function_ValidationCheck = Invalid_GroupNumber;
	}
	else 
	{
	// Sample Sequencer 1 channel register page 864
	// MUX0
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX1) |=
			 ((ADC_GroupConfg[ADC_GroupIdx].Sample0_Channel  << S0BITSHIFT)& MUX0);
	//MUX1
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX1) |=
			 (( ADC_GroupConfg[ADC_GroupIdx].Sample1_Channel << S1BITSHIFT) & MUX1);
	//MUX2
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX1) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample2_Channel << S2BITSHIFT) & MUX2);
	//MUX3
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX1) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample3_Channel << S3BITSHIFT) & MUX3);
	}
	return Function_ValidationCheck;
	
}


// SS0
static ADC_FunctionReturn ADC_SS0MUX(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck = Invalid_GroupNumber;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
			Function_ValidationCheck = Invalid_GroupNumber;
	}
	else 
	{
	// Sample Sequencer 0 channel register page 848
	// MUX0
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample0_Channel << S0BITSHIFT) & MUX0);
	//MUX1
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ((ADC_GroupConfg[ADC_GroupIdx].Sample1_Channel << S1BITSHIFT) & MUX1);
	//MUX2
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample2_Channel << S2BITSHIFT) & MUX2);
	//MUX3
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample3_Channel << S3BITSHIFT) & MUX3);
	// MUX4
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample4_Channel << S4BITSHIFT) & MUX4);
	//MUX5
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample5_Channel << S5BITSHIFT) & MUX5);
	//MUX6
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample6_Channel << S6BITSHIFT) & MUX6);
	//MUX7
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId] ,ADCSSMUX0) |=
			 ( (ADC_GroupConfg[ADC_GroupIdx].Sample7_Channel << S7BITSHIFT) & MUX7);
	
	}
	return Function_ValidationCheck;
	
}
/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/


/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/
// Setting Sample Sequencer Control Bits 
// SSCTL3
static ADC_FunctionReturn ADC_SSCTL3(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck=ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = Invalid_GroupNumber;
	}
	else
	{
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL3) |= 
						((ADC_GroupConfg[ADC_GroupIdx].sample0 << S0BITSHIFT) & SAMPLE0);
	}
	return Function_ValidationCheck;
}

static ADC_FunctionReturn ADC_SSCTL2(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck=ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = Invalid_GroupNumber;
	}
	else
	{
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL2) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample0 << S0BITSHIFT) & SAMPLE0);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL2) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample1 << S1BITSHIFT) & SAMPLE1);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL2) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample2 << S2BITSHIFT) & SAMPLE2);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL2) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample3 << S3BITSHIFT) & SAMPLE3);
	}
	return Function_ValidationCheck;
}

static ADC_FunctionReturn ADC_SSCTL1(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck=ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = Invalid_GroupNumber;
	}
	else
	{
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL1) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample0 << S0BITSHIFT) & SAMPLE0);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL1) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample1 << S1BITSHIFT) & SAMPLE1);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL1) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample2 << S2BITSHIFT) & SAMPLE2);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL1) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample3 << S3BITSHIFT) & SAMPLE3);
	}
	return Function_ValidationCheck;
}

static ADC_FunctionReturn ADC_SSCTL0(uint8_t ADC_GroupIdx)
{
	ADC_FunctionReturn Function_ValidationCheck=ADC_OK;
	
	if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = Invalid_GroupNumber;
	}
	else
	{
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample0 << S0BITSHIFT) & SAMPLE0);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample1 << S1BITSHIFT) & SAMPLE1);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample2 << S2BITSHIFT) & SAMPLE2);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample3 << S3BITSHIFT) & SAMPLE3);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample4 << S4BITSHIFT) & SAMPLE4);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample5 << S5BITSHIFT) & SAMPLE5);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample6 << S6BITSHIFT) & SAMPLE6);
		REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId], ADCSSCTL0) |= 
		((ADC_GroupConfg[ADC_GroupIdx].sample7 << S7BITSHIFT) & SAMPLE7);
	}
	return Function_ValidationCheck;
}
/************************************************************************************************************/

/************************************************************************************************************
 * This function is used in Initializing  ADC Groups and modules
 ***********************************************************************************************************/
ADC_FunctionReturn ADC_Init(void)
{
	ADC_FunctionReturn Function_ValidationCheck = ADC_OK, ADC_CalledFunctionReturn = ADC_OK;
	uint8_t ADC_LoopIdx, ADC_Err=0;
	
	// Check Module 0 Configure
	if (CONFIGUREMODULE0 == ADC_ModuleOn)
	{
		ADC_CalledFunctionReturn = ADC_ConfigureModule0();
		if( ADC_CalledFunctionReturn == ADC_NOK)
			ADC_Err=1;
		else
		{
			// Do NOthing
		}
	}
	else 
	{
		// Do Nothing
	}
	
	if(CONFIGUREMODULE1 == ADC_ModuleOn)
	{
		ADC_CalledFunctionReturn = ADC_ConfigureModule1();
		if( ADC_CalledFunctionReturn == ADC_NOK)
		{
			ADC_Err=1;
		}
		else 
		{
			// Do NOthing
		}
	}
	else 
	{
		// Do Nothing
	}
	
	if(ADC_Err == 1)
	{
		Function_ValidationCheck = ADC_NOK;
	}
	else 
	{
		for( ADC_LoopIdx = GROUP0 ; ADC_LoopIdx < ADCNUMBEROFGROUPS ; ADC_LoopIdx++)
		{
			// First step disable the required sample seq in the required module 
			// as disabling the sequencer during initialization prevents erroneous execution if a trigger
			// event were to occur during the configuration process
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_LoopIdx].ADC_ModuleId], ADCACTSS) 
			&= ~(1 << ADC_GroupConfg[ADC_LoopIdx].ADC_SSId);
			 
			 
			 // Setting the triggering event
			 REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_LoopIdx].ADC_ModuleId], ADCEMUX) 
		  	|= (ADC_GroupConfg[ADC_LoopIdx].ADC_TriggeringEvent << ADC_EMUXBitFieldShift[ADC_GroupConfg[ADC_LoopIdx].ADC_SSId]);
			 
			 // Choosing input channel for the sequencer depending on the sequencer id (0, 1, 2, 3)
			 switch(ADC_GroupConfg[ADC_LoopIdx].ADC_SSId)
			 {
				case 3:
						ADC_CalledFunctionReturn = ADC_SS3MUX(ADC_LoopIdx);
						break;
				case 2:
						ADC_CalledFunctionReturn = ADC_SS2MUX(ADC_LoopIdx);
						break;
				case 1:
						ADC_CalledFunctionReturn = ADC_SS1MUX(ADC_LoopIdx);
						break;
				case 0:
						ADC_CalledFunctionReturn = ADC_SS0MUX(ADC_LoopIdx);
						break;
				default :
						Function_ValidationCheck = ADC_NOK;
						break;
			 }
			 if(ADC_CalledFunctionReturn == ADC_NOK)
			 {
					Function_ValidationCheck = ADC_NOK;
			 }
			 else 
			 {
				 // Do Nothing
			 }
			 
			 
			 // Setting sample control bit depending on the sequencer id(0, 1, 2, 3)
			 switch(ADC_GroupConfg[ADC_LoopIdx].ADC_SSId)
			 {
				case 3:
						ADC_CalledFunctionReturn = ADC_SSCTL3(ADC_LoopIdx);
						break;
				case 2:
						ADC_CalledFunctionReturn = ADC_SSCTL2(ADC_LoopIdx);
						break;
				case 1:
						ADC_CalledFunctionReturn = ADC_SSCTL1(ADC_LoopIdx);
						break;
				case 0:
						ADC_CalledFunctionReturn = ADC_SSCTL0(ADC_LoopIdx);
						break;
				default:
					Function_ValidationCheck= ADC_NOK;
					break;
			}
			if( ADC_CalledFunctionReturn == ADC_NOK)
			{
					Function_ValidationCheck= ADC_NOK;
			}
			else 
			{
				// Do Nothing
			}
			
			//enable SSid
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_LoopIdx].ADC_ModuleId], ADCACTSS) |= 
			(1 << ADC_GroupConfg[ADC_LoopIdx].ADC_SSId);
		}
		
	}
	return Function_ValidationCheck;
	
}

/*******************************************************************************************
  * This Function is used in getting trigger state for the running sequence 
  * return 0 completed successfully
	* return 1 terminated unsuccessful
	* takes the adc group idx, a pointer to a variable
  * return the trigger state in the variable pointed to it by the second parameter
  *******************************************************************************************/
ADC_FunctionReturn ADC_GetTriggerState(uint8_t ADC_GroupIdx,uint8_t *ADC_TriggerState)
{
		ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
		if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
		{
			Function_ValidationCheck = ADC_NOK;
		}
		else 
		{
			*ADC_TriggerState =
			((REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCRIS) >> ADC_GroupConfg[ADC_GroupIdx].ADC_SSId) & 1);
		}
		return Function_ValidationCheck;
}


// Get sample from FIFO
ADC_FunctionReturn  ADC_SSFIFO(uint8_t ADC_GroupIdx,uint32_t* ADC_SampleRes)
{
		ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
		if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
		{
			Function_ValidationCheck = ADC_NOK;
		}
		else 
		{
			*ADC_SampleRes =
			(REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADC_FIFOn[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId])
			&0xFFF);
		}
		return Function_ValidationCheck;
}

/*********************************************************************************************************
 * Variadic function used in initializing the sequencer input channels.
 *********************************************************************************************************/
ADC_FunctionReturn ADC_SSnChooseInCh(uint8_t ADC_GroupIdx,uint8_t ADC_NoSamples, ... )
{
	uint8_t Loop_Idx;
	ADC_FunctionReturn Function_ValidationCheck;
	va_list enumArgumentPointer;
	if(ADC_GroupIdx < GROUP0 && ADC_GroupIdx > ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = Invalid_GroupNumber;
	}
	else if(ADC_GroupConfg[ADC_GroupIdx].ADC_SSId == 0 && ADC_NoSamples > 1)
	{
		Function_ValidationCheck = Invalid_SS0SampleNumbers;
	}
	else if( ADC_GroupConfg[ADC_GroupIdx].ADC_SSId == 1 && ADC_NoSamples > 4)
	{
		Function_ValidationCheck = Invalid_SS1SampleNumbers;
	}
	else if (ADC_GroupConfg[ADC_GroupIdx].ADC_SSId == 2 && ADC_NoSamples > 4)
	{
		Function_ValidationCheck = Invalid_SS2SampleNumbers;
	}
	else if(	ADC_GroupConfg[ADC_GroupIdx].ADC_SSId == 3 && ADC_NoSamples > 8)
	{
		Function_ValidationCheck = Invalid_SS3SampleNumbers;
	}
	else 
	{
			// Disable the sequencer before changing the sample input channel.
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCACTSS)
			&= ~(ADC_GroupConfg[ADC_GroupIdx].ADC_SSId);
			for(Loop_Idx = 0 ;Loop_Idx < ADC_NoSamples; Loop_Idx++)
			{
				REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADC_SSMUXn[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId])
				&= ~(MUX_BitFields[Loop_Idx]);
				REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADC_SSMUXn[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId])
				|= ((MUX_BitFields[Loop_Idx] <<  SSn_BITShift[Loop_Idx]) & va_arg( enumArgumentPointer, ADC_Channel));
				
			}
			va_end( enumArgumentPointer );
			// Enable the sequencer.
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCACTSS)
			|= (ADC_GroupConfg[ADC_GroupIdx].ADC_SSId);
			Function_ValidationCheck = ADC_OK;
	}
	return Function_ValidationCheck;
}

/*********************************************************************************************************
 * Variadic function used in Configuring the sequencer Control bits in ADCSSCTLn Registers.
 *********************************************************************************************************/
ADC_FunctionReturn ADC_SSnControl(uint8_t ADC_GroupIdx,uint8_t ADC_NoSamples, ... )
{
	uint8_t Loop_Idx;
	ADC_FunctionReturn Function_ValidationCheck;
	va_list enumArgumentPointer;
	if(ADC_GroupIdx < GROUP0 && ADC_GroupIdx > ADCNUMBEROFGROUPS)
	{
		Function_ValidationCheck = Invalid_GroupNumber;
	}
	else if(ADC_GroupConfg[ADC_GroupIdx].ADC_SSId == 0 && ADC_NoSamples > 8)
	{
		Function_ValidationCheck = Invalid_SS0SampleNumbers;
	}
	else if( ADC_GroupConfg[ADC_GroupIdx].ADC_SSId == 1 && ADC_NoSamples > 4)
	{
		Function_ValidationCheck = Invalid_SS1SampleNumbers;
	}
	else if (ADC_GroupConfg[ADC_GroupIdx].ADC_SSId == 2 && ADC_NoSamples > 4)
	{
		Function_ValidationCheck = Invalid_SS2SampleNumbers;
	}
	else if(	ADC_GroupConfg[ADC_GroupIdx].ADC_SSId == 3 && ADC_NoSamples > 1)
	{
		Function_ValidationCheck = Invalid_SS3SampleNumbers;
	}
	else 
	{
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCACTSS)
			&= ~(ADC_GroupConfg[ADC_GroupIdx].ADC_SSId);
			for(Loop_Idx = 0 ;Loop_Idx < ADC_NoSamples; Loop_Idx++)
			{
				REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADC_SSCTLn[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId])
				&= ~(Control_BitFields[Loop_Idx]);
				REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADC_SSCTLn[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId])
				|= ((Control_BitFields[Loop_Idx] << SSn_BITShift[Loop_Idx]) & va_arg( enumArgumentPointer, ADC_SampleSeqControlBitsCombination));
				
			}
			va_end( enumArgumentPointer );
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCACTSS)
			|= (ADC_GroupConfg[ADC_GroupIdx].ADC_SSId);
			Function_ValidationCheck = ADC_OK;
	}
	return Function_ValidationCheck;
}
	
/**************************************************************************************************************************
 * Clear ADIF Bit (in Polling)
 **************************************************************************************************************************/
ADC_FunctionReturn ADC_SampleAcknowledge(uint8_t ADC_GroupIdx)
{
		ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
		if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
		{
			Function_ValidationCheck = ADC_NOK;
		}
		else 
		{
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCISC)|=
			(1 << ADC_ISCBitFields[ADC_GroupConfg[ADC_GroupIdx].ADC_SSId]);
		}
		return Function_ValidationCheck;
	}

/*********************************************************************************************************************
 * Initiate sequence Conversion by setting the SSn bit in ADCPSSI register
	********************************************************************************************************************/
ADC_FunctionReturn ADC_ProcessorInitiateSampling(uint8_t ADC_GroupIdx)
{
		ADC_FunctionReturn Function_ValidationCheck = ADC_OK;
		if(ADC_GroupIdx < GROUP0 || ADC_GroupIdx >= ADCNUMBEROFGROUPS)
		{
			Function_ValidationCheck = ADC_NOK;
		}
		else 
		{
			REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCPSSI)|=
			(1 << ADC_GroupConfg[ADC_GroupIdx].ADC_SSId);
		}
		return Function_ValidationCheck;
}







 ADC_FunctionReturn ADC_EnableInterrupt(uint8_t ADC_GroupIdx, uint8_t PRI)
{
	ADC_FunctionReturn Function_ValidationCheck= ADC_OK;

	if(ADC_GroupIdx > ADCNUMBEROFGROUPS)
			Function_ValidationCheck = Invalid_GroupNumber;
	else{
	REGISTER(ADC_ModuleBase[ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId],ADCIM) |= 
	(1 << ADC_GroupConfg[ADC_GroupIdx].ADC_SSId);
	
	Interrupt_EnableSetPri(
	ADC_InterruptNum[4*ADC_GroupConfg[ADC_GroupIdx].ADC_ModuleId+ADC_GroupConfg[ADC_GroupIdx].ADC_SSId], PRI, 1);
	}
	return Function_ValidationCheck;
}