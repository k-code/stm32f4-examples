#include "main.h"
#include "dht21.h"
#include "time.h"

void delay(uint32_t ms) {
    ms *= 8000;
    while(ms--) {
        __NOP();
    }
}

int main() {
    init();
    DHT21_init();
    TIME_init();

    do {
        loop();
    } while (1);
}

void init() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);
}

void loop() {
    TIME_delay(2000);
	GPIO_ResetBits(GPIOD, LEDS);

	int16_t temp = 0;
	uint16_t hum = 0;
	uint8_t err = DHT21_read(DHT_C, &temp, &hum), i = 0, j = 0;
	temp = temp / 10;
	hum = hum / 10;

	if (err != DHT_OK) {
		if (err == DHT_ERR_CON) {
			j = 2;
		} else if (err == DHT_ERR_DEL) {
			j = 3;
		} else if (err == DHT_ERR_SUM) {
			j = 4;
		}
		for (i = 0; i < j; i++) {
			GPIO_SetBits(GPIOD, LED[0]);
			delay(250);
			GPIO_ResetBits(GPIOD, LED[0]);
			delay(200);
		}
	}
	else {
		/* temp = (buf[2] * 256 + buf[3]) / 10;*/
		if (temp == 0) {
			GPIO_SetBits(GPIOD, LED[0]);
		} else {
			GPIO_ResetBits(GPIOD, LED[0]);
		}
		if (temp > 10) {
			GPIO_SetBits(GPIOD, LED[1]);
		}
		if (temp > 20) {
			GPIO_SetBits(GPIOD, LED[2]);
		}
		if (temp > 30) {
			GPIO_SetBits(GPIOD, LED[3]);
		}
	}
}
