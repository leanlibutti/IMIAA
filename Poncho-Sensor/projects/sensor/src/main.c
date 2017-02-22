/*
 * main.c
 *
 *  Created on: 24 ene. 2017
 *      Author: castillo, libutti, sanjuan, risso
 */
/*==================[inclusions]=============================================*/
#include "main.h"       /* <= own header */


/*==================[internal data declaration]==============================*/
static uint16_t ms_ticks=0;
uint16_t umbral_mq135= 100;
uint16_t umbral_mq2= 5000;
uint16_t umbral_mq3= 5000;
uint16_t umbral_temp = 25;
unsigned char hora =0;
unsigned char minutos=0;
unsigned char segundos=0;
unsigned char flag_identificador=0; //utilizado para saber que dato modificar 
unsigned char flag_ubicacion=0; //utilizado para saber que digito del dato voy a leer
unsigned char flag_iniciar =0; 
unsigned char dato;
uint16_t dato2;

uint8_t UART0_IRQHandler(){
	dato= uartReadByte(); //utilizado para la modificacion de la hora del sistema 
	dato2= (uint16_t) dato; //utilizado para la modificacion de los umbrales
	if (flag_identificador != 0){
		switch(flag_identificador){
			case 1 :{ //modificar la hora
				if(flag_ubicacion == 0){
					hora = dato;
					flag_ubicacion ++;
				}
				else{
					hora= (dato*10) + hora;
					flag_ubicacion--;
					flag_identificador=0; 
				} 
				break;
			}
			case 2 :{ //modificar los minutos 		
				if(flag_ubicacion == 0){
					minutos = dato;
					flag_ubicacion ++;
				}
				else{
					minutos= (dato*10) + minutos;
					flag_ubicacion--;
					flag_identificador=0; 
				} 
				break;
			}
			case 3 :{ //modificar los segundos
				if(flag_ubicacion == 0){
					segundos = dato;
					flag_ubicacion ++;
				}
				else{
					segundos= (dato*10) + segundos;
					flag_ubicacion--;
					flag_identificador=0; 
					flag_iniciar=1;
				} 
				break;
			}
			case 4 :{ //modificar el umbral_mq2
				if(flag_ubicacion == 0){
					umbral_mq2 = dato2;
					flag_ubicacion ++;
				}
				else{
					umbral_mq2= (dato2 << 8) | (umbral_mq2);
					flag_ubicacion--;
					flag_identificador=0;
					flag_iniciar=1;
				} 
				break;
			}
			case 5 :{ //modificar l umbral_mq3
				if(flag_ubicacion == 0){
					umbral_mq3 = dato2;
					flag_ubicacion ++;
				}
				else{
					umbral_mq3= ((dato2 << 8) | (umbral_mq3));
					flag_ubicacion--;
					flag_identificador=0;
					flag_iniciar=1;
				} 
				break;
			}
			case 6 :{ //modificar el umbral_mq135
				if(flag_ubicacion == 0){
					umbral_mq135 = dato2;
					flag_ubicacion ++;
				}
				else{
					umbral_mq135= ((dato2 << 8) | (umbral_mq135));
					flag_ubicacion--;
					flag_identificador=0; 
					flag_iniciar=1;
				} 
				break;
			}
			case 7 :{ //modificar el umbral_temp
				if(flag_ubicacion == 0){
					umbral_temp = dato2;
					flag_ubicacion ++;
				}
				else{
					umbral_temp= (dato2 << 8) | (umbral_temp);
					flag_ubicacion--;
					flag_identificador=0; 
					flag_iniciar=1;
				} 
				break;
			}
		}
	}
	else{ //modifico el identificador
		flag_iniciar=0;
		switch(dato){
			case 'h':{
				flag_identificador= 1;
				break;
			}
			case 'm':{
				flag_identificador= 2;
				break;
			}
			case 's':{
				flag_identificador= 3;
				break;
			}
			case '2':{
				flag_identificador= 4;
				break;
			}
			case '3':{
				flag_identificador= 5;
				break;
			}
			case '5':{
				flag_identificador= 6;
				break;
			}
			case 't':{
				flag_identificador= 7;
				break;
			
			}

		}	

	}	

}


void SysTick_Handler(void)
{
	ms_ticks ++;
	if(ms_ticks == 1000){
		segundos ++;
		if (segundos == 60){
			minutos ++;
			segundos=0;
			if (minutos == 60){
				hora ++;
				minutos = 0;
				if (hora == 24)
					hora=0;
			}
		}
		ms_ticks=0;
	}
}

void config_sensor(){
	//configuracion del chip
    Board_Init();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000); // configurado para que interrumpa cada 1 ms

	//configuracion del adc
	analogConfig(ENABLE_ANALOG_INPUTS);

	//configuracion del Rs485
	uint8_t direccion;
	direccion= 1;
	rsConfig(direccion);

	//configuracion de la sd
	config_sd();

}

int main(){
  unsigned char hacer= 2;
  uint16_t medida_mq135;
  uint16_t medida_mq2;
  uint16_t medida_mq3;
  uint16_t medida_temp;
  uint8_t array[2];
  config_sensor();
  while(1){
	//leo las medidas de los 4 sensores (medida en 16 bits de tensiones)
	medida_mq135= analogRead(ADC_CH3); //conectado al CH3
	medida_mq2= analogRead(ADC_CH0); //conectado al CH0
	medida_mq3= analogRead(ADC_CH2); //conectado al CH2
	medida_temp= analogRead(ADC_CH1); //conectado al CH1

	//realizar ajuste y conversion de las medidas realizadas con la medida de la temperatura (falta lo de calibrar con temp)
	medida_mq2= 11.13 * medida_mq2 - 590.66 ; // 200 a 10000 ppm
	medida_mq135= 0.21 * medida_mq135 - 0.27; // 10 a 200 ppm
	medida_mq3= 10.4 * medida_mq3 - 388.76; // 100 a 10000 ppm
	medida_temp= (((3.3/1023) * medida_temp)- 0.79) / 0.01; // 0 a 150 °C
	
	if(flag_iniciar == 1){

		array[0]=medida_mq2 & 0xff;
		array[1]=(medida_mq2 >> 8);
		if(ms_ticks % 125)
			uartWriteByte('2');
		if(ms_ticks % 125)
			uartWriteByte(array[0]); //envia parte baja de los 16 bits
		if(ms_ticks % 125)
			uartWriteByte(array[1]); //envia parte alta de los 16 bits

		array[0]=medida_mq3 & 0xff;
		array[1]=(medida_mq3 >> 8);
		if(ms_ticks % 125)
			uartWriteByte('3'); //envia parte baja de los 16 bits
		if(ms_ticks % 125)
			uartWriteByte(array[0]); //envia parte baja de los 16 bits
		if(ms_ticks % 125)
			uartWriteByte(array[1]); //envia parte alta de los 16 bits

		//enviar medidas reales por rs485 al poncho interfaz
		array[0]=medida_mq135 & 0xff;
		array[1]=(medida_mq135 >> 8);
		if(ms_ticks % 125)
			uartWriteByte('5'); //envia parte baja de los 16 bits
		if(ms_ticks % 125)
			uartWriteByte(array[0]); //envia parte baja de los 16 bits*
		if(ms_ticks % 125)
			uartWriteByte(array[1]); //envia parte alta de los 16 bits

		//guardar medidas reales en la memoria sd
		unsigned char cadena2[]= {'M','q','2',':', ((medida_mq2/10000) + 48), (((medida_mq2 % 10000) / 1000) + 48), (((medida_mq2%1000)/100) + 48), (((medida_mq2%100)/10)+ 48), ((medida_mq2 %10)+ 48),'p','p','m','\n', '\0'};;
		sdWrite(cadena2);

		if (medida_mq2 > umbral_mq2){
			unsigned char cadena5[]= {'M','q','2',':', 'A', 'L', 'A','R','M','A','!','!',' ',' ',' ','\0'};
			sdWrite(cadena5);
			unsigned char cadena_h2[]= {(hora/10) + 48, (hora%10) + 48,':',(minutos/10) + 48, (minutos%10) + 48,':', (segundos/10) + 48, (segundos%10) + 48,'\n','\0'  };
			sdWrite(cadena_h2);
			unsigned char cadena_u1[]= {'U','m' ,'b' ,'r' , 'a','l' , '-', 'M', 'Q', '2',':',((umbral_mq2/10000) + 48), (((umbral_mq2 % 10000) / 1000) + 48), (((umbral_mq2%1000)/100) + 48), (((umbral_mq2%100)/10)+ 48), ((umbral_mq2 %10)+ 48),'p','p','m','\n', '\0'};;
			sdWrite(cadena_u1);
		}
		unsigned char cadena3[]= {'M','q','3', ':', ((medida_mq3/10000) + 48), (((medida_mq3 % 10000) / 1000) + 48), (((medida_mq3%1000)/100) + 48), (((medida_mq3%100)/10)+ 48), ((medida_mq3 %10)+ 48),'p','p','m','\n', '\0'};
		sdWrite(cadena3);
		if (medida_mq3 > umbral_mq3){
			unsigned char cadena6[]= {'M','q','3',':', 'A', 'L', 'A','R','M','A','!','!',' ',' ',' ','\0'};
			sdWrite(cadena6);
			unsigned char cadena_h3[] = {(hora/10) + 48, (hora%10) + 48,':',(minutos/10) + 48, (minutos%10) + 48,':', (segundos/10) + 48, (segundos%10) + 48,'\n','\0' };
			sdWrite(cadena_h3);
			unsigned char cadena_u2[]= {'U','m' ,'b' ,'r' , 'a','l' , '-', 'M', 'Q', '3',':',((umbral_mq3/10000) + 48), (((umbral_mq3 % 10000) / 1000) + 48), (((umbral_mq3%1000)/100) + 48), (((umbral_mq3%100)/10)+ 48), ((umbral_mq3 %10)+ 48),'p','p','m','\n', '\0'};;
			sdWrite(cadena_u2);
		}
		unsigned char cadena1[]= {'M','q','1','3','5',':', ((medida_mq135/10000) + 48), (((medida_mq135 % 10000) / 1000) + 48), (((medida_mq135%1000)/100) + 48), (((medida_mq135%100)/10)+ 48), ((medida_mq135 %10)+ 48),'p','p','m','\n', '\0'};
		sdWrite(cadena1);
		if (medida_mq135 > umbral_mq135){
			unsigned char cadena4[]= {'M','q','1','3','5',':', 'A', 'L', 'A','R','M','A','!','!',' ',' ',' ','\0'};
			sdWrite(cadena4);
			unsigned char cadena_h1[]= {(hora/10) + 48, (hora%10) + 48,':',(minutos/10) + 48, (minutos%10) + 48,':', (segundos/10) + 48, (segundos%10) + 48,'\n','\0'  };
			sdWrite(cadena_h1);
			unsigned char cadena_u3[]= {'U','m' ,'b' ,'r' , 'a','l' , '-', 'M', 'Q', '1', '3', '5',':',((umbral_mq135/10000) + 48), (((umbral_mq135 % 10000) / 1000) + 48), (((umbral_mq135%1000)/100) + 48), (((umbral_mq135%100)/10)+ 48), ((umbral_mq135 %10)+ 48),'p','p','m','\n', '\0'};;
			sdWrite(cadena_u3);
		}

		// mandar la medida de temperatura cada 1 segundo
		if (hacer == 2){
			array[0]=medida_temp & 0x00ff;
			array[1]=(medida_temp >> 8);
			if(ms_ticks % 125)
				uartWriteByte('t');
			if(ms_ticks % 125)
				uartWriteByte(array[0]); //envia parte baja de los 16 bits
			if(ms_ticks % 125)
				uartWriteByte(array[1]); //envia parte alta de los 16 bits
			hacer = 0;
			unsigned char cadena4[]= {'T','e','m','p','e','r','a','t','u','r','a',':', (medida_temp/100) + 48,((medida_temp/10)%10)+48,(medida_temp%10)+48 ,'°','C','\n', '\0'};
			sdWrite(cadena4);
			if (medida_temp > umbral_temp){
				unsigned char cadena7[]= {'T','e','m','p','e','r','a','t','u','r','a',':', 'A', 'L', 'A','R','M','A','!','!',' ',' ',' ','\0'};
				sdWrite(cadena7);
				unsigned char cadena_h4[] = {(hora/10) + 48, (hora%10) + 48,':',(minutos/10) + 48, (minutos%10) + 48,':', (segundos/10) + 48, (segundos%10) + 48,'\n','\0' };
				sdWrite(cadena_h4);
				unsigned char cadena_u4[]= {'U','m' ,'b' ,'r' , 'a','l' , '-','T','e','m','p',':',((umbral_temp/100) + 48), (((umbral_temp / 10) % 10) + 48), ((umbral_temp%10) + 48),'p','p','m','\n', '\0'};;
				sdWrite(cadena_u4); 
			}

		}
		hacer ++;
	}
 }
}
