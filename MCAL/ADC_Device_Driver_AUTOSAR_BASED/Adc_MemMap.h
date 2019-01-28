
/**
  * ADC Base addresses, offsets and important bit in registers
  * Author: Mohamed Ahmed Gebril 
  * (Gebro)
  * Start Date: 16/9/2018
  * Specified for graduation project 2018 (Faculty of engineering Alexandria university
  */
#ifndef ADC_MEMMAP_H
#define ADC_MEMMAP_H


/**
  * ADC Module bases Page 815 TM4C123GH6PM
  */
#define ADC0BASEADDRESS 0x40038000
#define ADC1BASEADDRESS	0x40039000




/*********************************************************************************************************************/
/**
  * ADC offsets Page 815, 816
  */
#define ADCACTSS 					0x000						// ADC Active Sample Sequencer
#define ADCRIS   					0x004						// ADC Raw Interrupt Status
#define ADCIM 						0x008						// ADC Interrupt Mask
#define ADCISC						0x00C						// ADC Interrupt Status And Clear
#define ADCOSTAT					0x010						// ADC Overflow Status
#define ADCEMUX						0x014						// ADC Event Multiplexer Select
#define ADCUSTAT					0x018						// ADC Underflow status
#define ADCTSSEL					0x01C						// ADC Trigger Source Select
#define ADCSSPRI					0x020						// ADC Sample Sequencer Priority
#define ADCSPC						0x024						// ADC Sample Phase Control
#define ADCPSSI						0x028						// ADC Processor Sample Sequence Initiate
#define ADCSAC						0x030						// ADC Sample Averaging Control
#define ADCDCISC					0x034						// ADC digital Comparator Interrupt Status Clear
#define ADCCTL						0x038						// ADC Control
#define ADCSSMUX0					0x040						// ADC Sample Sequence Input Multiplexer Select 0
#define ADCSSCTL0					0x044						// ADC Sequence Control 0
#define ADCSSFIFO0				    0x048						// ADC Sample Sequence Result FIFO 0
#define ADCSSFSTAT0				    0x04C						// ADC Sample Sequence FIFO 0 status
#define ADCSSOP0					0x050						// ADC Sample Sequence 0 Operation 
#define ADCSSDC0					0x054						// ADC Sample sequence 0 Digital Comparator select
#define ADCSSMUX1 				    0x060						// ADC Sample Sequence Input Multiplexer Select 1
#define ADCSSCTL1					0x064						// ADC Sample Sequence Control one 
#define ADCSSFIFO1				    0x068						// ADC Sample Sequence Result FIFO 1
#define ADCSSFSTAT1				    0x06C						// ADC Sample sequence 1 statue
#define ADCSSOP1					0x070						// ADC Sample Sequence Operation 1
#define ADCSSDC1					0x074						// ADC Sample Sequence 1 digital Comparator
#define ADCSSMUX2					0x080						// ADC Sample Sequence input multiplexer select 2
#define ADCSSCTL2 				    0x084						// ADC Sample Sequence Control 2
#define ADCSSFIFO2				    0x088						// ADC Sample Sequence result FIFO 2	
#define ADCSSFSTAT2			     	0x08C						// ADC Sample Sequence FIFO 2 Status
#define ADCSSOP2					0x090						// ADC Sample Sequence 2 Operation
#define ADCSSDC2 					0x094						// ADC Sample Sequence 2 Digital Comparator select 
#define ADCSSMUX3					0x0A0						// ADC Sample Sequence input multiplexer select 3
#define ADCSSCTL3 				    0x0A4						// ADC Sample Sequence Control 3
#define ADCSSFIFO3				    0x0A8						// ADC Sample Sequence result FIFO 3	
#define ADCSSFSTAT3				    0x0AC						// ADC Sample Sequence FIFO 3 Status
#define ADCSSOP3					0x0B0						// ADC Sample Sequence 3 Operation
#define ADCSSDC3					0x0B4						// ADC Sample Sequence 3 Digital Comparator select 
#define ADCDCRIC					0xD00						// ADC Digital Comparator Reset Initial Condition
#define ADCDCCTL0					0xE00						// ADC Digital Comparator Control 0
#define ADCDCCTL1					0xE04						// ADC Digital Comparator Control 1
#define ADCDCCTL2					0xE08						// ADC Digital Comparator Control 2
#define ADCDCCTL3					0xE0C						// ADC Digital Comparator control 3
#define ADCDCCTL4					0xE10						// ADC Digital Comparator Control 4
#define ADCDCCTL5					0xE14						// ADC Digital Comparator Control 5
#define ADCDCCTL6					0xE18						// ADC Digital Comparator Control 6
#define ADCDCCTL7					0xE1C						// ADC Digital Comparator Control 7
#define ADCDCCMP0					0xE40						// ADC Digital Comparator Range 0
#define ADCDCCMP1					0xE44						// ADC Digital Comparator Range 1
#define ADCDCCMP2					0xE48						// ADC Digital Comparator Range 2
#define ADCDCCMP3					0xE4C						// ADC Digital Comparator Range 3
#define ADCDCCMP4					0xE50						// ADC Digital Comparator Range 4
#define ADCDCCMP5					0xE54						// ADC Digital Comparator Range 5
#define ADCDCCMP6					0xE58						// ADC Digital Comparator Range 6
#define ADCDCCMP7					0xE5C						// ADC Digital Comparator Range 7
#define ADCPP						0xFC0						// ADC Peripheral Properties
#define ADCPC						0xFC4						// ADC Peripheral Configuration
#define ADCCC						0xFC8						// ADC Clock Configuration

#define ADC_MUX0_MODULE0_REG	(ADC0BASEADDRESS + ADCSSMUX0)	
#define ADC_MUX1_MODULE0_REG ((ADC0BASEADDRESS + ADCSSMUX1))
#define ADC_MUX2_MODULE0_REG ((ADC0BASEADDRESS + ADCSSMUX2))
#define ADC_MUX3_MODULE0_REG ((ADC0BASEADDRESS + ADCSSMUX3))
#define ADC_MUX0_MODULE1_REG ((ADC1BASEADDRESS + ADCSSMUX0))
#define ADC_MUX1_MODULE1_REG ((ADC1BASEADDRESS + ADCSSMUX1))
#define ADC_MUX2_MODULE1_REG ((ADC1BASEADDRESS + ADCSSMUX2))
#define ADC_MUX3_MODULE1_REG (*(volatile uint32_t* const) (ADC1BASEADDRESS + ADCSSMUX3))
 

/************************************************************************************************
 * The Following are defined for the bit fields in RCGC0 register (For The ADC modules only.)
	*/
#define ADC0						0x00010000		// ADC0 Clock Gating Control
#define ADC1 						0x00020000		// ADC1 Clock Gating Control
#define MAXADC0SPD 			8							// ADC0 Sample Speed shift
#define MAXADC1SPD 			10						// ADC1 Sample Speed shift

/******************************************************************************
 * The Following are defined for the bit fields in ADCACTSS registers
 ******************************************************************************/
#define BUSY_BITFIELD 	0x00010000
#define ASEN0_BITFIELD	0x00000001								// Sequencer0 enable bit 0 
#define ASEN1_BITFIELD	0x00000002								// Sequencer1 enable bit 1
#define ASEN2_BITFIELD	0x00000004								// Sequencer2 enable bit 2
#define ASEN3_BITFIELD	0x00000008								// Sequencer3 enable bit 3

/******************************************************************************
 * The Following are defined for the bit fields in ADCSSMUXn registers
 ******************************************************************************/
#define MUX0_BITFIELD 0x0000000F
#define MUX1_BITFIELD 0x000000F0
#define MUX2_BITFIELD 0x00000F00
#define MUX3_BITFIELD 0x0000F000
#define MUX4_BITFIELD 0x000F0000
#define MUX5_BITFIELD 0x00F00000
#define MUX6_BITFIELD 0x0F000000
#define MUX7_BITFIELD 0xF0000000


/******************************************************************************
 * The Following are defined for the bit fields in ADCSSCTLn registers
 ******************************************************************************/
#define SAMPLE0_BITFIELD 0x0000000F			
#define SAMPLE1_BITFIELD 0x000000F0
#define SAMPLE2_BITFIELD 0x00000F00
#define SAMPLE3_BITFIELD 0x0000F000
#define SAMPLE4_BITFIELD 0x000F0000
#define SAMPLE5_BITFIELD 0x00F00000
#define SAMPLE6_BITFIELD 0x0F000000
#define SAMPLE7_BITFIELD 0xF0000000

/******************************************************************************
 * The Following are defined for the bit fields in ADCEMUX register (page: 830).
 ******************************************************************************/
#define EM0_BITFIELD				0x0000000F									// SS3 trigger select.
#define EM1_BITFIELD				0x000000F0									// SS2 trigger select.
#define EM2_BITFIELD 				0x00000F00									// SS1 trigger select.
#define EM3_BITFIELD 				0x0000F000									// SS0 trigger select.
#define EM0_BITNUM					0											// Starting bit of event multiplexer SS0.
#define EM1_BITNUM					4											// Starting bit of event multiplexer SS1.
#define EM2_BITNUM   				8											// Starting bit of event multiplexer SS2.
#define EM3_BITNUM  				12											// Starting bit of event multiplexer SS3.
/******************************************************************************
 * The Following are defined for the bit fields in ADCISC register (page: 825).
 ******************************************************************************/
#define IN0 			0x00000001			// SS0 Interrupt status and clear
#define IN1 			0x00000002			// SS1 Interrupt status and clear
#define IN2 			0x00000004			// SS2 Interrupt status and clear
#define IN3 			0x00000008			// SS3 Interrupt status and clear
#define DCINSS0 		0x00010000			// Digital comparator interrupt status on SS0
#define DCINSS1 		0x00020000			// Digital comparator interrupt status on SS1
#define DCINSS2 		0x00040000			// Digital comparator interrupt status on SS2
#define DCINSS3 		0x00080000			// Digital comparator interrupt status on SS3

/********************************* ADCPSSI **************************************************************/
#define SS0_BITNUM 	    0
#define SS1_BITNUM 		1
#define SS2_BITNUM  	2
#define SS3_BITNUM 		3


/***************************************************************************************************
 * ADC Interrupt mask bits
 ***************************************************************************************************/
#define MASK0_BITNUM			0
#define MASK1_BITNUM 			1
#define MASK2_BITNUM			2	
#define MASK3_BITNUM			3

/**************************************************************************************************
* ADC IRQ number.
***************************************************************************************************/
#define ADC0_SS0IRQ	14
#define ADC0_SS1IRQ	15
#define ADC0_SS2IRQ	16
#define ADC0_SS3IRQ	17
#define	ADC1_SS0IRQ	48
#define ADC1_SS1IRQ	49
#define ADC1_SS2IRQ	50	
#define ADC1_SS3IRQ	51

	
#endif