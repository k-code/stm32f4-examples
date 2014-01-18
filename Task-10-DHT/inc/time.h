/*
 * time.h
 *
 *  Created on: Apr 21, 2013
 *      Author: kvv
 */

#ifndef TIME_H_
#define TIME_H_

#include "stm32f4xx.h"

uint32_t TIME_nano;

void TIME_init();
void TIME_delay(uint32_t ms);
void TIME_delayNano(uint32_t nano);
void SysTick_Handler(void);

#endif /* TIME_H_ */
