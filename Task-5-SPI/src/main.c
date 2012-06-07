#include "main.h"

int main(void) {
    LED_Init();
    SPI1_Init();
    LIS302DL_Init();

    u8 id;

    LIS302DL_Read(0x0F, &id, 1);

    int32_t data[3];

    while(ENABLE) {
        LIS302DL_ReadACC(data);

        GPIO_WriteBit(GPIOD, LED_Pins, SET);
        delay(1000-data[0]);
        GPIO_WriteBit(GPIOD, LED_Pins, RESET);
        delay(1000-data[0]);
    }
}

void LED_Init(void) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef LED_InitStruct;
    LED_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    LED_InitStruct.GPIO_OType = GPIO_OType_PP;
    LED_InitStruct.GPIO_Pin = LED_Pins;
    LED_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    LED_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOD, &LED_InitStruct);
}

void delay(int i) {
    i *= 1000;
    while (--i > 0) {
        __NOP();
    }
}

void SPI1_Init(void) {
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
    GPIO_WriteBit(GPIOE, GPIO_Pin_3, RESET);
    LIS302DL_SendByte(REG);
    for (int i=0; i < count; i++) {
        LIS302DL_SendByte(*DATA);
        DATA++;
    }
    GPIO_WriteBit(GPIOE, GPIO_Pin_3, SET);
}

void LIS302DL_Read(u8 REG, u8 *DATA, u8 count) {
    GPIO_WriteBit(GPIOE, GPIO_Pin_3, RESET);
    REG |= 0x80;
    if (count > 1) {
        REG |= 0x40;
    }
    LIS302DL_SendByte(REG);
    for (u8 i=0; i < count; i++) {
        *DATA = LIS302DL_SendByte((u8)0x00);
        DATA++;
    }

    GPIO_WriteBit(GPIOE, GPIO_Pin_3, SET);
}

void LIS302DL_ReadACC(int32_t* out) {
    u8 buffer[6];
    LIS302DL_Read(0x29, buffer, 6);

    for(int i=0; i<3; i++) {
      *out =(int32_t)(72 * (int8_t)buffer[2*i]);
      out++;
    }
}
