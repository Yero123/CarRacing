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

struct bala{
	int isCrashed;
	int isVisible;
	int x;
	int y;
}
bala1={0,0,30,30},
bala2={0,0,30,30},
bala3={0,0,30,30};

struct enemigo{
	int isCrashed;
	int x;
	int y;
}
enemigo1={0,80,47},
enemigo2={0,30,30},
enemigo3={0,30,30};


struct carro{
	int x;
	int y;
}
carro={2,47};


int tiempo=0;
int disparar=0;

void SysTick_Handler(void){
	tiempo++;
	if(tiempo%500==0){
		if(bala1.isVisible){
			bala1.x=bala1.x+10;
		}
		if(bala1.x>80){
			bala1.x=30;
			bala1.isVisible=0;
		}
	}
	if(tiempo>=1000){
		if(enemigo1.x<30){
			enemigo1.x=80;
		}else{
		enemigo1.x=enemigo1.x-10;
		}
		tiempo=0;
	}
	
}

int main(void)
{
	ConfiguraSysTick();
	Nokia5110_Init();
  Nokia5110_Clear();
	ConfigUART0();
	/*Posicion X Y del personaje */
	char nickName[3]={'-','-','-'};
	uint8_t level;
	logoJuego();
	setName(nickName);
	setLevel(&level);
	Nokia5110_DisplayBuffer();
	/*Juego*/
	while(1){
	
		Nokia5110_ClearBuffer();
		evaluarMovimiento(&carro.y);
		Nokia5110_PrintBMP(carro.x, carro.y, Car, 2);
		Nokia5110_PrintBMP(enemigo1.x,enemigo1.y, Enemigo, 2);
		if(bala1.isVisible)Nokia5110_PrintBMP(bala1.x, bala1.y, Bala, 2);
		Nokia5110_DisplayBuffer();
		Nokia5110_SetCursor(0,1);
		Nokia5110_OutString("__________");
		Nokia5110_SetCursor(0,4);
		Nokia5110_OutString("----------");	
		for(int o=0;o<80000;o++){}
	}
	
}