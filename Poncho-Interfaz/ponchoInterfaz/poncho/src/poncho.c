/*
 * poncho.c
 *
 *  Created on: 24/01/2017
 *  Author: Castillo, Amarís Lorena
 *  		Lubutti, Leandro Ariel
 *  		Risso, Agustín Lautaro
 *  		Sanjuan, Diego Nicolas
 */

#include "chip.h"

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "poncho.h"
#include "stdint.h"
#include "string.h"

//defino los puertos, pines, gpios y gpio pin de cada LED para su configuracion
#define LEDR_P    2
#define LEDR_P_   0
#define LEDR_GPIO 5
#define LEDR_PIN  0

#define LEDG_P    2
#define LEDG_P_   1
#define LEDG_GPIO 5
#define LEDG_PIN  1

#define LEDB_P    2
#define LEDB_P_   2
#define LEDB_GPIO 5
#define LEDB_PIN  2

#define LED1_P    2
#define LED1_P_  10
#define LED1_GPIO 0
#define LED1_PIN 14

#define LED2_P    2
#define LED2_P_  11
#define LED2_GPIO 1
#define LED2_PIN 11

#define LED3_P    2
#define LED3_P_  12
#define LED3_GPIO 1
#define LED3_PIN 12

#define UART0_DIR_P   6
#define UART0_DIR_P_  2
#define UART0_TXD_P   9
#define UART0_TXD_P_  5
#define UART0_RXD_P   9
#define UART0_RXD_P_  6
#define BAUD_RATE     19200

#define TICKRATE_HZ (1000) /* 1000 ticks per second --> 1ms Tick */

#define ENTRADA     0
#define SALIDA    1

#define ON        1
#define OFF       0

#define UART_485      LPC_USART0  /* UART2 (USB-UART) */

/* Define the number of cycles for 1ms */
#define INACCURATE_TO_MS 20400

//variables receptor
uint8_t dato_leido = 0;
volatile uint32_t msTicks = 0;
volatile uint32_t enviar=0;
volatile uint32_t cont = 0; //cada cien ms leo del teclado
//volatile uint8_t flag = 0;
//uint8_t pos = 0;
uint16_t data1=5000;
uint16_t data2=5000;
uint16_t data3=100;
uint16_t data4=25;
uint16_t aux_data;
//variables teclado
unsigned char tecla = ' ';
int filas = 4;
int columnas = 3;
int i, j;
int print = 0;
unsigned char buf='0';
unsigned char flag_escritura=0;
//unsigned char valor[6];

unsigned char cuenta='1'; //indica parte baja y parte alta
unsigned char parte='0';
//unsigned char digito; //utilizado para actualizar el timer
//unsigned char digito2;
extern unsigned char trans;
extern uint16_t valor;
extern unsigned char gas_trans;
extern unsigned char ingresado;
extern unsigned char hora1,hora2,min1,min2,seg1,seg2;
extern uint16_t umbral_temp;
unsigned char prender_led='0';
extern uint16_t umbral_temp;
unsigned char contanding=1;
unsigned char flag_identificador=0; //utilizado para saber que dato modificar
unsigned char flag_ubicacion=0;
unsigned char dato;
uint16_t dato2;
unsigned char mostrar_bienv=0;


const MAXDELAY = 2000000;

static void coreInit(void) {

	/* Read clock settings and update SystemCoreClock variable */
	SystemCoreClockUpdate();

	/* To configure the Systick timer we use the SysTick_Config(uint32_t ticks);
	 * funtion. With ticks we can set the Systick timer interval. In our case we
	 * have a 204 MHz clock and we want it to fire each ms. So the tick has to
	 * be set to 204 MHz / 1000.
	 */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ); /* TICKRATE_HZ ticks per second */
}

/* Set up and initialize board hardware */
void boardInit(void) {

	/* Config Core */
	coreInit();

}

void ledConfig(void) {

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* Config EDU-CIAA-NXP Led Pins as GPIOs */
	Chip_SCU_PinMux(LEDR_P, LEDR_P_, MD_PUP, FUNC4); /* P2_0,  GPIO5[0],  LEDR */
	Chip_SCU_PinMux(LEDG_P, LEDG_P_, MD_PUP, FUNC4); /* P2_1,  GPIO5[1],  LEDG */
	Chip_SCU_PinMux(LEDB_P, LEDB_P_, MD_PUP, FUNC4); /* P2_2,  GPIO5[2],  LEDB */
	Chip_SCU_PinMux(LED1_P, LED1_P_, MD_PUP, FUNC0); /* P2_10, GPIO0[14], LED1 */
	Chip_SCU_PinMux(LED2_P, LED2_P_, MD_PUP, FUNC0); /* P2_11, GPIO1[11], LED2 */
	Chip_SCU_PinMux(LED3_P, LED3_P_, MD_PUP, FUNC0); /* P2_12, GPIO1[12], LED3 */

	/* Config EDU-CIAA-NXP Led Pins as Outputs */
	Chip_GPIO_SetDir(LPC_GPIO_PORT, LEDR_GPIO, (1 << LEDR_PIN), SALIDA);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, LEDG_GPIO, (1 << LEDG_PIN), SALIDA);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, LEDB_GPIO, (1 << LEDB_PIN), SALIDA);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, LED1_GPIO, (1 << LED1_PIN), SALIDA);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, LED2_GPIO, (1 << LED2_PIN), SALIDA);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, LED3_GPIO, (1 << LED3_PIN), SALIDA);

	/* Init EDU-CIAA-NXP Led Pins OFF */
	Chip_GPIO_ClearValue(LPC_GPIO_PORT, LEDR_GPIO, (1 << LEDR_PIN));
	Chip_GPIO_ClearValue(LPC_GPIO_PORT, LEDG_GPIO, (1 << LEDG_PIN));
	Chip_GPIO_ClearValue(LPC_GPIO_PORT, LEDB_GPIO, (1 << LEDB_PIN));
	Chip_GPIO_ClearValue(LPC_GPIO_PORT, LED1_GPIO, (1 << LED1_PIN));
	Chip_GPIO_ClearValue(LPC_GPIO_PORT, LED2_GPIO, (1 << LED2_PIN));
	Chip_GPIO_ClearValue(LPC_GPIO_PORT, LED3_GPIO, (1 << LED3_PIN));

}

/* Initialize GPIO for LEDB Output */
void ledSet(uint8_t status) {
	Chip_GPIO_SetPinState( LPC_GPIO_PORT, LED1_GPIO, LED1_PIN, status);

}

void uartConfig(uint8_t address) {
	Chip_UART_Init(UART_485);
	Chip_UART_SetBaud(UART_485, BAUD_RATE);
	Chip_UART_SetupFIFOS(UART_485, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV3); /* habilita la cola fifo y seteo para que la interrupcion en la recepcion se genere cada 2 bytes */
	Chip_UART_TXEnable(UART_485); /* habilito transmisor */
	Chip_UART_IntEnable(UART_485, UART_IER_RBRINT); /* habilito interrupcion en el receptor */
	Chip_SCU_PinMux(UART0_TXD_P, UART0_TXD_P_, MD_PDN, FUNC7); /* P9_5,FUNC7: UART0_TXD */
	Chip_SCU_PinMux(UART0_RXD_P, UART0_RXD_P_, MD_PLN | MD_EZI | MD_ZI, FUNC7); /* P9_6,FUNC7: UART0_RXD */
	Chip_SCU_PinMux(UART0_DIR_P, UART0_DIR_P_, MD_PDN, FUNC2); /* P6_2,FUNC2: UART0_DIR */
	Chip_UART_SetRS485Flags(UART_485,
	UART_RS485CTRL_NMM_EN | UART_RS485CTRL_DCTRL_EN | UART_RS485CTRL_OINV_1);
	Chip_UART_SetRS485Addr(UART_485, address); /* seteo la direccion de comparacion para la recepcion */
	Chip_UART_ConfigData(UART_485, UART_LCR_WLEN8); /* modo dato de 8 bit */
	/* dudas sobre la prioridad que se setea (mirar datasheet - capitulo NVIC) */
	NVIC_SetPriority(USART0_IRQn, 6);
	/* Enable Interrupt for UART channel */
	NVIC_EnableIRQ(USART0_IRQn);
}

//Transmite datos mediante el protocolo RS-485
void uartWriteByte(uint8_t byte) {

	while ((Chip_UART_ReadLineStatus(UART_485) & UART_LSR_TEMT) == 0)
		; /* Wait for space in FIFO */
	Chip_UART_SendByte(UART_485, byte);
}

//Recibe datos mediante el protocolo RS-485
uint8_t uartReadByte(void) {

	uint8_t receivedByte = 0;

	if (Chip_UART_ReadLineStatus(UART_485) & UART_LSR_RDR) {
		receivedByte = Chip_UART_ReadByte(UART_485);
	}
	return receivedByte;
}

//interrupcion programada cada un segundo
__attribute__ ((section(".after_vectors")))
void SysTick_Handler(void) {
	msTicks++;
	cont++;
	enviar++;
}

//Funcion encargada de guardar los datos recibidos de los sensores en las variables correspondientes
void guardar_valores4(){
	dato= uartReadByte();
	if (flag_identificador != 0){
		switch(flag_identificador){
			case 1 :{ //guarda propano
				if(flag_ubicacion == 0){
					flag_escritura=0;
					data1= dato;
					flag_ubicacion ++;
				}
				else{
					data1=data1|(dato<<8);
					flag_ubicacion--;
					flag_identificador=0;
					flag_escritura=1;
				}
				break;
			}
			case 2 :{ //guarda monoxido de carbono
				if(flag_ubicacion == 0){
					flag_escritura=0;
					data2 = dato;
					flag_ubicacion ++;
				}
				else{
					data2=data2 |(dato<<8);
					flag_ubicacion--;
					flag_identificador=0;
					flag_escritura=1;
				}
				break;
			}
			case 3 :{ //guarda dioxido de carbono
				if(flag_ubicacion == 0){
					flag_escritura=0;
					data3 = dato;
					flag_ubicacion ++;
				}
				else{
					data3=data3 | (dato<<8);
					flag_ubicacion--;
					flag_identificador=0;
					flag_escritura=1;
				}
				break;
			}
			case 4 :{ //guarda temperatura
				if(flag_ubicacion == 0){
					data4=dato;
					flag_ubicacion ++;
					flag_escritura=0;
				}
				else{
					data4=data4|(dato<<8);
					flag_ubicacion--;
					flag_identificador=0;
					flag_escritura=1;
				}
				break;
			}
		}
	}
	else{ //ubica el identificador del dato a recibir
		switch(dato){
			case '2':{
				flag_identificador= 1;
				break;
			}
			case '3':{
				flag_identificador= 2;
				break;
			}
			case '5':{
				flag_identificador= 3;
				break;
			}
			case 't':{
				flag_identificador= 4;
				break;
			}

		}

	}

}

// Interrupción de la UART. Se atiende cada vez que se recibe un dato por el protocolo RS485
uint8_t UART0_IRQHandler(void) {


			guardar_valores4(); //guarda el dato recibido en la variable correspondiente
			if(data4>umbral_temp){ //si la temperatura supera el umbral, activa el flag, sino, lo apaga
				prender_led='1';
			}
			else{
				prender_led='0';
			}
}

//transmite el umbral modificado por el usuario a la placa de sensado, para que actualice el nuevo umbral de trabajo
void transmitir(uint16_t valor, unsigned char dat){
	unsigned char tipo;
	uint8_t array[2];
	switch (dat) {
	case 'P': //propano
		tipo='2';
		break;
	case 'C': //CO
		tipo='3';
		break;
	case '2': //CO2
		tipo='5';
		break;
	case 'T': //temperatura
		tipo='t';
	break;
	}

	switch(contanding){ //se utiliza contanding para enviar primero el tipo, luego la parte baja y finalmente la parte alta
		case 1:
			uartWriteByte(tipo);
			contanding++;
		break;
		case 2:
			array[0]=valor & 0xff;
			uartWriteByte(array[0]);
			contanding++;
		break;
		case 3:
			array[1]=(valor >> 8);
			uartWriteByte(array[1]);
			contanding=1;
			trans='0';

		break;
	}

}
//Funcion encargada de transmitir la hora ingresada por teclado a la placa de sensado.
//la variable ingresado se encarga de transmitir en orden los identificadores y los digitos de las horas, minutos y segundos
void trans_fecha(unsigned char hora1, unsigned char hora2, unsigned char min1, unsigned char min2, unsigned char seg1,unsigned char seg2){
	switch(ingresado){
		case 1:
			uartWriteByte('h');
			ingresado++;
		break;
		case 2:
			uartWriteByte(hora2-48);
			ingresado++;
		break;
		case 3:
			uartWriteByte(hora1-48);
			ingresado++;
		break;
		case 4:
			uartWriteByte('m');
			ingresado++;
		break;
		case 5:
			uartWriteByte(min2-48);
			ingresado++;
		break;
		case 6:
			uartWriteByte(min1-48);
			ingresado++;
		break;
		case 7:
			uartWriteByte('s');
			ingresado++;
		break;
		case 8:
			uartWriteByte(seg2-48);
			ingresado++;
		break;
		case 9:
			uartWriteByte(seg1-48);
			ingresado++;
		break;
	}
}

int main(void) {

	//receptor variables
	uint8_t address = 1;
	unsigned char aux;
	unsigned char cont_in=0;

	//configuracion de leds y del reloj del sistema
	boardInit();
	ledConfig();
	ledSet(OFF);
	//inicializacion del LCD y teclado
	lcd_init_port();
	lcd_init();
	KEYBOARD_init();
	lcd_gotoxy(1,1);

	//inicializacion del Receptor
	uartConfig(address);

	//inicializacion del LED de umbral de temperatura
	port_pin(6,12, MD_PUP,init_out);

	//inicializacion de la MEF
	Init_MEF();

	while (1) {

		//cada 20 ms se escanea la tecla ingresada por teclado
		if (cont >= 20){

			KEYBOARD_rebote();
			aux=KEYBOARD_devolvertecla(&tecla);
			cont=0;
			//en el estado inicial, se muestra un cartel de bienvenida. Después de dos segundos se deja de mostrar.
			if(cont_in<100){
				cont_in++;
			}
			else{
				mostrar_bienv=1;
			}
		}
		//cada 100ms se verifica que haya datos para transmitir. si los hay, se transmite.
		if(enviar>=100){ 
			if(ingresado>0 && ingresado<10){
				trans_fecha(hora1,hora2,min1,min2,seg1,seg2);
			}
			if(trans=='1'){
				transmitir(valor,gas_trans);
			}
					enviar=0;

		}

		//actualiza la MEF cada 500 ms.
		if (msTicks >= 500) {
			lcd_gotoxy(1,1);
			UpdateMEF(tecla,data1,data2,data3,data4);
			msTicks = 0;
			tecla=' '; //limpia el valor de la tecla ingresada
		}

		//enciende el LED si el flag está activo, sino lo apaga.

		if(prender_led=='1'){
			port_pin(6,12, disable, high);
		}
		else{
			port_pin(6,12, disable, low);
		}

	}

}
