#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H
/*
 * CPU register type width
 */
#define CPU_TYPE_8	  			(8u)
#define CPU_TYPE_16   			(16u)
#define CPU_TYPE_32	  			(32u)


/*
 * Bit order definition
 */
#define MSB_FIRST				(0u)
#define LSB_FIRST				(1u)


/*
 * Byte order definition
 */
#define HIGH_BYTE_FIRST			(0u)
#define LOW_BYTE_FIRST			(1u)


/*
 * platform type and endiness definitions, specific for tm4c123gh6pm (Not Sure)
 */
#define CPU_TYPE 				CPU_TYPE_32

#define CPU_BIT_ORDER			LSB_FIRST
#define CPU_BYTE_ORDER			LOW_BYTE_FIRST

#define FALSE                   0u
#define TRUE					1u


/*
 * AUTOSAR integer data types
 */
typedef signed char  	   	 sint8;
typedef unsigned char 	   	uint8;
typedef signed short       	sint16;
typedef unsigned short     	uint16;
typedef signed long        	sint32;
typedef unsigned long 	   	uint32;
typedef unsigned long long 	uint64;
typedef float 				float32;
typedef double 				float64;

typedef unsigned int 		uint8_least;
typedef unsigned int		uint16_least;
typedef unsigned long 		uint32_least;
typedef signed int 			sint8_least;
typedef signed int 			sint16_least;
typedef signed int 			sint32_least;

typedef unsigned int 		boolean;
#endif