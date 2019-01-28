#ifndef M4_MEM_MAP_H
#define M4_MEM_MAP_H

#include <stdint.h>
typedef volatile uint32_t * const M4_PrefRegType;
#define M4_PREF_BASE_ADD 0xE000E000

/*System Timer (SysTick) Registers*/

#define STCTRL_REG     *((M4_PrefRegType)(M4_PREF_BASE_ADD + 0x010))
#define STRELOAD_REG   *((M4_PrefRegType)(M4_PREF_BASE_ADD + 0x014)) 
#define STCURRENT_REG  *((M4_PrefRegType)(M4_PREF_BASE_ADD + 0x018))

/***********************************************************************************/
/*Nested Vectored Interrupt Controller (NVIC) Registers*/
#define EN_REG_BASE 0x100
#define EN_REG_NUM(INT_NUM) (INT_NUM/32)
#define EN_REG(INT_NUM) *((M4_PrefRegType)((EN_REG_NUM(INT_NUM) * 4) + EN_REG_BASE + M4_PREF_BASE_ADD))
#define EN_INT(INT_NUM)  EN_REG(INT_NUM) |= (1 << ((INT_NUM) % 32))

#define DIS_REG_BASE 0x180
#define DIS_REG_NUM(INT_NUM) (INT_NUM/32)
#define DIS_REG(INT_NUM) *((M4_PrefRegType)((DIS_REG_NUM(INT_NUM) * 4) + DIS_REG_BASE + M4_PREF_BASE_ADD))
#define DIS_INT(INT_NUM)  DIS_REG(INT_NUM) |= (1 << ((INT_NUM) % 32))

#define PEND_REG_BASE 0x200
#define PEND_REG_NUM(INT_NUM) (INT_NUM/32)
#define PEND_REG(INT_NUM) *((M4_PrefRegType)((PEND_REG_NUM(INT_NUM) * 4) + PEND_REG_BASE + M4_PREF_BASE_ADD))
#define PEND_INT(INT_NUM)  PEND_REG(INT_NUM) |= (1 << ((INT_NUM) % 32))
              
#define UNPEND_REG_BASE 0x280
#define UNPEND_REG_NUM(INT_NUM) (INT_NUM/32)
#define UNPEND_REG(INT_NUM) *((M4_PrefRegType)((UNPEND_REG_NUM(INT_NUM) * 4) + UNPEND_REG_BASE + M4_PREF_BASE_ADD))
#define UNPEND_INT(INT_NUM)  UNPEND_REG(INT_NUM) |= (1 << ((INT_NUM) % 32))

#define ACTIVE_REG_BASE 0x300
#define ACTIVE_REG_NUM(INT_NUM) (INT_NUM/32)
#define ACTIVE_REG(INT_NUM) *((M4_PrefRegType)((ACTIVE_REG_NUM(INT_NUM) * 4) + ACTIVE_REG_BASE + M4_PREF_BASE_ADD))
#define IS_INT_ACTIVE(INT_NUM)  (ACTIVE_REG(INT_NUM) & (1 << ((INT_NUM) % 32)) != 0x00)? 1 : 0

#define PRI_REG_BASE 0x400
#define PRI_REG_NUM(INT_NUM) (INT_NUM/4) //63-29=34 (tarteeb el registers in data sheet)
#define PRI_REG(INT_NUM) *((M4_PrefRegType)((PRI_REG_NUM(INT_NUM) * 4) + PRI_REG_BASE + M4_PREF_BASE_ADD))//addition of any number divided by 4
#define SET_INT_PRI(INT_NUM,PRI_NUM) PRI_REG(INT_NUM) &= ~(0x0F <<  (8*(INT_NUM%4)+5) );\
                                     PRI_REG(INT_NUM) |= ((PRI_NUM & 0X0F)<< (8*(INT_NUM%4)+5))

#define SWTRIG_REG   *((M4_PrefRegType)(M4_PREF_BASE_ADD + 0xF00))
/***********************************************************************************/
/*Clock gating Registers*/
#define SYS_CTRL_BASE_ADDRESS 0x400FE000
#define RCGCWD_REG       *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x600))
#define RCGCTIMER_REG    *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x604))
#define RCGCGPIO_REG     *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x608))
#define RCGCDMA_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x60C))
#define RCGCHIB_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x614))
#define RCGCUART_REG     *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x618))
#define RCGCSSI_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x61C))
#define RCGCI2C_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x620))
#define RCGCUSB_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x628))
#define RCGCCAN_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x634))
#define RCGCADC_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x638))
#define RCGCACMP_REG     *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x63C))
#define RCGCPWM_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x640))
#define RCGCQEI_REG      *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x644))
#define RCGCEEPROM_REG   *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x658))
#define RCGCWTIMER_REG   *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x65C))
#define RCGC0_REG		 *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x100))
#define RCGC1_REG		 *((M4_PrefRegType)(SYS_CTRL_BASE_ADDRESS + 0x200))
/***********************************************************************************/

#endif
