#ifndef _ST7565R_H_
#define _ST7565R_H_

#include <stdio.h>
#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "stm32f4_discovery.h"
#include "usbd_cdc_vcp.h"

NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
EXTI_InitTypeDef EXTI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef SPI_InitStructure;

#define ST7565R_CMD		0
#define ST7565R_DAT		1

//*****************************************************************************
//
//! ST7565R Pins
//
//*****************************************************************************
#define ST7565R_CS       GPIO_Pin_12
#define ST7565R_RST      GPIO_Pin_9
#define ST7565R_RS       GPIO_Pin_8
#define ST7565R_SCLK     GPIO_Pin_13
#define ST7565R_SID      GPIO_Pin_15
#define ST7565R_PORT     GPIOB

//*****************************************************************************
//
//! ST7565R Definition
//
//*****************************************************************************
#define ST7565R_RS_H     GPIO_SetBits(ST7565R_PORT, ST7565R_RS)
#define ST7565R_RS_L     GPIO_ResetBits(ST7565R_PORT, ST7565R_RS)
#define ST7565R_RST_H    GPIO_SetBits(ST7565R_PORT, ST7565R_RST)
#define ST7565R_RST_L    GPIO_ResetBits(ST7565R_PORT, ST7565R_RST)
#define ST7565R_SID_H    GPIO_SetBits(ST7565R_PORT, ST7565R_SID)
#define ST7565R_SID_L    GPIO_ResetBits(ST7565R_PORT, ST7565R_SID)
#define ST7565R_SCLK_H   GPIO_SetBits(ST7565R_PORT, ST7565R_SCLK)
#define ST7565R_SCLK_L   GPIO_ResetBits(ST7565R_PORT, ST7565R_SCLK)
#define ST7565R_CS_H     GPIO_SetBits(ST7565R_PORT, ST7565R_CS)
#define ST7565R_CS_L     GPIO_ResetBits(ST7565R_PORT, ST7565R_CS)

//*****************************************************************************
//
//! ST7565R Commands
//
//*****************************************************************************
#define CMD_DISPLAY_OFF			0xAE
#define CMD_DISPLAY_ON			0xAF

#define CMD_SET_DISP_START_LINE	0x40
#define CMD_SET_PAGE			0xB0

#define CMD_SET_COLUMN_UPPER	0x10
#define CMD_SET_COLUMN_LOWER	0x00

#define CMD_SET_ADC_NORMAL		0xA0
#define CMD_SET_ADC_REVERSE		0xA1

#define CMD_SET_DISP_NORMAL		0xA6
#define CMD_SET_DISP_REVERSE	0xA7

#define CMD_SET_ALLPTS_NORMAL	0xA4
#define CMD_SET_ALLPTS_ON		0xA5
#define CMD_SET_BIAS_9			0xA2
#define CMD_SET_BIAS_7			0xA3

#define CMD_RMW					0xE0
#define CMD_RMW_CLEAR			0xEE
#define CMD_INTERNAL_RESET		0xE2
#define CMD_SET_COM_NORMAL		0xC0
#define CMD_SET_COM_REVERSE		0xC8
#define CMD_SET_POWER_CONTROL	0x28
#define CMD_SET_RESISTOR_RATIO	0x20
#define CMD_SET_VOLUME_FIRST	0x81
#define CMD_SET_VOLUME_SECOND	0x00
#define CMD_SET_STATIC_OFF		0xAC
#define CMD_SET_STATIC_ON		0xAD
#define CMD_SET_STATIC_REG		0x00
#define CMD_SET_BOOSTER_FIRST	0xF8
#define CMD_SET_BOOSTER_234		0x00
#define CMD_SET_BOOSTER_5		0x01
#define CMD_SET_BOOSTER_6		0x03
#define CMD_NOP					0xE3
#define CMD_TEST				0xF0

//*****************************************************************************
//
//! ST7565R Functions
//
//*****************************************************************************
void ST7565R_GPIO_Init(void);
void ST7565R_Write(u8 ucDatOrCmd, u8 ucData);
void ST7565R_Init(void);
void ST7565R_Clear_Screen(void);
void ST7565R_Display_ASCII(u8 ucRow, u8 ucColumn, u8 ucAscii);
void ST7565R_Delay(u16 uTime);

#endif /* _ST7565R_H_ */

