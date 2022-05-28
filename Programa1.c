/***************************************************************************/
/***** Proyecto: Car Racing                                            *****/
/***************************************************************************/
/***** Microcontrolador: TM4C123GH6PM                                  *****/
/***** EvalBoard: Tiva C Series TM4C123G LaunchPad                     *****/
/***** Autorer:                                                        *****/
/***** -Adrian Anhuaman                                                *****/
/***** -Yerodin Cuellar                                                *****/
/***** Fecha: 2022-1                                                   *****/
/***************************************************************************/
/***** Enunciado:                                                      *****/
/***** Hacer el proyecto mas fachero                                   *****/
/*****                                                                 *****/
/***** Parámetros UART:                                                *****/
/***** 19200 bps, 8 bits de datos, 1 bit de parada, paridad impar.     *****/
/***** habilitación de FIFO                                            *****/
/***************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include <math.h>
#include "Uart.h"
#include "DAC.h"
#include "Timer.h"

uint32_t timer=0;

void actualizarTimer(){
	if((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)){
 //cada vez que pase 1ms, el contador de tiempo se incrementa
		timer++;
 }
}

void Camino(void){
	uint8_t i;
	for(i=0; i<=5; i++){
				Nokia5110_SetCursor(3,i);
				Nokia5110_OutString("|");
				Nokia5110_SetCursor(8,i);
        Nokia5110_OutString("|");
			}
}

const unsigned char Objeto[]={
0x42, 0x4D, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28,
0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00,
0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80,
0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00,
0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00,
0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xF0,
0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00,
0x0F, 0xFF, 0x0F, 0xFF, 0xF0, 0xFF, 0xF0, 0x00, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0,
0x00, 0x0F, 0x00, 0xF0, 0x00, 0x0F, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0xF0,
0x00, 0x00, 0xFF,};

int main(void)
{
	Nokia5110_Init();
  Nokia5110_Clear();
	/*ConfiguraTimer_1ms();*/
	ConfigUART0();
	char lectura;
	int x=35;
	while(1){
		TxCadena("Car racing primera version\n\r");
		lectura = RxCar();
		TxCar(lectura);
		Nokia5110_ClearBuffer();
		Nokia5110_PrintBMP(x, 47, Objeto, 2);
		Nokia5110_DisplayBuffer();
		Camino();
		if(lectura=='a'){
			x=x-30;
		}
		
		if(lectura=='d'){
  		x=x+30;
		}
		
	}
	
	return 0;
}