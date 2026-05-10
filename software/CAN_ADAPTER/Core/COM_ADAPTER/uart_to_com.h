/*
 * adapter.h
 *
 *  Created on: May 2, 2026
 *      Author: Admin
 */

#ifndef COM_ADAPTER_UART_TO_COM_H_
#define COM_ADAPTER_UART_TO_COM_H_

/*--------------------------------------------------------------------------------
 *                                 Include Files
 -------------------------------------------------------------------------------*/
#include "main.h"
/*--------------------------------------------------------------------------------
 *                               Macro Definitions
 -------------------------------------------------------------------------------*/
#define USART_PORT					USART1
#define USART_RX_DMA				DMA1
#define USART_RX_DMA_CHANEL			LL_DMA_CHANNEL_1
#define USART_RX_DMA_STREAM			DMA1_Channel1
#define USART_RX_DMA_CLEAR_STATUS()	DMA1->IFCR = 0x0f<<0

#define USART_TX_DMA				DMA1
#define USART_TX_DMA_CHANEL			LL_DMA_CHANNEL_2
#define USART_TX_DMA_STREAM			DMA1_Channel2
#define USART_TX_DMA_CLEAR_STATUS()	DMA1->IFCR = 0xf0<<0

#define QUEUE_BUF_SIZE				(1024*8)	//Must 2^N
/*--------------------------------------------------------------------------------
 *                                Type Definitions
 -------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
 *                              Variable Definitions
 -------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
 *                           Inline Function Prototypes
 -------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
 *                           Public Function Prototypes
 -------------------------------------------------------------------------------*/
void UART_Adapter_Init(void);
void UART_Adapter_Mainfunction(void);
void CDC_User_Rx(uint8_t* data, uint32_t len);
/*--------------------------------------------------------------------------------
 *                           Inline Function Definitions
 -------------------------------------------------------------------------------*/


#endif /* COM_ADAPTER_UART_TO_COM_H_ */
