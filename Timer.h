

#include "tm4c123gh6pm.h"

/****************************Timer_1ms************************************************/
void ConfiguraSysTick(void){		// Temporiza 1mseg, Fclksys=16MHz	
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
	NVIC_ST_RELOAD_R = (NVIC_ST_RELOAD_R&0xFF000000)|0x00003E7F;
	NVIC_ST_CURRENT_R &= ~(0x00FFFFFF);
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE;
}


/*************************************************************************************/