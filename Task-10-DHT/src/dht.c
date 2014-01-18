/*
 * dth.c
 *
 *  Created on: Jan 3, 2014
 *      Author: kvv
 */

#include "dht.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "time.h"

#define MAXTIMINGS 85
#define TRUE 1
#define FALSE 0

// STM32 settings
#define PIN GPIO_Pin_5
#define GPIO GPIOA
#define BUS RCC_AHB1Periph_GPIOA

static void pinModeWrite();
static void pinModeRead();
static uint16_t convertCtoF(uint16_t c);
static uint8_t read(void);
static uint8_t error(uint8_t err);

struct DHT dht;

void DHT_init(uint8_t type) {
	dht._type = type;

	// set up the pins!
	RCC_AHB1PeriphClockCmd(BUS, ENABLE);
	pinModeWrite();
	GPIO_SetBits(GPIO, PIN);
}

//boolean S == Scale.  True == Farenheit; False == Celcius
uint8_t DHT_read(uint8_t S, uint16_t *t, uint16_t *h) {
	uint16_t _t=0, _h =0;

	uint8_t err = read();
	if (err == DHT_OK) {
		switch (dht._type) {
			case DHT_DHT11:
				_h = dht.data[0];
				_t = dht.data[2];
				if (S) {
					_t = convertCtoF(_t);
				}
				break;
			case DHT_DHT21:
			case DHT_DHT22:
				//TODO : fix binary math
				_h = dht.data[0];
				_h *= 256;
				_h += dht.data[1];
				_t = dht.data[2] & 0x7F;
				_t *= 256;
				_t += dht.data[3];
				if (dht.data[2] & 0x80) {
					_t *= -1;
				}
				if (S) {
					_t= convertCtoF(_t);
				}
				break;
		}
	}
	*t = _t;
	*h = _h;
	return err;
}

static uint16_t convertCtoF(uint16_t c) {
	return c * 9 / 5 + 32;
}

static uint8_t read(void) {
	uint8_t laststate = Bit_RESET;
	uint8_t i = 0, j = 0;
	dht.data[0] = dht.data[1] = dht.data[2] = dht.data[3] = dht.data[4] = 0;

	// now pull it low for ~18 ms for DHT11 else ~1 ms
	GPIO_ResetBits(GPIO, PIN);
	TIME_delay(dht._type == DHT_DHT11 ? 18 : 1);

	// pull it high for ~20 us
	GPIO_SetBits(GPIO, PIN);
	TIME_delayNano(20);
	pinModeRead();

	// wait low signal ~ 20 us
	laststate = Bit_SET;
	for (i = 0; i < 20 && laststate == Bit_SET; i++) {
		laststate = GPIO_ReadInputDataBit(GPIO, PIN);
		TIME_delayNano(1);
	}
	if (i == 20) {
		return error(DHT_ERR_CON);
	}

	// wait high signal ~90 us
	for (i = 0; i < 90 && laststate == Bit_RESET; i++) {
		laststate = GPIO_ReadInputDataBit(GPIO, PIN);
		TIME_delayNano(1);
	}
	if (i == 90) {
		return error(DHT_ERR_DEL);
	}

	// wait low signal ~90 us
	for (i = 0; i < 90 && laststate == Bit_SET; i++) {
		laststate = GPIO_ReadInputDataBit(GPIO, PIN);
		TIME_delayNano(1);
	}
	if (i == 90) {
		return error(DHT_ERR_DEL);
	}

	// read in timings
	for (j = 0; j < 40 /*MAXTIMINGS*/; j++) {
		// wait high signal ~70 us
		for (i = 0; i < 70 && laststate == Bit_RESET; i++) {
			laststate = GPIO_ReadInputDataBit(GPIO, PIN);
			TIME_delayNano(1);
		}
		if (i == 70) {
			return error(DHT_ERR_DEL);
		}

		// calculate low signal ~100 us
		for (i = 0; i < 100 && laststate == Bit_SET; i++) {
			laststate = GPIO_ReadInputDataBit(GPIO, PIN);
			TIME_delayNano(1);
		}
		if (i == 100) {
			return error(DHT_ERR_DEL);
		}

		dht.data[j / 8] <<= 1;

		if (i > 30) {
			dht.data[j / 8] |= 1;
		}
	}

	// check we read 40 bits and that the checksum matches
	if ((j >= 40)
			&& (dht.data[4]
					== ((dht.data[0] + dht.data[1] + dht.data[2] + dht.data[3])
							& 0xFF))) {
		return error(DHT_OK);
	}

	return error(DHT_ERR_SUM);
}

static uint8_t error(uint8_t err) {
	pinModeWrite();
	GPIO_SetBits(GPIO, PIN);
	return err;
}

static void pinModeWrite() {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIO, &GPIO_InitStructure);

}

static void pinModeRead() {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIO, &GPIO_InitStructure);
}
