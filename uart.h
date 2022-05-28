// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Funcion de configuraci�n del UART0
#include <stdint.h>
#include "tm4c123gh6pm.h"
void ConfigUART0(void)
{
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // Habilitamos reloj para el UART0
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // Habilitamos reloj para GPIOD

	UART0_CTL_R &= ~UART_CTL_UARTEN;							  // Se desactiva el UART
	UART0_IBRD_R &= ~0xFFFF;									  // 19200bps
	UART0_IBRD_R |= 1000000UL / 38400;							  // Parte entera
	UART0_FBRD_R &= ~0x3F;										  // 19200bps
	UART0_FBRD_R |= (1000000UL % 38400 + 38400 / 2) * 64 / 38400; // Parte decimal
	// UART0_LCRH_R &= ~UART_LCRH_SPS;       //stick parity
	UART0_LCRH_R &= ~UART_LCRH_WLEN_M;
	UART0_LCRH_R |= UART_LCRH_WLEN_8; // 8 bits de dato
	UART0_LCRH_R |= UART_LCRH_FEN;	  // activa uso de pila FIFO
	UART0_LCRH_R &= ~UART_LCRH_STP2;  // 1 bit de parada
	UART0_LCRH_R |= UART_LCRH_EPS;	  // paridad impar
	UART0_LCRH_R |= UART_LCRH_PEN;	  // con paridad
	UART0_CTL_R |= UART_CTL_RXE;	  // Habilita Rx
	UART0_CTL_R |= UART_CTL_TXE;	  // Habilita Tx
	UART0_CTL_R &= ~UART_CTL_LBE;	  // Deshabilita loopback
	UART0_CTL_R &= ~UART_CTL_HSE;	  // Deshabilita high speed
	UART0_CTL_R |= UART_CTL_UARTEN;	  // Habilita UART
	//
	GPIO_PORTA_DIR_R |= (1 << 1);				  // PA1 como salida
	GPIO_PORTA_DIR_R &= ~(1 << 0);				  // PA0 como entrada
	GPIO_PORTA_AMSEL_R &= ~((1 << 1) | (1 << 0)); // Deshabilita funciones anal�gicas
	GPIO_PORTA_AFSEL_R |= ((1 << 1) | (1 << 0));  // Activa funci�n alternativa
	GPIO_PORTA_PCTL_R &= ~((0xF << (1 * 4)) | (0xF << (0 * 4)));
	GPIO_PORTA_PCTL_R |= ((0x1 << (1 * 4)) | (0x1 << (0 * 4))); // U0Rx, U0Tx
	GPIO_PORTA_DEN_R |= ((1 << 1) | (1 << 0));					// Activa funciones digitales
}
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Funci�n que transmite un caracter
void TxCar(uint8_t Car)
{
	while ((UART0_FR_R & UART_FR_TXFF) != 0)
		; // Espera mientras pila llena
	UART0_DR_R = Car;
}
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Funci�n que espera hasta recibir un caracter
uint8_t RxCar(void)
{
	while ((UART0_FR_R & UART_FR_RXFE) != 0)
		; // Espera mientra pila vac�a
	return UART0_DR_R & 0xFF;
}
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Funci�n que verifica el estado de la pila de recepci�n
uint8_t HayCarRx(void)
{
	if ((UART0_FR_R & UART_FR_RXFE) != 0)
		return 0; // pila vac�a
	else
		return 1; // hay caracter recibido
}
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Funcion que transmite una cadena ASCII-Z
void TxCadena(uint8_t Cadena[])
{
	uint8_t i;

	for (i = 0; Cadena[i] != '\0'; i++)
		TxCar(Cadena[i]);
}