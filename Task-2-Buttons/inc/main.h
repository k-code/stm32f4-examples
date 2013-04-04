#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

const uint16_t LEDS = GPIO_Pin_1;
const uint16_t LED = GPIO_Pin_1;
const uint16_t USER_BUTTON = GPIO_Pin_0;

void init();
void loop();

void delay();

void initLeds();
void initButton();
