/*
 * mef.c
 *
 *  Created on: 27/1/2017
 *      Author: Diego
 */

#include "mef.h"
unsigned char trans;
int bol;
int conta;
unsigned char hora1, hora2, min1, min2, seg1, seg2;
unsigned char gas;
unsigned char gas2;
unsigned char gas_trans;
unsigned char prop;
unsigned char error;
unsigned char primera;
unsigned char valingresado1;
unsigned char valingresado2;
unsigned char valingresado3;
unsigned char valingresado4;
unsigned char valingresado5;
unsigned char estado;
uint16_t valor;
unsigned char estado_ant;
uint16_t umbral_temp;
unsigned char trans;
unsigned char ingresado = 0;
extern unsigned char flag_escritura;
extern unsigned char mostrar_bienv;

uint16_t convertirAscii_Num(unsigned char s1, unsigned char s2, unsigned char s3, unsigned char s4, unsigned char s5) {
	int n1 = s1 - '0';
	int n2 = s2 - '0';
	int n3 = s3 - '0';
	int n4 = s4 - '0';
	int n5 = s5 - '0';
	uint16_t result = ((n1 * 10000) + (n2 * 1000) + (n3 * 100) + (n4 * 10) + n5);
	return result;
}
uint16_t convertirAscii_Num2(unsigned char s1, unsigned char s2, unsigned char s3) {
	int n1 = s1 - '0';
	int n2 = s2 - '0';
	int n3 = s3 - '0';
	uint16_t result = ((n1 * 100) + (n2 * 10) + n3);
	return result;
}

void Init_MEF(void) {

		estado = 'I';
		conta = 0;
		gas = 'H';
		gas2 = 'H';
		bol=0;
		trans = '0';
		prop=' ';
		error='0';
		primera='0';
		umbral_temp=40;
		valingresado1 = ' ';
		valingresado2 = ' ';
		valingresado3 = ' ';
		valingresado4 = ' ';
		valingresado5 = ' ';
		lcd_gotoxy(1, 1);
		printf_lcd("                ");
		lcd_gotoxy(1, 2);
		printf_lcd("                ");
		estado_ant = estado;
}

void controlar_gas(unsigned char tecla, unsigned char gas){
	switch(conta){
		case 0:
			switch(gas){
				case 'C':
				case 'P':
					if ((tecla == '0') || (tecla == '1')){
						valingresado1 = tecla;
						lcd_gotoxy(7, 2);
						lcd_putc(valingresado1);
						conta++;

					}
					else{
						lcd_gotoxy(1,1);
						printf_lcd("Error. Rango 0-1");
					}
				break;
				case '2':
					if(tecla=='0'){
						valingresado1=tecla;
						lcd_gotoxy(7, 2);
						lcd_putc(valingresado1);
						conta++;
					}
					else{
						lcd_gotoxy(1, 1);
						printf_lcd("Error. Rango 0  ");
					}
				break;
			}
		break;
		case 1:
			switch(gas){
				case 'P':
				case 'C':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					if (valingresado1 == '1'){
						if (tecla != '0'){
						lcd_gotoxy(1, 1);
						printf_lcd("Error. Rango 0  ");
						}
						else{
							valingresado2 = tecla;
							lcd_gotoxy(8, 2);
							lcd_putc(valingresado2);
							conta++;
						}

					}
					else{
						if ((tecla >= '0') && (tecla <= '9')){
							valingresado2 = tecla;
							lcd_gotoxy(8, 2);
							lcd_putc(valingresado2);
							conta++;
						}
						else{
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 0-9");
						}
					}
				break;
				case '2':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					if(tecla=='0'){
						valingresado2=tecla;
						lcd_gotoxy(8, 2);
						lcd_putc(valingresado2);
						conta++;
					}
					else{
						lcd_gotoxy(1, 1);
						printf_lcd("Error. Rango 0  ");
					}



				break;
			}
		break;
		case 2:
			switch(gas){
				case 'P':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					lcd_gotoxy(8, 2);
					lcd_putc(valingresado2);
					if(valingresado1=='0' && valingresado2=='0'){
						if(tecla>='2'){
							valingresado3=tecla;
							lcd_gotoxy(9, 2);
							lcd_putc(valingresado3);
							conta++;
						}
						else{
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 2-9");							
						}
					}
					else{
						if (valingresado1 == '1'){
							if (tecla != '0'){
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0  ");
							}
							else{
								valingresado3 = tecla;
								lcd_gotoxy(9, 2);
								lcd_putc(valingresado3);
								conta++;
							}

						}
						else{
							if ((tecla >= '0') && (tecla <= '9')){
								valingresado3 = tecla;
								lcd_gotoxy(9, 2);
								lcd_putc(valingresado3);
								conta++;
							}
							else{
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0-9");
							}

						}

					}
				break;
				case 'C':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					lcd_gotoxy(8, 2);
					lcd_putc(valingresado2);
					if(valingresado1=='0' && valingresado2=='0'){
						if(tecla>='1'){
							valingresado3=tecla;
							lcd_gotoxy(9, 2);
							lcd_putc(valingresado3);
							conta++;
						}
						else{
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 1-9");							
						}
					}
					else{
						if (valingresado1 == '1'){
							if (tecla != '0'){
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0  ");
							}
							else{
								valingresado3 = tecla;
								lcd_gotoxy(9, 2);
								lcd_putc(valingresado3);
								conta++;
							}

						}
						else{
							if ((tecla >= '0') && (tecla <= '9')){
								valingresado3 = tecla;
								lcd_gotoxy(9, 2);
								lcd_putc(valingresado3);
								conta++;
							}
							else{
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0-9");
							}

						}

					}
				break;
				case '2':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					lcd_gotoxy(8, 2);
					lcd_putc(valingresado2);
					if(tecla<='2'){
						valingresado3=tecla;
						lcd_gotoxy(9, 2);
						lcd_putc(valingresado3);
						conta++;
					}
					else{
						lcd_gotoxy(1, 1);
						printf_lcd("Error. Rango 0-2");							
					}
					
				break;
			}
		break;
		case 3:
				switch(gas){
					case 'C':
				case 'P':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					lcd_gotoxy(8, 2);
					lcd_putc(valingresado2);
					lcd_gotoxy(9, 2);
					lcd_putc(valingresado3);
					if (valingresado1 == '1'){
						if (tecla != '0'){
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 0  ");
						}
						else{
							valingresado4 = tecla;
							lcd_gotoxy(10, 2);
							lcd_putc(valingresado4);
							conta++;
						}

					}
					else{
						if ((tecla >= '0') && (tecla <= '9')){
							valingresado4 = tecla;
							lcd_gotoxy(10, 2);
							lcd_putc(valingresado4);
							conta++;
						}
						else{
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 0-9");
						}
					}

				break;
				case '2':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					lcd_gotoxy(8, 2);
					lcd_putc(valingresado2);
					lcd_gotoxy(9, 2);
					lcd_putc(valingresado3);
					if(valingresado3=='0'){
						if(tecla>='1'){
							valingresado4=tecla;
							lcd_gotoxy(10, 2);
							lcd_putc(valingresado4);
							conta++;
						}
						else{
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 1-9");
						}
					}
					else{
						if(valingresado3<'2'){
							if ((tecla >= '0') && (tecla <= '9')){
								valingresado4 = tecla;
								lcd_gotoxy(10, 2);
								lcd_putc(valingresado4);
								conta++;
							}
							else{
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0-9");
							}

						}
						else{
							if(valingresado3=='2'){
								if(tecla=='0'){
									valingresado4 = tecla;
									lcd_gotoxy(10, 2);
									lcd_putc(valingresado4);
									conta++;
								}
								else{
									lcd_gotoxy(1, 1);
									printf_lcd("Error. Rango 0  ");
								}

							}
						}

					}
				break;
				}
		break;
		case 4:
			switch(gas){
				case 'P':
				case 'C':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					lcd_gotoxy(8, 2);
					lcd_putc(valingresado2);
					lcd_gotoxy(9, 2);
					lcd_putc(valingresado3);
					lcd_gotoxy(10, 2);
					lcd_putc(valingresado4);
					if (valingresado1 == '1'){
						if (tecla != '0'){
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 0  ");
						}
						else{
							valingresado5 = tecla;
							lcd_gotoxy(11, 2);
							lcd_putc(valingresado5);
							conta++;
						}

					}
					else{
						if ((tecla >= '0') && (tecla <= '9')){
							valingresado5 = tecla;
							lcd_gotoxy(11, 2);
							lcd_putc(valingresado5);
							conta++;
						}
						else{
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 0-9");
						}
					}
				break;
				case '2':
					lcd_gotoxy(7, 2);
					lcd_putc(valingresado1);
					lcd_gotoxy(8, 2);
					lcd_putc(valingresado2);
					lcd_gotoxy(9, 2);
					lcd_putc(valingresado3);
					lcd_gotoxy(10, 2);
					lcd_putc(valingresado4);
					if(valingresado3<'2'){
						if ((tecla >= '0') && (tecla <= '9')){
							valingresado5 = tecla;
							lcd_gotoxy(11, 2);
							lcd_putc(valingresado5);
							conta++;
						}
						else{
							lcd_gotoxy(1, 1);
							printf_lcd("Error. Rango 0-9");
						}

					}
					else{
						if(valingresado3=='2'){
							if(tecla=='0'){
								valingresado5 = tecla;
								lcd_gotoxy(10, 2);
								lcd_putc(valingresado5);
								conta++;
							}
							else{
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0  ");
							}

						}
					}

				break;
			}
		break;
	}

}

void set_umbral(unsigned char tecla,unsigned char gas){
	if(gas=='T'){
				switch(conta){
					case 0:
						if (tecla == '1' || tecla == '0'){
							valingresado1 = tecla;
							lcd_gotoxy(7, 2);
							lcd_putc(valingresado1);
							conta++;
						}
						else{
							lcd_gotoxy(1,1);
							printf_lcd("Error. Rango 0-1");
						}
					break;
					case 1:
						lcd_gotoxy(7, 2);

						lcd_putc(valingresado1);
						if (valingresado1 == '1'){
							if (tecla != '0'){
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0  ");
							}
							else{
								valingresado2 = tecla;
								lcd_gotoxy(8, 2);
								lcd_putc(valingresado2);
								conta++;
							}

						}
						else{
							if ((tecla >= '0') && (tecla <= '9')){
								valingresado2 = tecla;
								lcd_gotoxy(8, 2);
								lcd_putc(valingresado2);
								conta++;
							}
							else{
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0-9");
							}
						}
					break;
					case 2:
						lcd_gotoxy(7, 2);
						lcd_putc(valingresado1);
						lcd_gotoxy(8, 2);
						lcd_putc(valingresado2);
						if (valingresado1 == '1'){
							if (tecla != '0'){
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0  ");
							}
							else{
								valingresado3 = tecla;
								lcd_gotoxy(9, 2);
								lcd_putc(valingresado3);
								conta++;
							}

						}
						else{
							if ((tecla >= '0') && (tecla <= '9')){
								valingresado3 = tecla;
								lcd_gotoxy(9, 2);
								lcd_putc(valingresado3);
								conta++;
							}
							else{
								lcd_gotoxy(1, 1);
								printf_lcd("Error. Rango 0-9");
							}
						}
					break;
				}

	}
	else{
		controlar_gas(tecla,gas);
	}
}


void set_hora(int indice, unsigned char tecla){
	switch(indice){
		case 0:
			if ((tecla >= '0') && (tecla <= '2')){
				hora1 = tecla;
				lcd_gotoxy(5, 2);
				lcd_putc(hora1);
				bol++;
			}
			else {
				lcd_gotoxy(1, 1);
				printf_lcd("Error. Rango 0-2");
			}

		break;
		case 1:
			lcd_gotoxy(5, 2);
			lcd_putc(hora1);
			if(hora1=='2'){
				if ((tecla >= '0') && (tecla <= '3')) {
					hora2 = tecla;
					lcd_gotoxy(6, 2);
					lcd_putc(hora2);
					bol++;
				}
				else {
					lcd_gotoxy(1, 1);
					printf_lcd("Error. Rango 0-3");
				}
			}
			else{
				if ((tecla >= '0') && (tecla <= '9')){
					hora2 = tecla;
					lcd_gotoxy(6, 2);
					lcd_putc(hora2);
					bol++;
				}
				else {
					lcd_gotoxy(1, 1);
					printf_lcd("Error. Rango 0-9");
				}

			}
		break;
		case 2:
			lcd_gotoxy(5, 2);
			lcd_putc(hora1);
			lcd_gotoxy(6, 2);
			lcd_putc(hora2);
			if ((tecla >= '0') && (tecla <= '5')){
				min1 = tecla;
				lcd_gotoxy(8, 2);
				lcd_putc(min1);
				bol++;
			}
			else {
				lcd_gotoxy(1, 1);
				printf_lcd("Error. Rango 0-5");
			}
		break;
		case 3:
			lcd_gotoxy(5, 2);
			lcd_putc(hora1);
			lcd_gotoxy(6, 2);
			lcd_putc(hora2);
			lcd_gotoxy(8, 2);
			lcd_putc(min1);
			if ((tecla >= '0') && (tecla <= '9')){
				min2 = tecla;
				lcd_gotoxy(9, 2);
				lcd_putc(min2);
				bol++;
			}
			else {
				lcd_gotoxy(1, 1);
				printf_lcd("Error. Rango 0-9");
			}
		break;
		case 4:
			lcd_gotoxy(5, 2);
			lcd_putc(hora1);
			lcd_gotoxy(6, 2);
			lcd_putc(hora2);
			lcd_gotoxy(8, 2);
			lcd_putc(min1);
			lcd_gotoxy(9, 2);
			lcd_putc(min2);
			if ((tecla >= '0') && (tecla <= '5')){
				seg1 = tecla;
				lcd_gotoxy(11, 2);
				lcd_putc(seg1);
				bol++;
			}
			else{
				lcd_gotoxy(1, 1);
				printf_lcd("Error. Rango 0-5");
			}
		break;
		case 5:
			lcd_gotoxy(5, 2);
			lcd_putc(hora1);
			lcd_gotoxy(6, 2);
			lcd_putc(hora2);
			lcd_gotoxy(8, 2);
			lcd_putc(min1);
			lcd_gotoxy(9, 2);
			lcd_putc(min2);
			lcd_gotoxy(11, 2);
			lcd_putc(seg1);
			if ((tecla >= '0') && (tecla <= '9')){
				seg2 = tecla;
				lcd_gotoxy(12, 2);
				lcd_putc(seg2);
				bol++;

			}
			else{
				lcd_gotoxy(1, 1);
				printf_lcd("Error. Rango 0-9");
			}
		break;
	}

}

void imprimir(uint16_t data) {

	lcd_gotoxy(1, 2);
	printf_lcd("                ");
	lcd_gotoxy(5, 2);
	if(prop=='2'){
		unsigned char cadena2[]= {((data/100000) + 48),((data/10000) + 48),',', (((data % 10000) / 1000) + 48), (((data%1000)/100) + 48), (((data%100)/10)+ 48), ((data%10)+ 48), '\0'};
		printf_lcd(cadena2);
		lcd_gotoxy(11, 2);
		lcd_putc('%');
	} else {
		unsigned char cadena[]= {((data/10000) + 48), (((data % 10000) / 1000) + 48), (((data%1000)/100) + 48), (((data%100)/10)+ 48), ((data%10)+ 48), '\0'};
		printf_lcd(cadena);
		if(gas!='T'){
			lcd_gotoxy(11, 2);
			printf_lcd("PPM");

		}
		else{
			lcd_gotoxy(11, 2);
			lcd_putc('C');
		}
	}
}

void printing(unsigned char gas, uint16_t data) {
	lcd_gotoxy(1, 1);
	switch (gas) {
	case 'P':
		printf_lcd(" GAS: PROPANO   ");
		break;
	case 'C':
		printf_lcd(" GAS: CO        ");
		break;
	case '2':
		printf_lcd(" GAS: CO2       ");
		break;
	case 'T':
		printf_lcd("  TEMPERATURA   ");
		break;
	}
	imprimir(data);
}




void UpdateMEF(unsigned char tecla,uint16_t data1,uint16_t data2,uint16_t data3, uint16_t data4) {

	switch (estado) {
	case 'I': //inicio
		lcd_gotoxy(1, 1);
		if(mostrar_bienv==0){
			printf_lcd("   BIENVENIDO   ");
		}
		else{
			printf_lcd("  INGRESE HORA  ");
			if(error=='0'){
					lcd_gotoxy(1, 2);
					printf_lcd("    HH:MM:SS    ");
					error='1';
				}
				lcd_gotoxy(1, 2);
				if(bol<6 && tecla!=' '){
					lcd_gotoxy(1, 2);
					set_hora(bol,tecla);
				}
				else{
					if(bol==6){
						estado_ant = estado;
						ingresado=1;
						estado='M';
					}
				}
		}

		break;

	case 'M': //menu
		lcd_gotoxy(1, 1);
		printf_lcd("   1_VER DATOS   ");
		lcd_gotoxy(1, 2);
		printf_lcd(" 2_MOD UMBRALES ");
		if (tecla == '1') {
			estado_ant = estado;
			estado = 'V'; //estado ver datos
		} else {
			if (tecla == '2') {
				estado_ant = estado;
				estado = 'C'; //estado cambiar umbral
			}
		}
		break;

	case 'V': //ver datos
		lcd_gotoxy(1, 1);
		printf_lcd(" 3_PROP  4_CO   ");
		lcd_gotoxy(1, 2);
		printf_lcd(" 5_CO2  6_TEMP  ");
		if (tecla == '3' || tecla == '4' || tecla == '5') {
			if (tecla == '3') {
				gas = 'P';
			}
			else {
				if (tecla == '4') {
					gas = 'C';
				} else {
					if (tecla == '5') {
						gas = '2';
					}
				}
			}
			estado_ant = estado;
			estado = 'P'; // estado proporcion
		}
		else{
			if (tecla == '6') {
				estado_ant = estado;
				estado = 'T';
				gas='T';
			} else {
				if (tecla == '*') {
					estado = estado_ant;
					estado_ant = 'M';
					gas='H';
				}
			}
		}
		break;
	case 'P': //proporcion
		lcd_gotoxy(1, 1);
		printf_lcd("   PROPORCION   ");
		lcd_gotoxy(1, 2);
		printf_lcd(" 7_PPM  8_%    ");
		if (tecla == '7' || tecla == '8') {
			if (tecla == '7')
				prop = '1';
			else
				prop = '2';
			estado_ant = estado;
			estado = 'G';
		} else {
			if (tecla == '*') {
				estado = estado_ant;
				gas='H'; //ver despues
				estado_ant = 'M';
			}
		}

		break;
	case 'G': //ver datos de los gases
		//obtiene los valores de la recta del gas y hace las debidas conversiones
	  	if (tecla == '*') {
	  		estado = estado_ant;
			estado_ant = 'V';
			prop=' ';
	  	}
	  	else{
	  		if(flag_escritura==1){
	  			if(gas=='P'){
	  				  			printing(gas,data1);
	  				  		}
	  				  		else{
	  				  			if(gas=='C'){
	  				  				printing(gas,data2);
	  				  			}
	  				  			else{
	  				  				if(gas=='2'){
	  				  					printing(gas,data3);
	  				  				}
	  				  			}
	  				  		}
	  		}
	  	}
		break;
	case 'T': // ver temperatura
		if (tecla == '*') {
			estado = estado_ant;
			estado_ant = 'M';
			gas='H';
		}
		else{
			printing(gas,data4);
		}
		break;
	case 'C': //cambiar datos
		lcd_gotoxy(1, 1);
		printf_lcd(" 3_PROP  4_CO   ");
		lcd_gotoxy(1, 2);
		printf_lcd(" 5_CO2  6_TEMP  ");
		if (tecla == '3' || tecla == '4' || tecla == '5' || tecla=='6') {
			if (tecla == '3') {
				gas2 = 'P';
			} else {
				if (tecla == '4') {
					gas2 = 'C';
				} else {
					if (tecla == '5') {
						gas2 = '2';
					} else {
						if (tecla == '6') {
							gas2 = 'T';
						}
					}
				}
			}
			estado_ant = estado;
			estado = 'U'; // estado proporcion
		}
		else{
			if (tecla == '*') {
				estado = estado_ant;
				estado_ant = 'M';
				gas2='H';
			}
		}
		break;
	case 'U': //modificar umbral
	 if (tecla == '*') {
	 	valingresado1=' ';
	 	valingresado2=' ';
	 	valingresado3=' ';
	 	valingresado4=' ';
	 	valingresado5=' ';
	 	gas2='H';
	 	estado = estado_ant;
		estado_ant = 'C';
		trans = '0';
		conta=0;
		primera='0';
	 }
	 else{
	 	lcd_gotoxy(1, 1);
		printf_lcd(" INGRESE UMBRAL ");
		if(primera=='0'){
			primera='1';
			lcd_gotoxy(1, 2);
			if(gas2=='P' || gas2=='C' || gas2=='2'){
				printf_lcd("      XXXXX     "); //arranca en 7
			}
			else{
				if(gas2=='T'){
					printf_lcd("      XXX       "); //arranca en 7
				}
			}
		}
		if(gas2=='P' || gas2=='C' || gas2=='2'){
 			if(conta<5 && tecla!=' '){
				set_umbral(tecla,gas2);
			}
			else{
				if(conta==5){
						valor = convertirAscii_Num(valingresado1, valingresado2, valingresado3, valingresado4, valingresado5);
	 					valingresado1=' ';
	 					valingresado2=' ';
	 					valingresado3=' ';
	 					valingresado4=' ';
	 					valingresado5=' ';
	 					gas_trans=gas2;
	 					gas2='H';
	 					estado = estado_ant;
						estado_ant = 'C';
						trans = '1';
						conta=0;
						primera='0';
				}
			}
		}
		else{
			if(gas2=='T'){
	 			if(conta<3 && tecla!=' '){
					set_umbral(tecla,gas2);
				}
				else{
					if(conta==3){
							valor = convertirAscii_Num2(valingresado1, valingresado2, valingresado3);
	 						valingresado1=' ';
	 						valingresado2=' ';
	 						valingresado3=' ';
	 						valingresado4=' ';
	 						valingresado5=' ';
	 						gas_trans=gas2;
	 						gas2='H';
	 						estado = estado_ant;
	 						umbral_temp=valor;
							estado_ant = 'C';
							trans = '1';
							conta=0;
							primera='0';
					}
				}
			}
		}

	}

	break;
	}

}
