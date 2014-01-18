/*
 * time.c
 *
 *  Created on: Apr 21, 2013
 *      Author: kvv
 */

#include "time.h"
#include "stm32f4xx_rcc.h"

void TIME_init() {
	TIME_nano = 0;

    RCC_ClocksTypeDef RCC_Clocks;
    /* SysTick end of count event each 1ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000000);
	NVIC_SetPriority(SysTick_IRQn, 1);
}

void TIME_delay(uint32_t ms) {
	TIME_delayNano(ms*1000);
}

void TIME_delayNano(uint32_t nano) {
	uint32_t deadLine = TIME_nano + nano;
	while (deadLine > TIME_nano) {
		__NOP();
	}
}

void SysTick_Handler(void) {
	TIME_nano++;
}
