/*
 * dth.c
 *
 *  Created on: Jan 3, 2014
 *      Author: kvv
 */

#include "dht21.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "time.h"

static void pinModeWrite();
static void pinModeRead();
static int16_t convertCtoF(int16_t c);
static uint8_t read(uint8_t *data);

void DHT21_init() {
	// set up the pins!
	RCC_AHB1PeriphClockCmd(DHT21_BUS, ENABLE);
	pinModeRead();
}

uint8_t DHT21_read(uint8_t S, int16_t *t, uint16_t *h) {
	int16_t _t=0;
	uint16_t _h =0;
	uint8_t data[5];

	uint8_t err = read(data);
	if (err == DHT_OK) {
		_h = data[0];
		_h <<= 8;
		_h |= data[1];
		_t = data[2] & 0x7F;
		_t <<= 8;
		_t |= data[3];
		if (data[2] & 0x80) {
			_t = 0 - _t;
		}
		if (S == DHT_F) {
			_t= convertCtoF(_t);
		}
	}
	*t = _t;
	*h = _h;
	return err;
}

static int16_t convertCtoF(int16_t c) {
	return c * 9 / 5 + 32;
}

// See http://meteobox.tk/files/AM2301.pdf
static uint8_t read(uint8_t *data) {
	uint8_t laststate = Bit_SET;
	uint8_t i = 0, j = 0;
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	pinModeWrite();
	GPIO_ResetBits(DHT21_GPIO, DHT21_PIN);
	// pull it high for ~1 ms (Tbe)
	TIME_delay(1);

	// pull it high for ~30 us (Tgo)
	GPIO_SetBits(DHT21_GPIO, DHT21_PIN);
	TIME_delayNano(30);
	pinModeRead();

	// wait low signal ~ 20 us (Tgo)
	for (i = 0; i < 10 && laststate == Bit_SET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		TIME_delayNano(1);
	}
	if (laststate == Bit_SET) {
		return DHT_ERR_CON;
	}

	// wait high signal ~ 85 us (Trel)
	TIME_delayNano(75);
	for (i = 0; i < 10 && laststate == Bit_RESET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		TIME_delayNano(1);
	}
	if (laststate == Bit_RESET) {
		return DHT_ERR_DEL;
	}

	// wait low signal ~85 us (Treh)
	TIME_delayNano(75);
	for (i = 0; i < 10 && laststate == Bit_SET; i++) {
		laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
		TIME_delayNano(1);
	}
	if (laststate == Bit_SET) {
		return DHT_ERR_DEL;
	}

	// read 40 bits (5 byte)
	for (j = 0; j < 40; j++) {
		// wait high signal ~55 us (Tlow)
		TIME_delayNano(48);
		for (i = 0; i < 17 && laststate == Bit_RESET; i++) {
			laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
			TIME_delayNano(1);
		}
		if (laststate == Bit_RESET) {
			return DHT_ERR_DEL;
		}

		// wait low signal ~26 us (Th0)
		TIME_delayNano(22);
		for (i = 0; i < 8 && laststate == Bit_SET; i++) {
			laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
			TIME_delayNano(1);
		}
		// seek to next bit
		data[j / 8] <<= 1;
		if (laststate == Bit_RESET) {
			continue;
		}

		// wait low signal ~40 us (Th1)
		TIME_delayNano(38);
		for (i = 0; i < 12 && laststate == Bit_SET; i++) {
			laststate = GPIO_ReadInputDataBit(DHT21_GPIO, DHT21_PIN);
			TIME_delayNano(1);
		}
		if (laststate == Bit_SET) {
			return DHT_ERR_DEL;
		}
		data[j / 8] |= 1;
	}

	// check we read 40 bits and that the checksum matches
	if ((j == 40)
			&& (data[4]
					== ((data[0] + data[1] + data[2] + data[3])
							& 0xFF))) {
		return DHT_OK;
	}

	return DHT_ERR_SUM;
}

static void pinModeWrite() {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT21_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(DHT21_GPIO, &GPIO_InitStructure);

}

static void pinModeRead() {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT21_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(DHT21_GPIO, &GPIO_InitStructure);
}
