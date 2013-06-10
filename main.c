#include "ST7565R.h"

u32 i = 0, j = 0, k = 0;

char tab[63];

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

void STM32_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitStructure.EXTI_Line = GPIO_Pin_0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb,&USR_cb);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
		{
			ST7565R_Clear_Screen();

			if(tab[4] == ' ')
			{
				for(j = 0; j < 21 ; j++)
				{
					if(j < 15)
						ST7565R_Display_ASCII(1, j+1, (int)tab[21*k+j]);
					else
						ST7565R_Display_ASCII(0, j-9, (int)tab[21*k+j]);
				}
			}
			else
			{
				for(j = 0; j < 21 ; j++)
				{
					if(j < 9)
						ST7565R_Display_ASCII(0, j+4, (int)tab[21*k+j]);
					else
						if(j < 15)
							ST7565R_Display_ASCII(1, j-4, (int)tab[21*k+j]);
						else
							ST7565R_Display_ASCII(2, j-10, (int)tab[21*k+j]);
				}
			}
			k++;

			if(k > 2)
			{
				k = 0;
			}
		}

		TIM_Cmd(TIM3, DISABLE);
		TIM_SetCounter(TIM3, 0);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		TIM_Cmd(TIM3, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**
**===========================================================================
**  Main
**===========================================================================
*/

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	STM32_Init();
	ST7565R_Init();
	ST7565R_Clear_Screen();

	//ST7565R_Display_ASCII(1, 15, 65); // od 0 do 3; od 0 do 15; ASCII

	for(i = 0 ; i < 63 ; i++)
		tab[i] = ' ';
	i = 0;

	while(1)
	{
		if(usb_cdc_kbhit())
		{
			if(i < 63)
			{
				tab[i] = usb_cdc_getc();

				if(tab[i] == ',')
					tab[i] = '.';

				i++;
			}

			if(i > 62)
			{
				i = 0;
			}
		}
	}
}
