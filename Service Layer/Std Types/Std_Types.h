#ifndef STD_TYPES_H
#define STD_TYPES_H


 /* Id for the company in the AUTOSAR
  * Gebro-u-motive's company ID = 0x0076 :) 
  * Gebro-u-motive is a fictional company owned by
	* Mohamed Ahmed Gebril: vendor of the module*/
#define STD_TYPES_VENDOR_ID                                  (0x0076U)

/*
 * Module Version 1.0.0
 */
#define STD_TYPES_SW_MAJOR_VERSION                           (1U)
#define STD_TYPES_SW_MINOR_VERSION                           (0U)
#define STD_TYPES_SW_PATCH_VERSION                           (0U)

/*
 * AUTOSAR Version 4.3.1
 */
#define STD_TYPES_AR_RELEASE_MAJOR_VERSION                   (4U)
#define STD_TYPES_AR_RELEASE_MINOR_VERSION                   (3U)
#define STD_TYPES_AR_RELEASE_PATCH_VERSION                   (1U)


#include "Compiler.h"
#include "Platform_Types.h"

/*
 * Describes the standard return Type Definitions used in the project 
 */
typedef uint8 Std_ReturnType;

/*
  * structure for the version of the module.
  * This is requested by calling <module_name>_GetVersionIfo()
  */
 
typedef struct
{
	uint16 vendorID;
	uint16 moduleID;
	uint8  instanceID;
	uint8  sw_major_version;
	uint8  sw_minot_version;
	uint8  sw_patch_version;
}Std_VersionInfoType;

#define STD_HIGH	0x01u		/* Standard HIGH */
#define STD_LOW		0x00u		/* Stanard  low */

#define STD_ACTIVE	0x01u		/* Logical State Active*/
#define STD_IDLE	0x00u		/* Logical State Idle*/

#define STD_ON		0x01u		/* Standard On*/
#define STD_OFF		0x00u		/* Standard Off*/

#define E_OK		((Std_ReturnType)0x00u)		/* Function Return Ok */
#define E_NOT_OK	((Std_ReturnType)0x01u)		/* Function Return NOT Ok */


#endif
