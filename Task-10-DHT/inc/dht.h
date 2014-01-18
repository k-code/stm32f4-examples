/*
 * dht.h
 *
 *  Created on: Jan 3, 2014
 *      Author: kvv
 */

#ifndef DHT_H_
#define DHT_H_

#include "stm32f4xx.h"

#define DHT_DHT11 11
#define DHT_DHT21 21
#define DHT_DHT22 22
#define DHT_AM2301 DHT_DHT21

#define DHT_OK		0	// OK
#define DHT_ERR_CON	1	// DHT is not connected
#define DHT_ERR_DEL	2	// DHT time out
#define DHT_ERR_SUM	3	// error control sum

#define DHT_C 0
#define DHT_F 1


typedef struct DHT {
  uint8_t data[5];
  uint8_t _type;
} _DHT;

void DHT_init(uint8_t type);
uint8_t DHT_read(uint8_t s, uint16_t *t, uint16_t *h); // TODO : the first byte is whole number the second byte is fractional part

#endif /* DHT_H_ */
