/**
 ******************************************************************************
 * @file    main.c
 * @authors MCD Application Team, Martin Thomas (not STM)
 * @version see VERSION_STRING below
 * @date    December-2012
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics, 2012 M. Thomas</center></h2>
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcc_stdio.h"
#include <stdio.h>
#include "xprintf.h"
#include <math.h>
#include <string.h>

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
 * @{
 */

/** @addtogroup SysTick_and_USART_Example_MThomas
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	COM1 = 0, COM2 = 1
} COM_TypeDef;

/* Private define ------------------------------------------------------------*/

#define VERSION_STRING   "V0.2.0"
#define COMn                             1

/**
 * @brief Definition for COM port1, connected to USART3
 */
#define EVAL_COM1                        USART3
#define EVAL_COM1_CLK                    RCC_APB1Periph_USART3
#define EVAL_COM1_TX_PIN                 GPIO_Pin_10
#define EVAL_COM1_TX_GPIO_PORT           GPIOC
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_TX_AF                  GPIO_AF_USART3
#define EVAL_COM1_RX_PIN                 GPIO_Pin_11
#define EVAL_COM1_RX_GPIO_PORT           GPIOC
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource11
#define EVAL_COM1_RX_AF                  GPIO_AF_USART3
#define EVAL_COM1_IRQn                   USART3_IRQn

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// static GPIO_InitTypeDef GPIO_InitStructure;
static USART_InitTypeDef USART_InitStructure;
static __IO uint32_t TimingDelay;
static __IO uint32_t LED_Tick;

static USART_TypeDef* COM_USART[COMn] = { EVAL_COM1 };
static GPIO_TypeDef* COM_TX_PORT[COMn] = { EVAL_COM1_TX_GPIO_PORT };
static GPIO_TypeDef* COM_RX_PORT[COMn] = { EVAL_COM1_RX_GPIO_PORT };
static const uint32_t COM_USART_CLK[COMn] = { EVAL_COM1_CLK };
static const uint32_t COM_TX_PORT_CLK[COMn] = { EVAL_COM1_TX_GPIO_CLK };
static const uint32_t COM_RX_PORT_CLK[COMn] = { EVAL_COM1_RX_GPIO_CLK };
static const uint16_t COM_TX_PIN[COMn] = { EVAL_COM1_TX_PIN };
static const uint16_t COM_RX_PIN[COMn] = { EVAL_COM1_RX_PIN };
static const uint16_t COM_TX_PIN_SOURCE[COMn] = { EVAL_COM1_TX_SOURCE };
static const uint16_t COM_RX_PIN_SOURCE[COMn] = { EVAL_COM1_RX_SOURCE };
static const uint16_t COM_TX_AF[COMn] = { EVAL_COM1_TX_AF };
static const uint16_t COM_RX_AF[COMn] = { EVAL_COM1_RX_AF };

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Configures COM port.
 * @param  COM: Specifies the COM port to be configured.
 *   This parameter can be one of following parameters:
 *     @arg COM1
 * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
 *   contains the configuration information for the specified USART peripheral.
 * @retval None
 */
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

	if (COM == COM1) {
		/* Enable UART clock */
		RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
	}

	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

	/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
	GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

	/* USART configuration */
	USART_Init(COM_USART[COM], USART_InitStruct);

	/* Enable USART */
	USART_Cmd(COM_USART[COM], ENABLE);

	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC ) == RESET) {
		// first character is not send correctly without this check
	}
}

/**
 * @brief  Character output function to terminal/stdio-retarget
 * @param  ch: character to be sent
 * @retval character which has been sent
 */
int _io_putchar(int ch)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(EVAL_COM1, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET) { ; }

	return ch;
}

/**
 * @brief  Checks if Character is available function to terminal/stdio-retarget
 * @param  none
 * @retval 0 if not char, !=0 if char available
 */
int _io_charavail()
{
	return ( USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET ) ? 0 : 1;
}

/**
 * @brief  gets character from input stream
 * @param  none
 * @retval character
 */
int _io_getchar()
{
	while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET) { ; }
	return (int)(USART_ReceiveData(EVAL_COM1));
}


/**
 * @brief   Main program
 * @param  None
 * @retval None
 */

int main(void) {

	int ch;
	int last_isavail, current_isavail;
	int led_flip;

	/* Initialize Leds mounted on STM32F4-Discovery board */
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	/* Turn on LED4 and LED5 */
	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED5);

	/* Setup SysTick Timer for 1 msec interrupts. */
	if (SysTick_Config(SystemCoreClock / 1000)) {
		/* Capture error */
		while (1) {
			;
		}
	}

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	STM_EVAL_COMInit(COM1, &USART_InitStructure);

	xdev_out(_io_putchar);

	xprintf("This is a modified version of STM's USB VCP Demo " VERSION_STRING " " __DATE__ " " __TIME__ "\n");
	xprintf("*** THIS IS JUST A PROOF-OF-CONCEPT ***\n");

	USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS
	            USB_OTG_HS_CORE_ID,
#else
	            USB_OTG_FS_CORE_ID,
#endif
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);

	current_isavail = USBD_USR_VCP_IsAvailable();
	last_isavail = current_isavail;
	led_flip = 0;
	LED_Tick = 0;

	while (1) {
		if (LED_Tick == 0) {
			if (led_flip) {
				/* Toggle LED3 and LED6 */
				STM_EVAL_LEDToggle(LED3); //3
				STM_EVAL_LEDToggle(LED6); //6
				LED_Tick = 50;
			}
			else {
				/* Toggle LED4 and LED5 */
				STM_EVAL_LEDToggle(LED4); //4
				STM_EVAL_LEDToggle(LED5); //5
				LED_Tick = 100;
			}
			led_flip = !led_flip;
		}

		/* check VCP's availability */
		current_isavail = USBD_USR_VCP_IsAvailable();
		if (current_isavail != last_isavail) {
			last_isavail = current_isavail;
			if (current_isavail) {
				xprintf("VCP available\n");
			} else {
				xprintf("VCP not available\n");
			}
		}

		/* send character received from USART to the host via VCP */
		if ( _io_charavail() ) {
			ch = _io_getchar();
			if (current_isavail) {
				char s[100];
				xprintf("from COM1: %c\n", (char)ch);
				xsprintf(s, "Controller sent [IN from STM32]: %c\r\n", (char)ch);
				VCP_SendString(s);
			} else {
				xprintf("VCP not available\n");
			}
		}

		/* check for characters received through VCP */
		if (VCP_HasReceived()) {
			ch = VCP_GetReceived();
			xprintf("Host sent [OUT to STM32]: %c\n", (char)ch);
		}
	}
}

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void Delay(__IO uint32_t nTime) {
	TimingDelay = nTime;

	while (TimingDelay != 0)
		;
}

/**
 * @brief  Decrements the TimingDelay variable.
 *         gets called by SysTick_Handler
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void) {
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
	if (LED_Tick != 0) {
		LED_Tick--;
	}
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	xprintf("******** Wrong parameter value: file %s on line %ld ********\n", file, line);

#if 0
	/* Infinite loop */
	while (1)
	{
	}
#endif
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
