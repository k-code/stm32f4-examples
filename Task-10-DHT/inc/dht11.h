/*
 * dht11.h
 *
 *  Created on: Jan 10, 2014
 *      Author: kvv
 */

#ifndef DHT11_H_
#define DHT11_H_

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

/* Exported constants --------------------------------------------------------*/
#define MAX_TICS 10000
#define DHT11_OK 0
#define DHT11_NO_CONN 1
#define DHT11_CS_ERROR 2
#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_Pin_5

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
uint8_t read_DHT11(uint8_t *buf);

#endif /* DHT11_H_ */
