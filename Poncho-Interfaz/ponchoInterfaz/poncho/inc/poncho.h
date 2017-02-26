/*
 *
 *  Created on: 16/1/2017
 *      Author: Diego
 */

#ifndef PONCHO
#define PONCHO

#include "stdint.h"

#include "chip.h"
#include "puertos.h"
//#include "lcd.h"
#include "leds.h"
#include "pulsadores.h"
#include "lpc_types.h"
#include "mef.h"
#include "RS485.h"
#include "tec.h"

#define lpc4337            1
#define mk60fx512vlq15     2

/*==================[typedef]================================================*/
//LPC_GPIO_T pGPIO;
/*==================[external data declaration]==============================*/
#if (CPU == mk60fx512vlq15)
/* Reset_Handler is defined in startup_MK60F15.S_CPP */
void Reset_Handler( void );

extern uint32_t __StackTop;
#elif (CPU == lpc4337)
/** \brief Reset ISR
 **
 ** ResetISR is defined in cr_startup_lpc43xx.c
 **
 ** \remark the definition is in
 **         externals/drivers/cortexM4/lpc43xx/src/cr_startup_lpc43xx.c
 **/
extern void ResetISR(void);

/** \brief Stack Top address
 **
 ** External declaration for the pointer to the stack top from the Linker Script
 **
 ** \remark only a declaration is needed, there is no definition, the address
 **         is set in the linker script:
 **         externals/base/cortexM4/lpc43xx/linker/ciaa_lpc4337.ld.
 **/
extern void _vStackTop(void);



void RIT_IRQHandler(void);


#else
#endif

void uartConfig(uint8_t address);
void uartWriteByte(uint8_t byte);
uint8_t uartReadByte(void);
void transmitir (uint16_t valor, unsigned char tipo);
void trans_fecha(unsigned char hora1, unsigned char hora2, unsigned char min1, unsigned char min2, unsigned char seg1,unsigned char seg2);

#endif /* PONCHO_H_ */
