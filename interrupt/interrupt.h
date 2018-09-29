#ifndef PRIORITY_H
#define PRIORITY_H
#include <stdint.h>

// Base address for interrupt Registers
#define INT_BASEADDRESS 0xE000E000

/*********************************************************************************************************
 * This offsets are for interrupt enable, disable and prioriy
 *********************************************************************************************************/
#define INTEN0    		0x100
#define INTEN1			0x104
#define INTEN2			0x108
#define INTEN3			0x10C
#define INTEN4			0x110
#define INTDIS0			0x180
#define INTDIS1			0x184
#define INTDIS2			0x188
#define INTDIS3			0x18C
#define INTDIS4			0x190
#define INTPRI0			0x400
#define INTPRI1			0x404
#define INTPRI2			0x408
#define INTPRI3			0x40C
#define INTPRI4			0x410
#define INTPRI5			0x414
#define INTPRI6			0x418
#define INTPRI7			0x41C
#define INTPRI8			0x420
#define INTPRI9			0x424
#define INTPRI10		0x428
#define INTPRI11		0x42C
#define INTPRI12		0x430
#define INTPRI13		0x434
#define INTPRI14		0x438
#define INTPRI15		0x43C
#define INTPRI16		0x440
#define INTPRI17		0x444
#define INTPRI18		0x448
#define INTPRI19		0x44C
#define INTPRI20		0x450
#define INTPRI21		0x454
#define INTPRI22		0x458
#define INTPRI23		0x45C
#define INTPRI24		0x460
#define INTPRI25		0x464
#define INTPRI26		0x468
#define INTPRI27		0x46C
#define INTPRI28		0x470
#define INTPRI29		0x474
#define INTPRI30		0x478
#define INTPRI31		0x47C
#define INTPRI32		0x480
#define INTPRI33		0x484
#define INTPRI34		0x488


typedef enum {INT_OK=0, INT_NOK} Interrupt_ReturnType;

Interrupt_ReturnType Interrupt_EnableSetPri(uint8_t interrupt_number, uint8_t priority, uint8_t int_en);
#endif 

 

