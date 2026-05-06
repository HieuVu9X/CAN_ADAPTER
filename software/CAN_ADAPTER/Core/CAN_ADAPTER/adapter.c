/*
 * adapter.c
 *
 *  Created on: May 2, 2026
 *      Author: Admin
 */

/*--------------------------------------------------------------------------------
 *                                 Include Files
 -------------------------------------------------------------------------------*/
#include "adapter.h"
#include "usbd_cdc_if.h"
/*--------------------------------------------------------------------------------
 *                              Constant Definitions
 -------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
 *                              Variable Definitions
 -------------------------------------------------------------------------------*/
uint8_t out_queue[QUEUE_BUF_SIZE];
uint32_t out_wr_index =0, out_rd_index=0;

uint8_t in_queue[QUEUE_BUF_SIZE];
uint32_t in_wr_index =0, in_rd_index=0,in_pending;
uint32_t usart_sr;
//uint8_t test_buffer[14*8];
//uint32_t test_index=0,decode_err=0;
/*--------------------------------------------------------------------------------
 *                           Static Function Prototypes
 -------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
 *                           Public Function Definitions
 -------------------------------------------------------------------------------*/
void Send_USART_OUT()
{
	//Update rd id when DMA transfer not active
	//Send when data pending
	if(out_wr_index != out_rd_index){
		//Try start send data when DMA stoped
		if((USART_TX_DMA_STREAM->CCR & DMA_CCR_EN)==0){
			USART_TX_DMA_CLEAR_STATUS();
			if(out_wr_index >  out_rd_index){
				USART_TX_DMA_STREAM->CMAR = out_queue + out_rd_index;
				USART_TX_DMA_STREAM->CNDTR = out_wr_index-out_rd_index;
				USART_TX_DMA_STREAM->CPAR = &USART_PORT->RDR;
				out_rd_index = out_wr_index;
			}
			else{
				USART_TX_DMA_STREAM->CMAR = out_queue + out_rd_index;
				USART_TX_DMA_STREAM->CNDTR = QUEUE_BUF_SIZE - out_rd_index;
				USART_TX_DMA_STREAM->CPAR = &USART_PORT->RDR;
				out_rd_index = 0;
			}
			USART_TX_DMA_STREAM->CCR |= DMA_CCR_EN;
		}
	}
}

void USER_CDC_SetLineCoding(uint32_t data_rate, uint8_t stop_bit, uint8_t parity)
{
	/*******************************************************************************/
	/* Line Coding Structure                                                       */
	/*-----------------------------------------------------------------------------*/
	/* Offset | Field       | Size | Value  | Description                          */
	/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
	/* 4      | bCharFormat |   1  | Number | Stop bits                            */
	/*                                        0 - 1 Stop bit                       */
	/*                                        1 - 1.5 Stop bits                    */
	/*                                        2 - 2 Stop bits                      */
	/* 5      | bParityType |  1   | Number | Parity                               */
	/*                                        0 - None                             */
	/*                                        1 - Odd                              */
	/*                                        2 - Even                             */
	/*                                        3 - Mark                             */
	/*                                        4 - Space                            */
	/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
	/*******************************************************************************/
	LL_USART_InitTypeDef USART_InitStruct = {0};
	//USART port
	USART_TX_DMA_STREAM->CCR &= ~DMA_CCR_EN;
	USART_RX_DMA_STREAM->CCR &= ~DMA_CCR_EN;
	USART_PORT->CR1 &= ~ USART_CR1_UE;

	USART_InitStruct.BaudRate = data_rate;
	USART_InitStruct.DataWidth = parity?LL_USART_DATAWIDTH_9B:LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = (stop_bit==0)?LL_USART_STOPBITS_1: LL_USART_STOPBITS_2;
	USART_InitStruct.Parity = parity==0?LL_USART_PARITY_NONE:(parity==1)? LL_USART_PARITY_ODD:LL_USART_PARITY_EVEN;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;

	//-----USART port-----//
	LL_USART_Init(USART_PORT, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USART_PORT);
	USART_PORT->CR3 |= USART_CR3_DMAR| USART_CR3_DMAT;

	//Start RX DMA
	USART_RX_DMA_STREAM->CCR &= ~ DMA_CCR_EN;
	USART_RX_DMA_STREAM->CNDTR = QUEUE_BUF_SIZE;
	USART_RX_DMA_STREAM->CMAR = (uint32_t)in_queue;
	USART_RX_DMA_STREAM->CPAR = (uint32_t)&USART_PORT->RDR;
	//START USART
	USART_PORT->CR1 |= USART_CR1_UE|USART_CR1_TE|USART_CR1_RE;
	USART_RX_DMA_STREAM->CCR |= DMA_CCR_EN;

}

void Can_Adapter_Init(void)
{
	// Config USART1 receive data
	USART_PORT->CR1 &= USART_CR1_UE;
	USART_PORT->CR3 |= USART_CR3_DMAR| USART_CR3_DMAT;
	//Start RX DMA
	USART_RX_DMA_STREAM->CCR &= ~ DMA_CCR_EN;
	USART_RX_DMA_STREAM->CNDTR = QUEUE_BUF_SIZE;
	USART_RX_DMA_STREAM->CMAR = (uint32_t)in_queue;
	USART_RX_DMA_STREAM->CPAR = (uint32_t)&USART_PORT->RDR;

	USART_RX_DMA_STREAM->CCR |= DMA_CCR_EN;
	USART_PORT->CR1 |= USART_CR1_UE|USART_CR1_TE|USART_CR1_RE;
}

void Can_Adapter_Mainfunction(void)
{
	static uint32_t last_tick = 0;

	/*** send data from usb to uart ***/
	Send_USART_OUT();

	/*** send data from uart to usb ***/
	usart_sr = USART1->ISR;
	uint32_t dma_cnt = USART_RX_DMA_STREAM->CNDTR;

	in_wr_index = QUEUE_BUF_SIZE - dma_cnt;
	if(in_wr_index >=QUEUE_BUF_SIZE ) in_wr_index = QUEUE_BUF_SIZE-1;
	in_pending = (in_wr_index >= in_rd_index)? in_wr_index - in_rd_index: in_wr_index + QUEUE_BUF_SIZE - in_rd_index;
	uint32_t cur_tick = HAL_GetTick();
	uint32_t time_diff = (cur_tick-last_tick)&0xffff;

	if(in_wr_index != in_rd_index && (time_diff >=2 || in_pending >= 128)) {
		last_tick = HAL_GetTick();
		if(in_wr_index > in_rd_index) {
			CDC_Transmit_FS(in_queue+in_rd_index,in_wr_index - in_rd_index);
			in_rd_index = in_wr_index;
		}
		else {
			CDC_Transmit_FS(in_queue+in_rd_index,QUEUE_BUF_SIZE - in_rd_index);
			in_rd_index = 0;
		}
	}
}

void CDC_User_Rx(uint8_t* data, uint32_t len){
	while(len-->0){
		uint32_t wr_next = out_wr_index+1;
		if(wr_next >= QUEUE_BUF_SIZE) wr_next = 0;
		if(wr_next != out_rd_index) {
			out_queue[out_wr_index] = *data;
			data ++;
			out_wr_index = wr_next;
		}
		else break;
	}
}
/*--------------------------------------------------------------------------------
 *                           Static Function Definitions
 -------------------------------------------------------------------------------*/


