
#ifndef __STM32F4_DISCOVERY_LIS302DL_H
#define __STM32F4_DISCOVERY_LIS302DL_H

/* Includes ------------------------------------------------------------------*/
 #include "stm32f4xx.h"
 #include "stm32f4xx_gpio.h"
 #include "stm32f4xx_spi.h"
 #include "stm32f4xx_rcc.h"

typedef struct {
    u16 CLK_Pin;
    u16 MISO_Pin;
    u16 MOSI_Pin;
    u16 CS_Pin;
} SPIPIN_struct;

#endif //__STM32F4_DISCOVERY_LIS302DL_H
