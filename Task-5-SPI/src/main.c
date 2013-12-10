#include "main.h"

int main(void) {
    init();

    u8 id;

    LIS302DL_Read(0x0F, &id, 1);

    while(1) {
        loop();
    }
}

void init(void) {
    initLeds();
    initTimer();
    initPWM();
    initSpi();
    LIS302DL_Init();
}

void loop(void) {
    int32_t data[3];
    LIS302DL_ReadACC(data);

    int32_t x = cround(data[0], 300)/5;
    int32_t y = cround(data[1], 300)/5;


    TIM_SetCompare1(TIM4, 0);
    TIM_SetCompare2(TIM4, 0);
    TIM_SetCompare3(TIM4, 0);
    TIM_SetCompare4(TIM4, 0);

    if ( x > 0 ) {
        TIM_SetCompare2(TIM4, x);
    }
    else {
        TIM_SetCompare4(TIM4, -x);
    }
    if ( y > 0 ) {
        TIM_SetCompare1(TIM4, y);
    }
    else {
        TIM_SetCompare3(TIM4, -y);
    }

    delay(10);
}

void delay(u32 ms) {
    ms *= 1000;
    while (--ms > 0) {
        __NOP();
    }
}

void initLeds() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LEDS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
}

void initSpi(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);

    SPI_InitTypeDef SPI_InitStructure;
    SPI_I2S_DeInit(SPI1);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

void initTimer() {
    /* TIM4 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* Compute the prescaler value */
    u32 PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 21000000) - 1;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}

void initPWM() {
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    /* PWM1 Mode configuration: Channel1 (GPIOD Pin 12)*/
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 (GPIOD Pin 13)*/
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel3 (GPIOD Pin 14)*/
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel4 (GPIOD Pin 15)*/
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_Cmd(TIM4, ENABLE);
}

void LIS302DL_Init() {
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    u8 reg = 0x47;
    LIS302DL_Write(0x20, &reg, 1);
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
}

u8 LIS302DL_SendByte(u8 byte) {
    u32 LIS302DLTimeout = LIS302DL_FLAG_TIMEOUT;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET) {
        if((LIS302DLTimeout--) == 0) return 0x00;
    }

    /* Send a Byte through the SPI peripheral */
    SPI_I2S_SendData(SPI1, byte);

    return LIS302DL_GetByte();
}


u8 LIS302DL_GetByte() {
    u32 LIS302DLTimeout = LIS302DL_FLAG_TIMEOUT;
    /* Wait to receive a Byte */
    LIS302DLTimeout = LIS302DL_FLAG_TIMEOUT;
    while (SPI_I2S_GetFlagStatus(SPI1,  SPI_FLAG_RXNE) == RESET) {
        if((LIS302DLTimeout--) == 0) return 0x00;
    }

    /* Return the Byte read from the SPI bus */
    return (u8)SPI_I2S_ReceiveData(SPI1);
}

void LIS302DL_Write(u8 REG, u8 *DATA, u8 count) {
    int i;

    GPIO_WriteBit(GPIOE, GPIO_Pin_3, RESET);
    LIS302DL_SendByte(REG);
    for (i=0; i < count; i++) {
        LIS302DL_SendByte(*DATA);
        DATA++;
    }
    GPIO_WriteBit(GPIOE, GPIO_Pin_3, SET);
}

void LIS302DL_Read(u8 REG, u8 *DATA, u8 count) {
    u8 i;

    GPIO_WriteBit(GPIOE, GPIO_Pin_3, RESET);
    REG |= 0x80;
    if (count > 1) {
        REG |= 0x40;
    }
    LIS302DL_SendByte(REG);
    for (i=0; i < count; i++) {
        *DATA = LIS302DL_SendByte((u8)0x00);
        DATA++;
    }

    GPIO_WriteBit(GPIOE, GPIO_Pin_3, SET);
}

void LIS302DL_ReadACC(int32_t* out) {
    int i;

    u8 buffer[6];
    LIS302DL_Read(0x29, buffer, 6);

    for(i=0; i<3; i++) {
      *out =(int32_t)(72 * (int8_t)buffer[2*i]);
      out++;
    }
}

void LIS302DL_ReadACCY(int32_t* out) {
    int i;

    u8 buffer[6];
    LIS302DL_Read(0x2B, buffer, 6);

    for(i=0; i<3; i++) {
      *out =(int32_t)(72 * (int8_t)buffer[2*i]);
      out++;
    }
}

u32 abs(int32_t n) {
    if (n < 0) {
        return -n;
    }
    return n;
}

int32_t cround(int32_t x, int32_t y) {
    return x/y*y;
}
