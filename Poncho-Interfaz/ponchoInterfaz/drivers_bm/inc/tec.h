/*
 * tec.h
 *
 *  Created on: 20 feb. 2017
 *      Author: Diego
 */

#ifndef PROJECTS_DRIVERS_BM_INC_TEC_H_
#define PROJECTS_DRIVERS_BM_INC_TEC_H_


#include "stdint.h"

static uint8_t barrerTeclado(unsigned char *);
void KEYBOARD_rebote(void); //Funci�n que realiza un barrido del teclado con el objetivo de comprobar si se ha presionado alguna tecla
uint8_t KEYBOARD_devolvertecla(unsigned char *key);//Funci�n que devuelve un 0 si ninguna tecla ha sido presionada, si devuelve un uno una tecla fue presionada y la misma se encuentra alamcenada en la variable pasada como parametro


#endif /* PROJECTS_DRIVERS_BM_INC_TEC_H_ */
