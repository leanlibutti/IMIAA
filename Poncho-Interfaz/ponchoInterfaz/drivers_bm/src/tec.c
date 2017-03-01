
#include "tec.h"
#include "chip.h"
#include "cmsis.h"

#define ENTRADA          0
#define SALIDA           1
#define BAJO             0
#define ALTO             1

//PUERTOS Y PINES DEL TECLADO
//Se utiliza las filas como entradas
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

<<<<<<< HEAD
//Se utiliza las columnas como salidas
=======
//SE DEFINE COLUMNAS COMO SALIDA
>>>>>>> aa8d858a4f70f1f53b0870337e61795a563caba8
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

//inicializacion de los pines del teclado
void KEYBOARD_init(void)
{
//inicializa las filas como entras y con pull-up enable
Chip_SCU_PinMux ( SCU_PORT_F1IN , SCU_PIN__F1IN , MD_PUP|MD_EZI, FUNC4);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_F1IN  , GPIO_PIN_F1IN, ENTRADA);

Chip_SCU_PinMux ( SCU_PORT_F2IN, SCU_PIN__F2IN, MD_PUP|MD_EZI, FUNC4);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_F2IN, GPIO_PIN_F2IN, ENTRADA);

Chip_SCU_PinMux ( SCU_PORT_F3IN, SCU_PIN__F3IN, MD_PUP|MD_EZI, FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_F3IN, GPIO_PIN_F3IN, ENTRADA);

Chip_SCU_PinMux ( SCU_PORT_F4IN, SCU_PIN__F4IN, MD_PUP|MD_EZI, FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_F4IN, GPIO_PIN_F4IN, ENTRADA);

//inicializa las columnas como salidas 

Chip_SCU_PinMux ( SCU_PORT_C1OUT , SCU_PIN__C1OUT , SCU_MODE_INACT , FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_C1OUT  , GPIO_PIN_C1OUT, SALIDA);

Chip_SCU_PinMux ( SCU_PORT_C2OUT , SCU_PIN__C2OUT , SCU_MODE_INACT , FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_C2OUT  , GPIO_PIN_C2OUT, SALIDA);

Chip_SCU_PinMux ( SCU_PORT_C3OUT , SCU_PIN__C3OUT , SCU_MODE_INACT , FUNC0);
Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, GPIO_PORT_C3OUT  , GPIO_PIN_C3OUT, SALIDA);
}

//realiza un barrido del teclado matricial para detectar si se encuentra una tecla presionada y la devuelve
static uint8_t barrerTeclado(unsigned char *presionado){
  unsigned char i;
  bool F1,F2,F3,F4;
  for(i=0; i<3; i++){  //en cada iteracion se pone una columna en estada bajo, la cual sera la analizada y las otras en alto
    if(i==0){          
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C1OUT, GPIO_PIN_C1OUT, BAJO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C2OUT, GPIO_PIN_C2OUT, ALTO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C3OUT, GPIO_PIN_C3OUT, ALTO);
    }
    else if(i==1){
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C1OUT, GPIO_PIN_C1OUT, ALTO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C2OUT, GPIO_PIN_C2OUT, BAJO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C3OUT, GPIO_PIN_C3OUT, ALTO);
    }
    else if(i==2){
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C1OUT, GPIO_PIN_C1OUT, ALTO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C2OUT, GPIO_PIN_C2OUT, ALTO);
     Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C3OUT, GPIO_PIN_C3OUT, BAJO);
    }
    else{
    Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C1OUT, GPIO_PIN_C1OUT, ALTO);
    Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C2OUT, GPIO_PIN_C2OUT, ALTO);
    Chip_GPIO_SetPinState( LPC_GPIO_PORT, GPIO_PORT_C3OUT, GPIO_PIN_C3OUT, ALTO);
   }

   F1=Chip_GPIO_ReadPortBit  ( LPC_GPIO_PORT,GPIO_PORT_F1IN,GPIO_PIN_F1IN);
   F2=Chip_GPIO_ReadPortBit  ( LPC_GPIO_PORT,GPIO_PORT_F2IN,GPIO_PIN_F2IN);
   F3=Chip_GPIO_ReadPortBit  ( LPC_GPIO_PORT,GPIO_PORT_F3IN,GPIO_PIN_F3IN);
   F4=Chip_GPIO_ReadPortBit  ( LPC_GPIO_PORT,GPIO_PORT_F4IN,GPIO_PIN_F4IN);


  if( F1==BAJO || F2==BAJO || F3==BAJO || F4==BAJO){ //pregunta si hay tecla presionada en alguna de las filas de la columna analizada
      //identifica la tecla presionada
     if(i==0){ 
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
      else if(i==1){ 
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
      else if(i==2){ 
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
    }
  }
  return 0;

}

//control del rebote del teclado
void KEYBOARD_rebote(void){  

  static uint8_t estado=0, estado_ant=0, tec, solto=0;
  unsigned char presionado;

  estado=barrerTeclado(&presionado);

  if(estado==0 && estado_ant==1) 
    solto=1; //si suelta la tecla
  else
    solto=0; 

  estado_ant=estado;

  if(solto==1){  
    KeyChanged=1;//si se suelta la tecla se activa el flag
    teclaval=tec; //se asigna la tecla que corresponde
  }
  tec=presionado;
}

//devuelve la tecla presionada. Si no hay tecla presionada devuelve cero, sino, uno
uint8_t KEYBOARD_devolvertecla(unsigned char *presionado){

  if(!KeyChanged)
    return 0;
  else{
    KeyChanged=0;
    *presionado=teclaval;
    return 1;
  } 
}


