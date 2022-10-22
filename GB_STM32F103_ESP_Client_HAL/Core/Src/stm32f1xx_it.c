/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//#define gb_RX_BUFFER_SIZE_UART1 255
//	uint8_t gb_Rx_Buffer_UART1[gb_RX_BUFFER_SIZE_UART1]; // character array (buffer)
//	uint8_t gb_RX_Wr_Index_UART1 =0; //index of next char to be put into the buffer  // head
//	uint8_t gb_RX_Rd_Index_UART1=0; //index of next char to be fetched from the buffer    //tail
//	uint8_t gb_RX_Counter_UART1=0; //a total count of characters in the buffer
//	uint8_t gb_RX_No_of_byte_UART1=0;
//	_Bool gb_RX_Buffer_Overflow_UART1; // This flag is set on USART Receiver // buffer overflow
//	char gb_rx_byte;
//	char gb_RX_DATA_UART1[gb_RX_BUFFER_SIZE_UART1];
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */
//void USART1_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART1_IRQn 0 */
////
////	 if((__NVIC_GetActive(USART1_IRQn)) == 1)
////	    //check if we are here because of RXNE interrupt
////	         if (USART1->SR & USART_SR_RXNE) //if RX is not empty
////	                {
////	                      gb_rx_byte = USART1->DR; //fetch the data received
////	                      gb_Rx_Buffer_UART1[gb_RX_Wr_Index_UART1]= gb_rx_byte;   /* put received char in buffer */
////	  		              if(++gb_RX_Wr_Index_UART1 > gb_RX_BUFFER_SIZE_UART1)
////	  		              gb_RX_Wr_Index_UART1 = 0;
////	  		              if(++gb_RX_Counter_UART1 > gb_RX_BUFFER_SIZE_UART1) /* keep a character count */
////	  		                   {
////	  			                    /* overflow check.. */
////	  			                    gb_RX_Counter_UART1 = gb_RX_BUFFER_SIZE_UART1; /* if too many chars came */
////	  			                  //  gb_RX_Buffer_Overflow_UART2 = 1;            /* in before they could be used */
////	  			                }                                          /* that could cause an error!! */
////	  			          gb_RX_No_of_byte_UART1=gb_RX_Counter_UART1;
////	  			        		HAL_UART_Transmit(&huart1,(uint8_t *)&gb_rx_byte,1, 10);
////
////	                     // GB_UART_TxChar1(gb_rx_byte);
////
////	  			          //printf("%d",gb_rx_byte);
////
////	                 }
//  /* USER CODE END USART1_IRQn 0 */
//  HAL_UART_IRQHandler(&huart1);
//  /* USER CODE BEGIN USART1_IRQn 1 */
////
//  /* USER CODE END USART1_IRQn 1 */
//}
//
///**
//  * @brief This function handles USART2 global interrupt.
//  */
//void USART2_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART2_IRQn 0 */
//
//  /* USER CODE END USART2_IRQn 0 */
//  HAL_UART_IRQHandler(&huart2);
//  /* USER CODE BEGIN USART2_IRQn 1 */
//
//  /* USER CODE END USART2_IRQn 1 */
//}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

