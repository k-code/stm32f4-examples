#include "main.h"

static u16 currentLed = 0;
static u32 currentTime = 0;

int main() {
    init();

    do {
        loop();
    } while (1);
}

void init() {
    initLeds();
    initButton();
    initExtInterrupt();
    initClkInterrupt();
}

void loop() {
    __NOP();
}

void delay(u32 ms) {
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}

void initLeds() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);
}

void initButton() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Pin = USER_BUTTON;
    GPIO_Init(GPIOD, &gpio);
}

void initExtInterrupt() {
    EXTI_InitTypeDef   EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void initClkInterrupt() {
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_SetPriority(SysTick_IRQn, 1);
}

void EXTI0_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        switchLed();
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void SysTick_Handler(void) {
    currentTime++;
    toggleLed();
}

void toggleLed(void) {
    if (currentTime % 200 == 0) {
        GPIO_ToggleBits(GPIOD, LED[currentLed]);
    }
}

void switchLed(void) {
    GPIO_ResetBits(GPIOD, LED[currentLed]);

    currentLed++;
    if (currentLed > 3) {
        currentLed = 0;
    }
}
