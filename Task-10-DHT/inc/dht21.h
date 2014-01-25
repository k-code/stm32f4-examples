/*
 * dht.h
 *
 *  Created on: Jan 3, 2014
 *      Author: kvv
 */

#ifndef DHT21_H_
#define DHT21_H_

#include "stm32f4xx.h"

#define DHT_OK		0	// OK
#define DHT_ERR_CON	1	// DHT is not connected
#define DHT_ERR_DEL	2	// DHT time out
#define DHT_ERR_SUM	3	// error control sum

#define DHT_C 0
#define DHT_F 1

#define DHT21_PIN GPIO_Pin_5
#define DHT21_GPIO GPIOA
#define DHT21_BUS RCC_AHB1Periph_GPIOA

void DHT21_init();
uint8_t DHT21_read(uint8_t s, int16_t *t, uint16_t *h); // TODO : the first byte is whole number the second byte is fractional part

#endif /* DHT21_H_ */
