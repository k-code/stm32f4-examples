#include "main.h"

static uint8_t lastButtonStatus = RESET;
static uint16_t currentLed = 0;

int main() {
    init();

    do {
        loop();
    } while (1);
}

void init() {
    initLeds();
    initButton();
}

void loop() {
    uint8_t currentButtonStatus = GPIO_ReadInputDataBit(GPIOA, USER_BUTTON);

    if (lastButtonStatus != currentButtonStatus && currentButtonStatus != RESET) {
        GPIO_ToggleBits(GPIOA, LED);
    }
    lastButtonStatus = currentButtonStatus;
}

void initLeds() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOA, &gpio);
}

void initButton() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Pin = USER_BUTTON;
    GPIO_Init(GPIOA, &gpio);
}

void delay(uint32_t ms) {
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}
