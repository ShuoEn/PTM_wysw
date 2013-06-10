#include "ST7565R.h"
#include "ST7565R_Font.h"

//*****************************************************************************
//
//! Initialization ST7565R
//
//*****************************************************************************
void ST7565R_Init(void)
{
	ST7565R_GPIO_Init();

    ST7565R_Delay(50);
    ST7565R_RST_H;
    ST7565R_Delay(50);

    ST7565R_Write(ST7565R_CMD, CMD_SET_BIAS_7);
    ST7565R_Write(ST7565R_CMD, CMD_SET_ADC_REVERSE);
    ST7565R_Write(ST7565R_CMD, CMD_SET_COM_NORMAL);
    ST7565R_Write(ST7565R_CMD, CMD_SET_RESISTOR_RATIO | 0x06);
    ST7565R_Write(ST7565R_CMD, CMD_SET_VOLUME_FIRST);
    ST7565R_Write(ST7565R_CMD, CMD_SET_VOLUME_SECOND | (0x0A & 0x3F));
    ST7565R_Write(ST7565R_CMD, CMD_SET_POWER_CONTROL | 0x04);
    ST7565R_Write(ST7565R_CMD, CMD_SET_POWER_CONTROL | 0x06);
    ST7565R_Write(ST7565R_CMD, CMD_SET_POWER_CONTROL | 0x07);
    ST7565R_Write(ST7565R_CMD, CMD_SET_ALLPTS_NORMAL);
    ST7565R_Write(ST7565R_CMD, CMD_DISPLAY_ON);
    ST7565R_Write(ST7565R_CMD, CMD_SET_DISP_START_LINE);
}

//*****************************************************************************
//
//! Initialization GPIO and SPI for ST7565R
//
//*****************************************************************************
void ST7565R_GPIO_Init(void)
{
	GPIO_InitStructure.GPIO_Pin = ST7565R_CS | ST7565R_RS | ST7565R_RST;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(ST7565R_PORT, &GPIO_InitStructure);

    GPIO_PinAFConfig(ST7565R_PORT, GPIO_PinSource13, GPIO_AF_SPI2);
    GPIO_PinAFConfig(ST7565R_PORT, GPIO_PinSource15, GPIO_AF_SPI2);

    GPIO_InitStructure.GPIO_Pin = ST7565R_SCLK | ST7565R_SID;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(ST7565R_PORT, &GPIO_InitStructure);

    ST7565R_RS_L;
    ST7565R_RST_L;
    ST7565R_SID_L;
    ST7565R_SCLK_L;
    ST7565R_CS_H;
    ST7565R_Delay(50);

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);
}

//*****************************************************************************
//
//! Delay ms
//
//*****************************************************************************
void ST7565R_Delay(u16 uTime)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;
	TIM_TimeBaseStructure.TIM_Period = (uTime*10) - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision= 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_Cmd(TIM2, ENABLE);

    TIM_SetCounter(TIM2, 0);

    while(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET);

    TIM_Cmd(TIM2, DISABLE);
    TIM_ClearFlag(TIM2, TIM_IT_Update);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}

//*****************************************************************************
//
//! Send data or command to ST7565R
//!
//! ucDatOrCmd decide send data or command.
//! ucSendByte is data(8bit) or command(8bit).
//! ucDatOrCmd can be ST7920_DAT or ST7920_CMD
//
//*****************************************************************************
void ST7565R_Write(u8 ucDatOrCmd, u8 ucData)
{
    ST7565R_CS_L;

    if(ucDatOrCmd == ST7565R_DAT)
    {
        ST7565R_RS_H;
    }
    else
    {
        ST7565R_RS_L;
    }

    ST7565R_Delay(10);
    SPI_I2S_SendData(SPI2, ucData);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    ST7565R_Delay(10);

    ST7565R_CS_H;
    ST7565R_Delay(50);
}

//*****************************************************************************
//
//! Clear the screen of ST7565R
//
//*****************************************************************************
void ST7565R_Clear_Screen(void)
{
    u8 i, j;

    for (i = 0; i < 8; i++)
    {
        ST7565R_Write(ST7565R_CMD, CMD_SET_PAGE + i);
        ST7565R_Write(ST7565R_CMD, CMD_SET_COLUMN_UPPER);
        ST7565R_Write(ST7565R_CMD, CMD_SET_COLUMN_LOWER);

        for (j = 0; j < 132; j++)
        {
            ST7565R_Write(ST7565R_DAT, 0x00);
        }
    }
}

//*****************************************************************************
//
//! Display a ASCII
//!
//! ucRow is Row
//! ucColumn is Column
//! ucAscii is ASCII
//
//*****************************************************************************
void ST7565R_Display_ASCII(u8 ucRow, u8 ucColumn, u8 ucAscii)
{
    u8 i, j, Column_Temp, Column_L, Column_H;
    u32 uiHChar;

    uiHChar = (ucAscii - 32) * 16;

    for (i = 0; i < 2; i++)
    {
        ST7565R_Write(ST7565R_CMD, CMD_SET_PAGE + i + (2*ucRow));

        Column_Temp = ucColumn * 0x08;
        Column_H = Column_Temp & 0xF0;
        Column_H = Column_H >> 4;
        Column_L = Column_Temp & 0x0F;

        ST7565R_Write(ST7565R_CMD, CMD_SET_COLUMN_UPPER + Column_H);
        ST7565R_Write(ST7565R_CMD, CMD_SET_COLUMN_LOWER + Column_L);

        for (j = 0; j < 8; j++)
        {
            ST7565R_Write(ST7565R_DAT, ASCII[uiHChar]);
            uiHChar++;
        }
    }

    ST7565R_Delay(20);
}
