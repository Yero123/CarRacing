/*****************************Funciones para el DAC************************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Uart.h"
#include "Assets.h"
uint32_t timer=0;
uint8_t carril = 2;
const uint8_t selector_x[]={-10,4,29,53,4,29};
const uint8_t selector_y[]={-10,30,30,30,47,47};
const uint8_t pos_carro_y[]={47,29,8};

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
	Nokia5110_DrawFullImage(SplashScreen);
	Nokia5110_DisplayBuffer();
	Nokia5110_DrawFullImage(SplashScreen);
	while(1){
	lectura = UART0_DR_R;
	if(lectura > 0){
			break;
		}
	}
}

void evaluarMovimiento(int *y ){
	
	char lectura =  UART0_DR_R;

		if(lectura=='w'){
			carril=carril + 1;
		}
		
		if(lectura=='s'){
  		carril=carril - 1;
		}
		
		if(carril> 3){
			carril = 3;
		}
		if(carril < 1){
			carril = 1;
		}
		*y=pos_carro_y[carril-1];
}
