/*
 * tec.c
 *
 *  Created on: 20 feb. 2017
 *      Author: Diego
 */


#include "tec.h"
#include "chip.h"
#include "cmsis.h"

#define ENTRADA          0
#define SALIDA           1
#define BAJO             0
#define ALTO             1

//PUERTOS Y PINES DEL TECLADO
//SE DEFINE FILAS COMO ENTRADAS
//FILA1
#define SCU_PORT_F1IN    3
#define SCU_PIN__F1IN    1
#define GPIO_PORT_F1IN   5
#define GPIO_PIN_F1IN    8

//FILA2
#define SCU_PORT_F2IN    3
#define SCU_PIN__F2IN    2
#define GPIO_PORT_F2IN   5
#define GPIO_PIN_F2IN    9

//FILA3
#define SCU_PORT_F3IN    7
#define SCU_PIN__F3IN    4
#define GPIO_PORT_F3IN   3
#define GPIO_PIN_F3IN    12

//FILA4
#define SCU_PORT_F4IN    4
#define SCU_PIN__F4IN    0
#define GPIO_PORT_F4IN   2
#define GPIO_PIN_F4IN    0

//SE DEFINE COLUMNAS COMO SALIDA
//COLUMNA1
#define SCU_PORT_C1OUT     4
#define SCU_PIN__C1OUT     3
#define GPIO_PORT_C1OUT    2
#define GPIO_PIN_C1OUT     3

//COLUMNA2
#define SCU_PORT_C2OUT     4
#define SCU_PIN__C2OUT     2
#define GPIO_PORT_C2OUT    2
#define GPIO_PIN_C2OUT     2

//COLUMNA3
#define SCU_PORT_C3OUT     1
#define SCU_PIN__C3OUT     5
#define GPIO_PORT_C3OUT    1
#define GPIO_PIN_C3OUT     8


static uint8_t KeyChanged;
static uint8_t teclaval;

void KEYBOARD_init(void)
{
//INICIALIZACION DE LOS PINES PARA ENTRADA
//FILA1
Chip_SCU_PinMux ( SCU_PORT_F1IN , SCU_PIN__F1IN , MD_PUP|MD_EZI, FUNC4);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_F1IN  , GPIO_PIN_F1IN, ENTRADA);
//FILA2
Chip_SCU_PinMux ( SCU_PORT_F2IN, SCU_PIN__F2IN, MD_PUP|MD_EZI, FUNC4);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_F2IN, GPIO_PIN_F2IN, ENTRADA);
//FILA3
Chip_SCU_PinMux ( SCU_PORT_F3IN, SCU_PIN__F3IN, MD_PUP|MD_EZI, FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_F3IN, GPIO_PIN_F3IN, ENTRADA);
//FILA4
Chip_SCU_PinMux ( SCU_PORT_F4IN, SCU_PIN__F4IN, MD_PUP|MD_EZI, FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_F4IN, GPIO_PIN_F4IN, ENTRADA);

//INICIALIZACION DE LOS PINES PARA SALIDA
//COLUMNA1
Chip_SCU_PinMux ( SCU_PORT_C1OUT , SCU_PIN__C1OUT , SCU_MODE_INACT , FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_C1OUT  , GPIO_PIN_C1OUT, SALIDA);
//COLUMNA2
Chip_SCU_PinMux ( SCU_PORT_C2OUT , SCU_PIN__C2OUT , SCU_MODE_INACT , FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_C2OUT  , GPIO_PIN_C2OUT, SALIDA);
//COLUMNA3
Chip_SCU_PinMux ( SCU_PORT_C3OUT , SCU_PIN__C3OUT , SCU_MODE_INACT , FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_C3OUT  , GPIO_PIN_C3OUT, SALIDA);
}


static uint8_t barrerTeclado(unsigned char *presionado){
  unsigned char i;
  bool F1,F2,F3,F4;
  for(i=0; i<3; i++){  //Coloca p/c iteraci�n un GPIO en cero y el resto en 1
    if(i==0){
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C1OUT, GPIO_PIN_C1OUT, BAJO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C2OUT, GPIO_PIN_C2OUT, ALTO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C3OUT, GPIO_PIN_C3OUT, ALTO);
     //Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_F4OUT, GPIO_PIN_F4OUT, ALTO);
    }
    else if(i==1){
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C1OUT, GPIO_PIN_C1OUT, ALTO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C2OUT, GPIO_PIN_C2OUT, BAJO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C3OUT, GPIO_PIN_C3OUT, ALTO);
     //Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_F4OUT, GPIO_PIN_F4OUT, ALTO);
    }
    else if(i==2){
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C1OUT, GPIO_PIN_C1OUT, ALTO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C2OUT, GPIO_PIN_C2OUT, ALTO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C3OUT, GPIO_PIN_C3OUT, BAJO);
     //Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_F4OUT, GPIO_PIN_F4OUT, ALTO);
    }
    else{
    Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C1OUT, GPIO_PIN_C1OUT, ALTO);
    Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C2OUT, GPIO_PIN_C2OUT, ALTO);
    Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C3OUT, GPIO_PIN_C3OUT, ALTO);
    //Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_F4OUT, GPIO_PIN_F4OUT, BAJO);
   }

   F1=Chip_GPIO_ReadPortBit  ( LPC_GPIO_PORT,GPIO_PORT_F1IN,GPIO_PIN_F1IN);
   F2=Chip_GPIO_ReadPortBit  ( LPC_GPIO_PORT,GPIO_PORT_F2IN,GPIO_PIN_F2IN);
   F3=Chip_GPIO_ReadPortBit  ( LPC_GPIO_PORT,GPIO_PORT_F3IN,GPIO_PIN_F3IN);
   F4=Chip_GPIO_ReadPortBit  ( LPC_GPIO_PORT,GPIO_PORT_F4IN,GPIO_PIN_F4IN);


  if( F1==BAJO || F2==BAJO || F3==BAJO || F4==BAJO){ //si apreto una tecla
      //identifico la tecla apretada
     if(i==0){ //Si la tecla apretada pertenece a la fila 1
        if(F1 == BAJO)
          *presionado='1';
        else if(F2==BAJO)
          *presionado='4';
         else if(F3==BAJO)
          *presionado='7';
         else if(F4==BAJO)
        	 *presionado='*';
        return 1;
      }
      else if(i==1){ //Si la tecla apretada pertenece a la fila 2
        if(F1==BAJO)
          *presionado='2';
        else if(F2==BAJO)
          *presionado='5';
        else if(F3==BAJO)
          *presionado='8';
        else if(F4==BAJO)
          *presionado='0';

        return 1;
      }
      else if(i==2){ //Si la tecla apretada pertenece a la fila 3
        if(F1==BAJO)
          *presionado='3';
        else if(F2==BAJO)
          *presionado='6';
        else if(F3==BAJO)
          *presionado='9';
        else if(F4==BAJO)
          *presionado='#';
        return 1;
      }
//      else if(i==3){ //Si la tecla apretada pertenece a la fila 4
//        if(F1==LOW)
//          *key='*'; //14=*
//        else if(F2==LOW)
//          *key='0';
//        else if(F3==BAJO)
//          *key='#';
//        return 1;
//      }
    }
  }
  return 0;

}

void KEYBOARD_rebote(void){  //funcion de rebote

  static uint8_t estado=0, estado_ant=0, tec, solto=0;
  unsigned char presionado;

  estado=barrerTeclado(&presionado);

  if(estado==0 && estado_ant==1) //Se determina si se solto la tecla
    solto=1; //si solt� la tecla
  else
    solto=0; // sigue apretando la tecla

  estado_ant=estado;

  if(solto==1){  //Si se solt� se coloca la tecla como v�lida
    KeyChanged=1;//flag que avisa que cambio la tecla
    teclaval=tec; //coloca en el buffer la tecla que corresponde
  }
  tec=presionado;//se almacena el valor nuevo
}

uint8_t KEYBOARD_devolvertecla(unsigned char *presionado){

  if(!KeyChanged)
    return 0;
  else{
    KeyChanged=0;
    *presionado=teclaval;
    return 1;
  } //se devuelve la tecla si no fue tomada antes
//evita que se tome una tecla 2 veces
}


