#include "stm32f4xx.h"
#include "startup.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
uint32_t i = 1;

void init();
void loop();

void delay();

void slowMode();
void defaultMode();
void fastMode();
