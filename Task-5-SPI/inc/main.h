/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

typedef enum {FALSE = 0, TRUE = !FALSE} boolean;

const u16 LED_Pins = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const u16 LED1 = GPIO_Pin_12;
const u16 LED2 = GPIO_Pin_13;
const u16 LED3 = GPIO_Pin_14;
const u16 LED4 = GPIO_Pin_15;
const u32 LIS302DL_FLAG_TIMEOUT = 0x1000;

int main(void);
void LED_Init(void);
void SPI1_Init(void);
void delay(int i);
void LIS302DL_Init(void);
u8 LIS302DL_SendByte(u8 byte);
u8 LIS302DL_GetByte(void);
void LIS302DL_Write(u8 REG, u8 *DATA, u8 count);
void LIS302DL_Read(u8 REG, u8 *DATA, u8 count);
void LIS302DL_ReadACC(int32_t* out);

#endif /* __MAIN_H */
