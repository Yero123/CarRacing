/***************************************************************************/
/***** Proyecto: Car Racing                                            *****/
/***************************************************************************/
/***** Microcontrolador: TM4C123GH6PM                                  *****/
/***** EvalBoard: Tiva C Series TM4C123G LaunchPad                     *****/
/***** Autores:                                                        *****/
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
#include "DAC.h"
#include "Timer.h"
#include "LogicGame.h"



int main(void)
{
	/*ConfiguraTimer_1ms();*/
	Nokia5110_Init();
  Nokia5110_Clear();
	ConfigUART0();
	
	/*Posicion X Y del personaje */
	int x=4;
	int y=47;
	char nickName[3]={'-','-','-'};
	uint8_t level;
	logoJuego();
	setName(nickName);
	setLevel(&level);
	Nokia5110_DisplayBuffer();
	/*Juego*/
	while(1){
		Nokia5110_ClearBuffer();
		evaluarMovimiento(&y);
		Nokia5110_PrintBMP(x, y, Car, 2);
		Nokia5110_DisplayBuffer();
		Nokia5110_SetCursor(0,1);
		Nokia5110_OutString("__________");
		Nokia5110_SetCursor(0,4);
		Nokia5110_OutString("----------");	
		for(int o=0;o<80000;o++){}
	}
	
}