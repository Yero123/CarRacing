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

const uint32_t vel_level[]={1000,300,250,200,150,100};
uint8_t level = 0;

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
	int isVisible;
	int x;
	int y;
}
enemigo1={0,0,64,47},
enemigo2={0,0,64,29},
enemigo3={0,0,64,8};

int count=0;
struct carro{
	int x;
	int y;
}
carro={2,47};

int aleatorio=0;
int aleatorio_2 = 0;
int aleatorio_3 = 0;
int i= 0;
int tiempo=0;
int disparar=0;



void SysTick_Handler(){
	tiempo++;
	i++;
	if(i%11 == 0){
		srand(tiempo);
	}
	aleatorio=rand()%3;
	if(tiempo%11 == 0){
		srand(i+4);
	}
	aleatorio_2=rand()%3;
	if(i%6 == 0){
		srand(i+10);
	}
	aleatorio_3=rand()%3;
	aleatorio = (aleatorio + aleatorio_2 + aleatorio_3)%3;
	if(tiempo%500==0){
		if(bala1.isVisible){
			bala1.x=bala1.x+10;
		}
		if(bala1.x>80){
			bala1.x=30;
			bala1.isVisible=0;
		}
	}
	if(i >=739){
		i = 0;
	}
	if(tiempo>=vel_level[level]){
		if(enemigo1.x<20){
			enemigo1.isVisible=0;
		}else{
		enemigo1.x=enemigo1.x-10;
		}
		if(enemigo2.x<20){
			enemigo2.isVisible=0;
		}else{
		enemigo2.x=enemigo2.x-10;
		}
		if(enemigo3.x<20){
			enemigo3.isVisible=0;
		}else{
		enemigo3.x=enemigo3.x-10;
		}
		tiempo=0;
		count++;
		if(count==4){
			count=0;
		}
	}
if(count==3){
	  count = 0;
		if(aleatorio==0 && !enemigo1.isVisible){
			enemigo1.isVisible=1;
			enemigo1.x=64;
		}
		if(aleatorio==1 && !enemigo2.isVisible){
			enemigo2.isVisible=1;
			enemigo2.x=64;
		}
		if(aleatorio==2 && !enemigo3.isVisible){
			enemigo3.isVisible=1;
			enemigo3.x=64;
		}
	}

}


void evaluarDisparo(){
	char lectura2 = UART0_DR_R;
	if(lectura2 == 't'){
		bala1.isVisible=1;
		bala1.y=carro.y;
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
	
	logoJuego();
	setName(nickName);
	setLevel(&level);
	Nokia5110_DisplayBuffer();
	
	while(1){
		
	  //evaluarDisparo();
		Nokia5110_ClearBuffer();
		evaluarMovimiento(&carro.y);
		Nokia5110_PrintBMP(carro.x, carro.y, Car, 2);
		if(enemigo1.isVisible)Nokia5110_PrintBMP(enemigo1.x,enemigo1.y, Enemigo, 2);
		if(enemigo2.isVisible)Nokia5110_PrintBMP(enemigo2.x,enemigo2.y, Enemigo, 2);
		if(enemigo3.isVisible)Nokia5110_PrintBMP(enemigo3.x,enemigo3.y, Enemigo, 2);
		//if(bala1.isVisible)Nokia5110_PrintBMP(bala1.x, bala1.y, Bala, 2);
		Nokia5110_DisplayBuffer();
		Nokia5110_SetCursor(0,1);
		Nokia5110_OutString("__________");
		Nokia5110_SetCursor(0,4);
		Nokia5110_OutString("----------");	
		for(int o=0;o<80000;o++){}
	}
	
}
