#include "interrupt.h"

#define REGISTER(base, offset) (*(volatile uint32_t*)(base+offset))

#define SETPRI(PRIREG,priority,bits) 	PRIREG |= (priority & 1 ) << (8*bits+5);\
										PRIREG |= ((priority >> 1) & 1 ) << (8*bits+6);\
										PRIREG |= ((priority >> 2) & 1 ) << (8*bits+7)

uint32_t Pri_Offset[]={INTPRI0, INTPRI1, INTPRI2, INTPRI3, INTPRI4, INTPRI5, INTPRI6, INTPRI7, INTPRI8,
						INTPRI9, INTPRI10, INTPRI11, INTPRI12, INTPRI13, INTPRI14, INTPRI15, INTPRI16, INTPRI17,
						INTPRI18, INTPRI19, INTPRI20, INTPRI21, INTPRI22, INTPRI23, INTPRI24, INTPRI25, INTPRI26,
						INTPRI27, INTPRI28, INTPRI29, INTPRI30, INTPRI31, INTPRI32, INTPRI33, INTPRI34};
						
uint32_t En_Offset[]={INTEN0, INTEN1, INTEN2, INTEN3};

uint32_t Dis_Offset[]={INTDIS0, INTDIS1, INTDIS2, INTDIS3};



Interrupt_ReturnType Interrupt_EnableSetPri(uint8_t interrupt_number, uint8_t priority, uint8_t int_en)
{
	Interrupt_ReturnType Function_ValidationCheck= INT_OK;
	uint8_t Int_EnIdx=0, Int_PriIdx=0, Int_EnBit=0, Int_PriBit=0;
	
	if(interrupt_number > 138)
	{
		Function_ValidationCheck = INT_NOK;
	}
	else if(priority > 7)
	{
		Function_ValidationCheck = INT_NOK;
	}
	else 
	{
		
		Int_EnIdx = interrupt_number / 32;
		Int_EnBit= interrupt_number % 32;
		
		// disable interrupt
		if(int_en == 0)
		{
			REGISTER(INT_BASEADDRESS, Dis_Offset[Int_EnIdx]) |= (1 << Int_EnBit);
		}
		// enable interrupt
		else 
		{
		Int_PriIdx = interrupt_number / 4;
		Int_PriBit = interrupt_number % 4;
		REGISTER(INT_BASEADDRESS, En_Offset[Int_EnIdx]) |= (1 << Int_EnBit);
		SETPRI(REGISTER(INT_BASEADDRESS, Pri_Offset[Int_PriIdx]),priority,Int_PriBit);
		check = REGISTER(INT_BASEADDRESS, Pri_Offset[Int_PriIdx]);
		}
	}
	return Function_ValidationCheck;
}
