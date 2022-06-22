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
#include <stdio.h>

#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include <math.h>
#include "DAC.h"
#include "Timer.h"
#include "LogicGame.h"

const uint32_t vel_level[] = {1000, 150, 125, 100, 150, 100};
uint8_t level = 0;
int score = 0;
int vidas = 3;
int vidaAnterior = 3;
int historyRecord = 0;
char nickName[3] = {'-', '-', '-'};
int stop = 0;
struct bala
{
	int isCrashed;
	int isVisible;
	int x;
	int y;
} bala1 = {0, 0, 15, 30},
  bala2 = {0, 0, 30, 30},
  bala3 = {0, 0, 30, 30};

struct enemigo
{
	int isCrashed;
	int isVisible;
	int x;
	int y;
} enemigo1 = {0, 0, 64, 47},
  enemigo2 = {0, 0, 64, 29},
  enemigo3 = {0, 0, 64, 8};

struct player
{
	char nickName[3];
	int puntaje;
} player1 = {"PL1", 0},
  player2 = {"PL2", 0},
  player3 = {"PL3", 0};

int count = 0;
struct carro
{
	int x;
	int y;
} carro = {2, 47};

int aleatorio = 0;
int aleatorio_2 = 0;
int aleatorio_3 = 0;
int i = 0;
int tiempo = 0;
int disparar = 0;

void SysTick_Handler()
{
	if (!stop)
	{
		tiempo++;
		i++;
		if (i % 11 == 0)
		{
			srand(tiempo);
		}
		aleatorio = rand() % 3;
		if (tiempo % 11 == 0)
		{
			srand(i + 4);
		}
		aleatorio_2 = rand() % 3;
		if (i % 6 == 0)
		{
			srand(i + 10);
		}
		aleatorio_3 = rand() % 3;
		aleatorio = (aleatorio + aleatorio_2 + aleatorio_3) % 3;
		if (tiempo % (vel_level[level] / 2) == 0)
		{
			if (bala1.isVisible)
			{
				bala1.x = bala1.x + 10;
			}
			if (bala1.x > 80)
			{
				bala1.x = 30;
				bala1.isVisible = 0;
			}
		}
		if (i >= 739)
		{
			i = 0;
		}
		if (tiempo >= vel_level[level])
		{
			if (enemigo1.x < 20)
			{
				enemigo1.isVisible = 0;
			}
			else
			{
				enemigo1.x = enemigo1.x - 10;
			}
			if (enemigo2.x < 20)
			{
				enemigo2.isVisible = 0;
			}
			else
			{
				enemigo2.x = enemigo2.x - 10;
			}
			if (enemigo3.x < 20)
			{
				enemigo3.isVisible = 0;
			}
			else
			{
				enemigo3.x = enemigo3.x - 10;
			}
			score = score + 7;
			tiempo = 0;
			count++;
			if (count == 4)
			{
				count = 0;
			}
		}
		if (count == 3)
		{
			count = 0;
			if (aleatorio == 0 && !enemigo1.isVisible)
			{
				enemigo1.isVisible = 1;
				enemigo1.x = 64;
			}
			if (aleatorio == 1 && !enemigo2.isVisible)
			{
				enemigo2.isVisible = 1;
				enemigo2.x = 64;
			}
			if (aleatorio == 2 && !enemigo3.isVisible)
			{
				enemigo3.isVisible = 1;
				enemigo3.x = 64;
			}
		}
	}
}

void showNumberOfHearts()
{
	int a;
	Nokia5110_SetCursor(0, 1);
	Nokia5110_OutString("Te quedan");
	Nokia5110_SetCursor(0, 2);
	Nokia5110_OutString("3 vidas");
	a = RxCar();
	enemigo1.x = 64;
	enemigo2.x = 64;
	enemigo3.x = 64;
}

void showRecords()
{
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
	char str1[32];
	char str2[32];
	char str3[32];
	int d;
	if (player1.puntaje > player2.puntaje && player1.puntaje > player3.puntaje)
	{
		sprintf(str1, "%c%c%c: %d", player1.nickName[0], player1.nickName[1], player1.nickName[2], player1.puntaje);
		if (player2.puntaje > player3.puntaje)
		{
			sprintf(str2, "%c%c%c: %d", player2.nickName[0], player2.nickName[1], player2.nickName[2], player2.puntaje);
			sprintf(str3, "%c%c%c: %d", player3.nickName[0], player3.nickName[1], player3.nickName[2], player3.puntaje);
		}
		else
		{
			sprintf(str2, "%c%c%c: %d", player2.nickName[0], player2.nickName[1], player2.nickName[2], player2.puntaje);
			sprintf(str3, "%c%c%c: %d", player3.nickName[0], player3.nickName[1], player3.nickName[2], player3.puntaje);
		}
	}
	if (player2.puntaje > player1.puntaje && player2.puntaje > player3.puntaje)
	{
		sprintf(str1, "%c%c%c: %d", player2.nickName[0], player2.nickName[1], player2.nickName[2], player2.puntaje);
		if (player1.puntaje > player3.puntaje)
		{
			sprintf(str2, "%c%c%c: %d", player1.nickName[0], player1.nickName[1], player1.nickName[2], player1.puntaje);
			sprintf(str3, "%c%c%c: %d", player3.nickName[0], player3.nickName[1], player3.nickName[2], player3.puntaje);
		}
		else
		{
			sprintf(str2, "%c%c%c: %d", player2.nickName[0], player2.nickName[1], player2.nickName[2], player2.puntaje);
			sprintf(str3, "%c%c%c: %d", player3.nickName[0], player3.nickName[1], player3.nickName[2], player3.puntaje);
		}
	}
	if (player3.puntaje > player1.puntaje && player3.puntaje > player2.puntaje)
	{
		sprintf(str1, "%c%c%c: %d", player3.nickName[0], player3.nickName[1], player3.nickName[2], player3.puntaje);
		if (player1.puntaje > player2.puntaje)
		{
			sprintf(str2, "%c%c%c: %d", player1.nickName[0], player1.nickName[1], player1.nickName[2], player1.puntaje);
			sprintf(str3, "%c%c%c: %d", player2.nickName[0], player2.nickName[1], player2.nickName[2], player2.puntaje);
		}
		else
		{
			sprintf(str2, "%c%c%c: %d", player2.nickName[0], player2.nickName[1], player2.nickName[2], player2.puntaje);
			sprintf(str3, "%c%c%c: %d", player1.nickName[0], player1.nickName[1], player1.nickName[2], player1.puntaje);
		}
	}
	// int b= sprintf(str3,"%c%c%c: %d \r\n",'a','a','a',score);
	int a;
	Nokia5110_SetCursor(2, 0);
	Nokia5110_OutString("Records");
	Nokia5110_SetCursor(2, 2);
	Nokia5110_OutString(str1);
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString(str2);
	Nokia5110_SetCursor(2, 4);
	Nokia5110_OutString(str3);
	a = RxCar();
}

void showCongratulation()
{
	int a;
	Nokia5110_SetCursor(0, 1);
	Nokia5110_OutString("Mejor");
	Nokia5110_SetCursor(0, 3);
	Nokia5110_OutString("Puntaje");
	Nokia5110_SetCursor(0, 4);
	Nokia5110_OutString(":3");
	a = RxCar();
}
void evaluarChoque()
{
	if (enemigo1.isVisible == 1 && enemigo1.x == 24 && enemigo1.y == carro.y)
	{
		vidas--;
	}
	if (enemigo2.isVisible == 1 && enemigo2.x == 24 && enemigo2.y == carro.y)
	{
		vidas--;
	}
	if (enemigo3.isVisible == 1 && enemigo3.x == 24 && enemigo3.y == carro.y)
	{
		vidas--;
	}
}
int main(void)
{
	ConfiguraSysTick();
	Nokia5110_Init();
	Nokia5110_Clear();
	ConfigUART0();
	int mayorPuntaje = 0;
	int menorPuntaje = 0;
	int jugada = 1;
	/*Posicion X Y del personaje */

	// const unsigned char *numbersBMP[10][133]={zero,one,two,three,four,five,six,seven,eight,nine};
	int points_y = 4;
	int points_x = 73;
	stop = 0;

	while (1)
	{
		logoJuego();
		setName(nickName);
		setLevel(&level);
		Nokia5110_DisplayBuffer();
		while (vidas > 0)
		{

			Nokia5110_ClearBuffer();
			// evaluarMovimiento(&carro.y);
			evaluarMovimientoDisparo(&carro.y, &bala1.isVisible, &bala1.y);
			Nokia5110_PrintBMP(carro.x, carro.y, Rocket, 2);
			showScore(score);
			if (enemigo1.isVisible)
				Nokia5110_PrintBMP(enemigo1.x, enemigo1.y, Enemigo, 2);
			if (enemigo2.isVisible)
				Nokia5110_PrintBMP(enemigo2.x, enemigo2.y, Enemigo, 2);
			if (enemigo3.isVisible)
				Nokia5110_PrintBMP(enemigo3.x, enemigo3.y, Enemigo, 2);
			if (bala1.isVisible)
			Nokia5110_PrintBMP(bala1.x, bala1.y, Bala, 2);
			Nokia5110_DisplayBuffer();
			Nokia5110_SetCursor(0, 1);
			Nokia5110_OutString("__________");
			Nokia5110_SetCursor(0, 4);
			Nokia5110_OutString("----------");

			if (vidas != vidaAnterior)
			{
				Nokia5110_ClearBuffer();
				Nokia5110_DisplayBuffer();
				carro.y = 47;
				stop = 1;
				showNumberOfHearts();
				stop = 0;
				vidaAnterior = vidas;
			}
			else
			{
				evaluarChoque();
			}
			// if(carro.y==8)vidas--;
			for (int o = 0; o < 80000; o++)
			{
			}
		}
		if (score > historyRecord)
		{
			Nokia5110_ClearBuffer();
			Nokia5110_DisplayBuffer();
			showCongratulation();
			historyRecord = score;
		}

		if (jugada < 4)
		{
			for (int u = 0; u < 3; u++)
			{
				if (jugada == 1)
				{
					player1.nickName[u] = nickName[u];
					player1.puntaje = score;
				}
				if (jugada == 2)
				{
					player2.nickName[u] = nickName[u];
					player2.puntaje = score;
				}
				if (jugada == 3)
				{
					player3.nickName[u] = nickName[u];
					player3.puntaje = score;
				}
			}
		}
		else
		{
			int puntajes[3] = {player1.puntaje, player2.puntaje, player3.puntaje};
			menorPuntaje = calculaMenor(puntajes);
			if (score > menorPuntaje)
			{
				if (menorPuntaje == player1.puntaje)
				{
					for (int u = 0; u < 3; u++)
					{
						player1.nickName[u] = nickName[u];
						player1.puntaje = score;
					}
				}
				else
				{
					if (menorPuntaje == player2.puntaje)
					{
						for (int k = 0; k < 3; k++)
						{
							player2.nickName[k] = nickName[k];
							player2.puntaje = score;
						}
					}
					else
					{
						if (menorPuntaje == player3.puntaje)
						{
							for (int l = 0; l < 3; l++)
							{
								player3.nickName[l] = nickName[l];
								player3.puntaje = score;
							}
						}
					}
				}
			}
		}
		jugada == 4 ? jugada = 4 : jugada++;
		showRecords();
		vidas = 3;
		score = 0;
		nickName[0] = '-';
		nickName[1] = '-';
		nickName[2] = '-';
	}
}
