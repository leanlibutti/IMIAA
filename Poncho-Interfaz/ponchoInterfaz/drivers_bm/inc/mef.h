/*
 * mef.h
 *
 *  Created on: 27/1/2017
 *      Author: Diego
 */

#ifndef MEF_H_
#define MEF_H_

#include "lpc_types.h"

#include "stdint.h"
#include "chip.h"
#include "puertos.h"
#include "lcd.h"



uint16_t convertirAscii_Num(unsigned char s1, unsigned char s2, unsigned char s3, unsigned char s4, unsigned char s5);
uint16_t convertirAscii_Num2(unsigned char s1, unsigned char s2, unsigned char s3);
void Init_MEF(void);
void UpdateMEF(unsigned char tecla,uint16_t data1,uint16_t data2,uint16_t data3, uint16_t data4);

#endif /* MEF_H_ */
