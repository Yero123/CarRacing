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
#include "Assets.h"

uint32_t timer=0;
const uint8_t selector_x[]={-10,4,29,53,4,29};
const uint8_t selector_y[]={-10,30,30,30,47,47};


void actualizarTimer(){
	if((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)){
 //cada vez que pase 1ms, el contador de tiempo se incrementa
		timer++;
 }
}
void setLevel (uint8_t *nivel){
	char lectura=0x00;
	int i;
	Nokia5110_Clear();
	Nokia5110_OutString("Seleccione");
	Nokia5110_SetCursor(0,1);
	Nokia5110_OutString("un nivel");
	Nokia5110_SetCursor(0,2);
	Nokia5110_OutString("(1)(2)(3)");
	Nokia5110_SetCursor(0,4);
	Nokia5110_OutString("(4)(5)");
	(*nivel) = 1;
	while(lectura != ' '){
		lectura = UART0_DR_R;
		if(lectura == 'd'){
			(*nivel) = (*nivel) + 1;
			if((*nivel) > 5){
			(*nivel) = 1;
			}
			for(i = 0; i <30000; i++){}
		}
		else if(lectura == 'a'){
			(*nivel) = (*nivel) - 1;
			if((*nivel) < 1){
				(*nivel) = 5;
			}
			for(i = 0; i <30000; i++){}
		}
		Nokia5110_ClearBuffer();
	  Nokia5110_PrintBMP(selector_x[(*nivel)], selector_y[(*nivel)], Selector, 2);
	  Nokia5110_DisplayBuffer();
	  Nokia5110_OutString("Seleccione");
	  Nokia5110_SetCursor(0,1);
	  Nokia5110_OutString("un nivel");
	  Nokia5110_SetCursor(0,2);
	  Nokia5110_OutString("(1)(2)(3)");
	  Nokia5110_SetCursor(0,4);
	  Nokia5110_OutString("(4)(5)");
		for(i = 0; i<80000;i++){}
	}
}
void setName(char nickName[]){
	uint16_t letter;
	uint8_t i=0;
	while(i<=3){
		Nokia5110_ClearBuffer();
		Nokia5110_DisplayBuffer();
		Nokia5110_OutString("Ingrese");
		Nokia5110_SetCursor(0,1);
		Nokia5110_OutString("NickName");
		Nokia5110_SetCursor(0,2);
		Nokia5110_SetCursor(10,3);
		Nokia5110_OutString("'");
		Nokia5110_OutString(nickName);
		Nokia5110_OutString("'");
		if(i==3){
			Nokia5110_SetCursor(0,4);
			Nokia5110_OutString("Press to");
			Nokia5110_SetCursor(0,5);
			Nokia5110_OutString("continue");
		}
		letter = RxCar();
		nickName[i]=letter;
		i++;
	}
}

void logoJuego(){
	char lectura= 0x00;
	Nokia5110_ClearBuffer();
	Nokia5110_DrawFullImage(splashScreen);
	Nokia5110_DisplayBuffer();
	Nokia5110_DrawFullImage(splashScreen);
	while(1){
	lectura = UART0_DR_R;
	if(lectura > 0){
			break;
		}
	}
}
void evaluarMovimiento( int *x, int *y ){
	char lectura =  UART0_DR_R;
	if(lectura=='a'){
			(*x)=(*x)-30;
		}
		if(lectura=='d'){
  		(*x)=(*x)+30;
		}
		
		if((*x) > 65){
			(*x) = 65;
		}
		if((*x) < 5){
			(*x) = 5;
		}
		if(lectura=='w'){
			(*y)=(*y)-20;
		}
		
		if(lectura=='s'){
  		(*y)=(*y)+20;
		}
		
		if((*y)> 48){
			(*y) = 47;
		}
		if((*y) < 7){
			(*y) = 10;
		}
}
int main(void)
{
	/*ConfiguraTimer_1ms();*/
	Nokia5110_Init();
  Nokia5110_Clear();
	ConfigUART0();
	
	/*Posicion X Y del personaje */
	int x=35;
	int y=47;
	char nickName[3]={'-','-','-'};
	uint8_t level;
	logoJuego();
	setName(nickName);
	setLevel(&level);
	Nokia5110_DisplayBuffer();
	
	while(1){
		Nokia5110_ClearBuffer();
		Nokia5110_DrawFullImage(camino);
		evaluarMovimiento(&x,&y),
		Nokia5110_PrintBMP(x, y, Enemy10Point1, 2);
		Nokia5110_DisplayBuffer();	
		for(int o=0;o<32000;o++){}
	}
	
		
	return 0;
}