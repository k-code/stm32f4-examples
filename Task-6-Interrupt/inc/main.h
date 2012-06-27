#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

const u16 LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const u16 LED[4] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
const u16 USER_BUTTON = GPIO_Pin_0;

int main(void);
void init(void);
void loop(void);
void delay(u32);

void initLeds(void);
void initButton(void);
void initExtInterrupt(void);
void initClkInterrupt();
void toggleLed(void);
void switchLed(void);
