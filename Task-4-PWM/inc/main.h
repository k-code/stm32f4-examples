#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

const uint16_t LEDS = GPIO_Pin_13 | GPIO_Pin_15;;
const uint16_t USER_BUTTON = GPIO_Pin_0;

int brightLed1 = 0;
int brigthLed2 = 5;
int delta = 1;
int lastButtonStatus = RESET;

void init();
void loop();

void delay();

void initButton();
void initLeds();
void initTimer();
void initPWM();
