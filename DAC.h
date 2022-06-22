/*****************************Funciones para el DAC************************************/

#include "tm4c123gh6pm.h"

uint32_t Tensiones[4] = {0,1,2,3}; //Variable para el DAC

void KEYS_Init(void){
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3 ; // Activa puerto D
  while ( ( SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R3 )== 0 ); //Espera activacion de puerto
  GPIO_PORTD_AFSEL_R &= ~0x03;              // Desactiva funciones alternativas PD1-0
	GPIO_PORTD_AMSEL_R &= ~0x03;              // No se requieren funciones analógicas
  GPIO_PORTD_PCTL_R  &= ~0x00FFFFFF;        // No selecciona ninguna función alternaticas
  GPIO_PORTD_DIR_R   &= ~0x03;               // Configura como salidas
  GPIO_PORTD_DEN_R   |= 0x03;               // Habilita la funcion digital sobre PD1-0
}

void DAC_Init(void){
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4 ; // Activa puerto E
  while ( ( SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4 )== 0 ); //Espera activacion de puerto
  GPIO_PORTE_AFSEL_R &= ~0x03;              // Desactiva funciones alternativas PE1-0
	GPIO_PORTE_AMSEL_R &= ~0x03;              // No se requieren funciones analógicas
  GPIO_PORTE_PCTL_R  &= ~0x00FFFFFF;        // No selecciona ninguna función alternaticas
  GPIO_PORTE_DIR_R   |= 0x03;               // Configura como salidas
	GPIO_PORTE_DR8R_R  |= 0x03;               // Habilita el driver de 8 mA sobre PE1-0
  GPIO_PORTE_DEN_R   |= 0x03;               // Habilita la funcion digital sobre PE1-0
  GPIO_PORTE_DATA_R  &= ~0x03;              // Apaga todo por defecto
}

void DAC_Out(unsigned long data){
	data &= ~0xFFFFFC; 
  GPIO_PORTE_DATA_R = Tensiones[data]; 
}
/*************************************************************************************/