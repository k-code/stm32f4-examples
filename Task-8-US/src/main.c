#include "main.h"

int main() {
    init();

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
    gpio.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOD, &gpio);

    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &gpio);
}

void delay_us(uint32_t us) {
    us *= 3;
    while(us--) {
        __NOP();
    }
}

uint32_t timing_1() {
  GPIO_ResetBits(GPIOA, GPIO_Pin_2);
  delay_us(2);
  GPIO_SetBits(GPIOA, GPIO_Pin_2);
  delay_us(10);
  GPIO_ResetBits(GPIOA, GPIO_Pin_2);

  uint32_t duration = 0;
  while ( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == RESET && duration < 200) {
      duration++;
      delay_us(1);
  };
  duration = 0;
  while ( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == SET ) {
      delay_us(1);
      duration++;
  }
  return duration;
}

void loop() {
    uint32_t duration = timing_1();
    if (duration > 30000) {
        return;
    }
    uint32_t distacne_cm = duration /29 / 2 ;

    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    delay(distacne_cm * 20);
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    delay(distacne_cm * 20);
}


void delay(uint32_t ms) {
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}
