/*****************************Funciones para el DAC************************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Uart.h"
#include "Assets.h"
#include "Nokia5110.h"
uint32_t timer = 0;
uint8_t carril = 2;
const uint8_t selector_x[] = {-10, 4, 29, 53, 4, 29};
const uint8_t selector_y[] = {-10, 30, 30, 30, 47, 47};
const uint8_t pos_carro_y[] = {47, 29, 8};
struct player
{
	char nickName[3];
	int puntaje;
} player1 = {"PL1", 0},
  player2 = {"PL2", 0},
  player3 = {"PL3", 0};
	
struct enemigo
{
	int isCrashed;
	int isVisible;
	int x;
	int y;
} enemigo1 = {0, 0, 64, 47},
  enemigo2 = {0, 0, 64, 29},
  enemigo3 = {0, 0, 64, 8};
void actualizarTimer()
{
	if ((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT))
	{
		// cada vez que pase 1ms, el contador de tiempo se incrementa
		timer++;
	}
}

void setLevel(uint8_t *nivel)
{
	char lectura = 0x00;
	int i;
	Nokia5110_Clear();
	Nokia5110_OutString("Seleccione");
	Nokia5110_SetCursor(0, 1);
	Nokia5110_OutString("un nivel");
	Nokia5110_SetCursor(0, 2);
	Nokia5110_OutString("(1)(2)(3)");
	Nokia5110_SetCursor(0, 4);
	Nokia5110_OutString("(4)(5)");
	(*nivel) = 1;
	while (lectura != ' ')
	{
		lectura = UART0_DR_R;
		if (lectura == 'd')
		{
			(*nivel) = (*nivel) + 1;
			if ((*nivel) > 5)
			{
				(*nivel) = 1;
			}
			for (i = 0; i < 30000; i++)
			{
			}
		}
		else if (lectura == 'a')
		{
			(*nivel) = (*nivel) - 1;
			if ((*nivel) < 1)
			{
				(*nivel) = 5;
			}
			for (i = 0; i < 30000; i++)
			{
			}
		}
		Nokia5110_ClearBuffer();
		Nokia5110_PrintBMP(selector_x[(*nivel)], selector_y[(*nivel)], Selector, 2);
		Nokia5110_DisplayBuffer();
		Nokia5110_OutString("Seleccione");
		Nokia5110_SetCursor(0, 1);
		Nokia5110_OutString("un nivel");
		Nokia5110_SetCursor(0, 2);
		Nokia5110_OutString("(1)(2)(3)");
		Nokia5110_SetCursor(0, 4);
		Nokia5110_OutString("(4)(5)");
		for (i = 0; i < 80000; i++)
		{
		}
	}
}

void setName(char nickName[])
{
	uint16_t letter;
	uint8_t i = 0;
	while (i <= 3)
	{
		Nokia5110_ClearBuffer();
		Nokia5110_DisplayBuffer();
		Nokia5110_OutString("Ingrese");
		Nokia5110_SetCursor(0, 1);
		Nokia5110_OutString("NickName");
		Nokia5110_SetCursor(0, 2);
		Nokia5110_SetCursor(10, 3);
		Nokia5110_OutString("'");
		Nokia5110_OutString(nickName);
		Nokia5110_OutString("'");
		if (i == 3)
		{
			Nokia5110_SetCursor(0, 4);
			Nokia5110_OutString("Press to");
			Nokia5110_SetCursor(0, 5);
			Nokia5110_OutString("continue");
		}
		letter = RxCar();
		nickName[i] = letter;
		i++;
	}
}

void logoJuego()
{
	char lectura = 0x00;
	Nokia5110_ClearBuffer();
	Nokia5110_DrawFullImage(SplashScreen);
	Nokia5110_DisplayBuffer();
	Nokia5110_DrawFullImage(SplashScreen);
	while (1)
	{
		lectura = UART0_DR_R;
		if (lectura > 0)
		{
			break;
		}
	}
}

void evaluarMovimiento(int *y)
{

	char lectura = UART0_DR_R;

	if (lectura == 'w')
	{
		carril = carril + 1;
	}

	if (lectura == 's')
	{
		carril = carril - 1;
	}

	if (carril > 3)
	{
		carril = 3;
	}
	if (carril < 1)
	{
		carril = 1;
	}
	*y = pos_carro_y[carril - 1];
}

void evaluarMovimientoDisparo(int *y, int *balaVisible, int *balaY, int *stop, int *reset)
{

	char lectura = UART0_DR_R;

	if (lectura == 'w')
	{
		carril = carril + 1;
	}

	if (lectura == 's')
	{
		carril = carril - 1;
	}

	if (carril > 3)
	{
		carril = 3;
	}
	if (carril < 1)
	{
		carril = 1;
	}
	if (lectura == 't')
	{
		(*balaVisible) = 1;
		(*balaY) = (*y) - 3;
	}
	*y = pos_carro_y[carril - 1];

	if (lectura == 'p')
	{
		(*stop) = !(*stop);
	}
	if (lectura == '0')
	{
		(*reset) = !(*reset);
	}
}
const unsigned char *convertNumberToMatrix(int number)
{
	if (number == 0)
	{
		return zero;
	}
	if (number == 1)
	{
		return one;
	}
	if (number == 2)
	{
		return two;
	}
	if (number == 3)
	{
		return three;
	}
	if (number == 4)
	{
		return four;
	}
	if (number == 5)
	{
		return five;
	}
	if (number == 6)
	{
		return six;
	}
	if (number == 7)
	{
		return seven;
	}
	if (number == 8)
	{
		return eight;
	}
	if (number == 9)
	{
		return nine;
	}
	return zero;
}
void showScore(int number)
{
	// const unsigned char numbersBMP[10][133]={*zero,*one,*two,*three,*four,*five,*six,*seven,*eight,*nine};
	int points_y = 4;
	int points_x = 73;
	int digitos[4] = {0, 0, 0, 0};
	int digito = 0, n = 0;
	while (number != 0 && number > 0)
	{
		digito = number % 10;
		number = number / 10;
		digitos[n] = digito;
		n++;
	}
	// 4321
	Nokia5110_PrintBMP(points_x, points_y, convertNumberToMatrix(digitos[3]), 0);
	Nokia5110_PrintBMP(points_x + 3, points_y, convertNumberToMatrix(digitos[2]), 0);
	Nokia5110_PrintBMP(points_x + 6, points_y, convertNumberToMatrix(digitos[1]), 0);
	Nokia5110_PrintBMP(points_x + 9, points_y, convertNumberToMatrix(digitos[0]), 0);
}

void showHistoryRecord(int number){
	int points_y = 4;
	int points_x = 58;
	int digitos[4] = {0, 0, 0, 0};
	int digito = 0, n = 0;
	while (number != 0 && number > 0)
	{
		digito = number % 10;
		number = number / 10;
		digitos[n] = digito;
		n++;
	}
	// 4321
	Nokia5110_PrintBMP(points_x, points_y, convertNumberToMatrix(digitos[3]), 0);
	Nokia5110_PrintBMP(points_x + 3, points_y, convertNumberToMatrix(digitos[2]), 0);
	Nokia5110_PrintBMP(points_x + 6, points_y, convertNumberToMatrix(digitos[1]), 0);
	Nokia5110_PrintBMP(points_x + 9, points_y, convertNumberToMatrix(digitos[0]), 0);
}
// Funci�n calculaMayor:
//  entrada: Arreglo[] es un arreglo , tamanoArreglo es el tama�o del arreglo
//  salida: el mayor valor del arreglo
int calculaMayor(int Arreglo[])
{
	int mayor, i;
	mayor = Arreglo[0];
	for (i = 1; i < 3; i++)
	{
		if (Arreglo[i] > mayor)
			mayor = Arreglo[i];
	}
	return mayor;
}
// Funci�n calculaMenor:
//  entrada: Arreglo[] es un arreglo , tamanoArreglo es el tama�o del arreglo
//  salida: el menor valor del arreglo
int calculaMenor(int Arreglo[])
{
	int menor, i;
	menor = Arreglo[0];
	for (i = 1; i < 3; i++)
	{
		if (Arreglo[i] < menor)
			menor = Arreglo[i];
	}
	return menor;
}

void ordena(int arr_entrada[], int arr_ordena[])
{
	uint8_t menor = 0;
	uint8_t y = 0;
	uint8_t t = 1;
	while (t <= 3)
	{
		menor = calculaMenor(arr_entrada);
		for (int i = 0; i < 3; i++)
		{
			if (menor == arr_entrada[i])
			{
				arr_ordena[y] = arr_entrada[i];
				arr_entrada[i] = 10000;
				y++;
			}
		}
		t++;
	}
}

void showNumberOfHearts(int *vidas)
{
	if (*vidas == 3)
	{
		Nokia5110_PrintBMP(78, 47, Corazon, 0);
		Nokia5110_PrintBMP(72, 47, Corazon, 0);
		Nokia5110_PrintBMP(66, 47, Corazon, 0);
	}
	else if (*vidas == 2)
	{
		Nokia5110_PrintBMP(78, 47, Corazon, 0);
		Nokia5110_PrintBMP(72, 47, Corazon, 0);
	}
	else if (*vidas== 1)
	{
		Nokia5110_PrintBMP(78, 47, Corazon, 0);
	}
}

void mostrarCamino(){
	Nokia5110_DisplayBuffer();
				Nokia5110_SetCursor(0, 1);
				Nokia5110_OutString("__________");
				Nokia5110_SetCursor(0, 4);
				Nokia5110_OutString("----------");
}

void showCongratulation()
{
	int a;
	Nokia5110_SetCursor(0, 1);
	Nokia5110_OutString("Mejor");
	Nokia5110_SetCursor(0, 3);
	Nokia5110_OutString("Puntaje");
	Nokia5110_SetCursor(0, 4);
	Nokia5110_OutString(":D");
	a = RxCar();
}

void evaluateCongratulation(int *score,int *historyRecord){
		if (*score > (*historyRecord))
		{
			Nokia5110_ClearBuffer();
			Nokia5110_DisplayBuffer();
			showCongratulation();
			(*historyRecord) = (*score);
		}
}



	
	void evaluarChoque(
	int *vidas, 
	struct enemigo enemigo1,
	struct enemigo enemigo2,
	struct enemigo enemigo3,
	int carroy
	)
{
	if (enemigo1.isVisible == 1 && enemigo1.x == 16 && enemigo1.y == carroy)
	{
		(*vidas)--;
	}
	if (enemigo2.isVisible == 1 && enemigo2.x == 16 && enemigo2.y == carroy)
	{
		(*vidas)--;
	}
	if (enemigo3.isVisible == 1 && enemigo3.x == 16 && enemigo3.y == carroy)
	{
		(*vidas)--;
	}
}


void guardarScore2(int jugada,
	struct player player1,
	struct player player2,
	struct player player3, 
	char nickName[], 
	int score,
	int puntajes[],
	int menorPuntaje){
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
			puntajes[0] = player1.puntaje;
			puntajes[1] = player2.puntaje;
			puntajes[2] = player3.puntaje;

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

}
/*
void evaluarDisparo(){
	char lectura2 = UART0_DR_R;
	if(lectura2 == 't'){
		bala1.isVisible=1;
		bala1.y=carro.y;
	}
}
*/

// evaluarDisparo();
