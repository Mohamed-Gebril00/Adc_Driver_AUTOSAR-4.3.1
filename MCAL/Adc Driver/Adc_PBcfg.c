#include "Adc.h"
#define NUMBER_OF_HW_UNITS       (8u)
void test(void);
/* configuration for Adc_channel Group */
Adc_ChannelGroupConfigType Adc_ChannelGroupConfig[NUMBER_OF_CHANNEL_GROUPS]=
{
	{
		0,							/* Group_Id */
		ADC_ACCESS_MODE_SINGLE, 	/* Group_Access_Mode */
		ADC_CONV_MODE_ONESHOT,		/* Group_Conv_Mode	*/
		/* preprocessor remove unconfigured variable in preprossing time */
	  /* removed not configured by sw */
		0,
		ADC_TRIGG_SRC_SW,			/* Group Event Trigger */
		EXTERNAL,
		ADC_HW_TRIG_RISING_EDGE,
		
		
		ADC_STREAM_BUFFER_CIRCULAR,	/* Software buffer mode */
		1,	
		
		test,
		
		ADC_GROUP_REPL_ABORT_RESTART,
		
		{
			ADC_CHANNEL_1
		}
	}
};
/* ***************************************************************** */

/* Configuration for hw uits 
	Two modules each with four sequencers 
	then eight Hw units 
	NUMBER_OF_HW_UNITS defined in Adc_Cfg.h
	*/
Adc_HwUnitConfigType Adc_HwUnitConfig[NUMBER_OF_HW_UNITS]=
{
	{
	 0, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_0,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_0, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 8 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 1, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_0,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_1, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 4 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 2, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_0,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_2, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 4 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 3, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_0,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_3, /* Numberic Id of the Adc_Sequencer */
	 TRUE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 1 */
	 1,			
	 0	/* /id to the Adc_ChannelGroup*/
	},
	{
	 4, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_1,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_0, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 8 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 5, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_1,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_1, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 4 */
	 0,			
	 255	/* Pointer to the Adc_ChannelGroup*/
	},
	{
	 6, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_1,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_2, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 4 */
	 0,			
	 255 /* Pointer to the Adc_ChannelGroup*/
	},
	{
	 7, /* Numeric ID of the HW Unit. */
	 ADC_MODULE_1,	/*  Numeric Id of the Adc_Module */
	 ADC_SEQUENCER_3, /* Numberic Id of the Adc_Sequencer */
	 FALSE, /* True: use Adc_Sequencer false: don't configure */
	 /* Hold the number of input channels per hw unit*/
	 /* maximum for this hw unit is 1 */
	 1,			
	 255	/* Id of Adc_ChannelGroup*/
	},
};



/* ***************************************************************** */
/* The Overall configuration to be passed to Adc_init Function */
const Adc_ConfigType Adc_Config=
{
	TRUE, /* True: Configure and enable clock for module 1, False: Don't configure */
	ADC_500_K_SAMPLING_SPEED,	/* 500 kilo sample per second. */
	TRUE, /* True: Configure and enable clock for module 1, False: Don't configure */
	ADC_500_K_SAMPLING_SPEED,
	&Adc_HwUnitConfig[0]
};