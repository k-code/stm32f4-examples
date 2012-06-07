#include "main.h"

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
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);
}

void loop() {
    i++;
    switch(i) {
        case 10: slowMode(); break;
        case 15: defaultMode(); break;
        case 25: fastMode(); break;
        case 40: i=0; break;
    }

    GPIO_ToggleBits(GPIOD, LEDS);

    delay(300);
}

void delay(u32 ms) {
    ms *= 1000;
    while(ms--) {
        __NOP();
    }
}

void slowMode() {
    RCC_DeInit();

    RCC_HSEConfig(RCC_HSE_ON);
    if (RCC_WaitForHSEStartUp() == ERROR) {
        return;
    }

    RCC_PLLConfig(RCC_PLLSource_HSE, 8, 192, 8, 15);
    RCC_PLLCmd(ENABLE);

    RCC_WaitForPLLSturtUp();

    RCC_HCLKConfig(RCC_SYSCLK_Div16);
    RCC_PCLK1Config(RCC_HCLK_Div16);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}

void defaultMode() {
    RCC_DeInit();
}

void fastMode() {
    RCC_DeInit();

    RCC_HSEConfig(RCC_HSE_ON);
    if (RCC_WaitForHSEStartUp() == ERROR) {
        return;
    }
    RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 15);
    RCC_PLLCmd(ENABLE);

    RCC_WaitForPLLSturtUp();

    RCC_HCLKConfig(RCC_SYSCLK_Div4);
    RCC_PCLK1Config(RCC_HCLK_Div1);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}
