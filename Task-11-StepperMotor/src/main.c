#include "main.h"

const uint8_t steps[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
uint32_t i = 0;

int main() {
    init();

    do {
        loop();
    } while (1);
}

void init() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LED;
    GPIO_Init(GPIOD, &gpio);
}

void loop() {
	uint8_t j;
	for (j = 0; j < 4; j++) {
		if (steps[i % 4][j] == 0) {
			GPIO_ResetBits(GPIOD, LEDS[j]);
		} else {
			GPIO_SetBits(GPIOD, LEDS[j]);
		}
	}
    delay(10);
    i++;
}

void delay(uint32_t ms) {
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}
