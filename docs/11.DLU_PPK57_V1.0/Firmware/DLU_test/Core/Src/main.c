/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RX_BUFFER_SIZE 		150
#define QUEUE_SIZE 			152 // RX_BUFFERE_SIZE + 2
#define FRAME_SIZE			72
#define PACKET_SIZE 		FRAME_SIZE*6
#define NUM_OF_PORT 6
#define PORT_1		0
#define PORT_2		1
#define PORT_3		2
#define PORT_4		3
#define PORT_5		4
#define PORT_6		5

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_uart5_rx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart6_rx;

/* USER CODE BEGIN PV */
uint8_t rx_dma_buffer_port_3[RX_BUFFER_SIZE] = {0};
uint8_t rx_dma_buffer_port_4[RX_BUFFER_SIZE] = {0};
uint8_t rx_dma_buffer_port_5[RX_BUFFER_SIZE] = {0};
uint8_t rx_dma_buffer_port_6[RX_BUFFER_SIZE] = {0};
uint8_t rx_dma_buffer_port_extend[RX_BUFFER_SIZE] = {0};

uint8_t txBuffer_debug[PACKET_SIZE] = {0};

uint8_t port_6_queue[QUEUE_SIZE] = {0};
uint8_t port_5_queue[QUEUE_SIZE] = {0};
uint8_t port_4_queue[QUEUE_SIZE] = {0};
uint8_t port_3_queue[QUEUE_SIZE] = {0};
uint8_t port_2_queue[QUEUE_SIZE] = {0};
uint8_t port_1_queue[QUEUE_SIZE] = {0};

uint8_t frame_size[6] = {0};

uint8_t uart_dma_processing[6] = {0};

uint32_t db_cur_tick = 0;
uint32_t led_timer_counter = 0;
uint32_t process_period_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_UART4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_UART5_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	static uint32_t led_port_1_debug_irq = 0;
	static uint32_t led_port_2_debug_irq = 0;
	static uint32_t led_port_3_debug_irq = 0;
	static uint32_t led_port_4_debug_irq = 0;
	static uint32_t led_port_5_debug_irq = 0;
	static uint32_t led_port_6_debug_irq = 0;
	led_port_1_debug_irq++;
	led_port_2_debug_irq++;
	led_port_3_debug_irq++;
	led_port_4_debug_irq++;
	led_port_5_debug_irq++;
	led_port_6_debug_irq++;
	if (huart->Instance == USART2){ // port 6
		if (led_port_6_debug_irq > 10) {
			led_port_6_debug_irq = 0;
			HAL_GPIO_TogglePin(LED_7_GPIO_Port, LED_7_Pin);
		}
		frame_size[PORT_6] = Size;
		uart_dma_processing[PORT_6] = 1;
		port_6_queue[0] = 0xaa;
		port_6_queue[1] = 6;
		memcpy((port_6_queue + 2), rx_dma_buffer_port_6, frame_size[PORT_6]);
//		volatile uint32_t byte_sr;
//		byte_sr = USART2->SR;
//		byte_sr = USART2->DR;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_dma_buffer_port_6, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}

	if (huart->Instance == USART1){ // port 5
		if (led_port_5_debug_irq > 10) {
			led_port_5_debug_irq = 0;
			HAL_GPIO_TogglePin(LED_6_GPIO_Port, LED_6_Pin);
		}
		frame_size[PORT_5] = Size;
		uart_dma_processing[PORT_5] = 1;
		port_5_queue[0] = 0xaa;
		port_5_queue[1] = 5;
		memcpy((port_5_queue + 2), rx_dma_buffer_port_5, frame_size[PORT_5]);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_dma_buffer_port_5, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}

	if (huart->Instance == USART6){ // port 4
		if (led_port_4_debug_irq > 10) {
			led_port_4_debug_irq = 0;
			HAL_GPIO_TogglePin(LED_5_GPIO_Port, LED_5_Pin);
		}
		frame_size[PORT_4] = Size;
		uart_dma_processing[PORT_4] = 1;
		port_4_queue[0] = 0xaa;
		port_4_queue[1] = 4;
		memcpy((port_4_queue + 2), rx_dma_buffer_port_4, frame_size[PORT_4]);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart6, rx_dma_buffer_port_4, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
	}

	if (huart->Instance == USART3){ // port 3
		if (led_port_3_debug_irq > 10) {
			led_port_3_debug_irq = 0;
			HAL_GPIO_TogglePin(LED_4_GPIO_Port, LED_4_Pin);
		}
		frame_size[PORT_3] = Size;
		uart_dma_processing[PORT_3] = 1;
		port_3_queue[0] = 0xaa;
		port_3_queue[1] = 3;
		memcpy((port_3_queue + 2), rx_dma_buffer_port_3, frame_size[PORT_3]);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_dma_buffer_port_3, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
	}

	if (huart->Instance == UART5){ // port extend 1-2
		if (rx_dma_buffer_port_extend[1] == 1){
			if (led_port_1_debug_irq > 2) {
				led_port_1_debug_irq = 0;
				HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
			}
			frame_size[PORT_1] = Size;
			uart_dma_processing[PORT_1] = 1;
			memcpy(port_1_queue, rx_dma_buffer_port_extend, frame_size[PORT_1]);
		} else if (rx_dma_buffer_port_extend[1] == 2){
			if (led_port_2_debug_irq > 2) {
				led_port_2_debug_irq = 0;
				HAL_GPIO_TogglePin(LED_3_GPIO_Port, LED_3_Pin);
			}
			frame_size[PORT_2] = Size;
			uart_dma_processing[PORT_2] = 1;
			memcpy(port_2_queue, rx_dma_buffer_port_extend, frame_size[PORT_2]);
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rx_dma_buffer_port_extend, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_uart5_rx, DMA_IT_HT);
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_UART5_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  //port 6 - USART 2
//  HAL_GPIO_WritePin(USART2_DIR_GPIO_Port, USART2_DIR_Pin, GPIO_PIN_RESET);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_dma_buffer_port_6, RX_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

  //port 5 - USART 1
//  HAL_GPIO_WritePin(USART1_DIR_GPIO_Port, USART1_DIR_Pin, GPIO_PIN_RESET);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_dma_buffer_port_5, RX_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

  //port 4 - USART 6
//  HAL_GPIO_WritePin(USART6_DIR_GPIO_Port, USART6_DIR_Pin, GPIO_PIN_RESET);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart6, rx_dma_buffer_port_4, RX_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);

  //port 3 - USART 3
//  HAL_GPIO_WritePin(USART3_DIR_GPIO_Port, USART3_DIR_Pin, GPIO_PIN_RESET);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_dma_buffer_port_3, RX_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);

  //port extend - UART 5
//  HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, GPIO_PIN_RESET);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rx_dma_buffer_port_extend, RX_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_uart5_rx, DMA_IT_HT);

  // start timer 2
  HAL_TIM_Base_Start_IT(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  db_cur_tick = HAL_GetTick();
	  if (db_cur_tick % 500 == 100) HAL_GPIO_TogglePin(DB_LED_GPIO_Port, DB_LED_Pin);
//	  for (int i = 0; i < NUM_OF_PORT; i++){
//		  uart_dma_processing[i] = 0;
//	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 59;
  htim2.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim2.Init.Period = 9999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 500000;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED_7_Pin|LED_6_Pin|LED_5_Pin|LED_1_Pin
                          |LED_2_Pin|LED_3_Pin|LED_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, USART3_DIR_Pin|USART2_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USART6_DIR_GPIO_Port, USART6_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USART1_DIR_GPIO_Port, USART1_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DB_LED_GPIO_Port, DB_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_7_Pin LED_6_Pin LED_5_Pin LED_1_Pin
                           LED_2_Pin LED_3_Pin LED_4_Pin */
  GPIO_InitStruct.Pin = LED_7_Pin|LED_6_Pin|LED_5_Pin|LED_1_Pin
                          |LED_2_Pin|LED_3_Pin|LED_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USART3_DIR_Pin USART2_DIR_Pin */
  GPIO_InitStruct.Pin = USART3_DIR_Pin|USART2_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USART6_DIR_Pin */
  GPIO_InitStruct.Pin = USART6_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USART6_DIR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USART1_DIR_Pin */
  GPIO_InitStruct.Pin = USART1_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USART1_DIR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DB_LED_Pin */
  GPIO_InitStruct.Pin = DB_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DB_LED_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART1){ // port 5
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);
		HAL_UART_DMAStop(huart);
		HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dma_buffer_port_5, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	} else if (huart->Instance == USART2){ // port 6
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);
		HAL_UART_DMAStop(huart);
		HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dma_buffer_port_6, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	} else if (huart->Instance == USART3){ // port 3
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);
		HAL_UART_DMAStop(huart);
		HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dma_buffer_port_3, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
	} else if (huart->Instance == USART6){ // port 4
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);
		HAL_UART_DMAStop(huart);
		HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dma_buffer_port_4, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
	} else if (huart->Instance == UART5){ // port extend
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);
		if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE)) __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);
		HAL_UART_DMAStop(huart);
		HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_dma_buffer_port_extend, RX_BUFFER_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_uart5_rx, DMA_IT_HT);
	}
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM2)
  {
	  process_period_counter++;
	  led_timer_counter++;
	  if (led_timer_counter > 25) {
		  led_timer_counter = 0;
		  HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
	  }
	  if (process_period_counter >3){
		  process_period_counter = 0;
		  // process_data, merge data from port_queue to txBuffer_debug
		  for (int i = 0; i < 72; i++){
			  txBuffer_debug[i] 	= port_1_queue[i];
			  txBuffer_debug[i+72]  = port_2_queue[i];
			  txBuffer_debug[i+144] = port_3_queue[i];
			  txBuffer_debug[i+216] = port_4_queue[i];
			  txBuffer_debug[i+288] = port_5_queue[i];
			  txBuffer_debug[i+360] = port_6_queue[i];
		  }
		txBuffer_debug[0]   = 0xaa;
		txBuffer_debug[72]  = 0xaa;
		txBuffer_debug[144] = 0xaa;
		txBuffer_debug[216] = 0xaa;
		txBuffer_debug[288] = 0xaa;
		txBuffer_debug[360] = 0xaa;
		// send data
  		HAL_UART_Transmit_DMA(&huart4, (uint8_t *)txBuffer_debug, PACKET_SIZE);
  		// clear port queue after merge and send
//  		for (int i = 0; i < QUEUE_SIZE; i++){
//  			port_1_queue[i] = 0;
//  			port_2_queue[i] = 0;
//  			port_3_queue[i] = 0;
//  			port_4_queue[i] = 0;
//  			port_5_queue[i] = 0;
//  			port_6_queue[i] = 0;
//  		}
  	}
  }
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
