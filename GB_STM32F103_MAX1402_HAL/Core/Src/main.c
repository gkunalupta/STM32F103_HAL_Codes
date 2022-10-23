/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//#include "GB_MAX14002_STM32.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
#define cs_set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
#define cs_reset() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)

#define MAX1402_ADC   0x00
#define MAX1402_FADC  0X01
#define MAX1402_FLAGS 0x02
#define MAX1402_FLTEN 0x03
#define MAX1402_THL   0x04
#define MAX1402_THU   0x05
#define MAX1402_INRR  0x6
#define MAX1402_INRT  0x07
#define MAX1402_INRP  0x08
#define MAX1402_CFG   0x09
#define MAX1402_ENBL  0x0A
#define MAX1402_ACT   0x0B
#define MAX1402_WEN   0x0C
#define MAX1402_FLTV  0x13
#define MAX1402_THLV  0x14
#define MAX1402_THUV  0x15
#define MAX1402_INRRV 0x16
#define MAX1402_INRTV 0x17
#define MAX1402_INRPV 0x18
#define MAX1402_CFGV  0x19
#define MAX1402_ENBLV 0x1A



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t read_reg(uint16_t Rreg)
{
	return(((Rreg << 11) & 0xf800));
}
uint16_t write_reg(uint16_t Wreg,uint16_t Wdata)
{
	return(uint16_t)(((((uint16_t)Wreg) <<11) & 0xf800 ) + Wdata + 1024);
}
//uint16_t rx_buf[1025];

uint16_t rx_buf[15] =  {0};
uint16_t tx_buf;

void SPI1_Send (uint16_t *dt, uint16_t cnt)
{
  HAL_SPI_Transmit (&hspi1, (uint8_t*)dt, cnt, 5000);

}
HAL_StatusTypeDef SPI1_Send_Recv( uint16_t *dt, uint16_t *rt, uint16_t cnt)
{
	HAL_StatusTypeDef status = HAL_ERROR;

	status = HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)dt, (uint8_t*)rt, cnt, 5000);

	if(HAL_OK == status)
	{
	*rt =  (((uint16_t)(*rt<<6))>>6);
	return status;
	}
	return status;
}

void MAX1402_Read_reg(uint16_t reg, uint16_t *RregD)
{
	cs_reset();
	tx_buf=read_reg(reg);
	SPI1_Send_Recv(&tx_buf, RregD, 1);
	cs_set();
}

void MAX1402_Write_reg(uint16_t reg, uint16_t WregD)
{
	cs_reset();
	tx_buf=write_reg(reg, WregD);
	SPI1_Send(&tx_buf,1);
	cs_set();
}
uint16_t flag_read[15] = {0};
void MAX1402_init()
{
	//rx_buf[15] =  {0};

	//status = MAX_1402_GEN_ERROR;
	HAL_Delay(500);
	MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[0]);

	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_WEN,0x294); //WEN is written to 294 for enabling writing to SPI registers
	MAX1402_Read_reg(MAX1402_WEN, &rx_buf[0]);

	//MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[1]);

	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_ENBL, 0x000); // ENA bit is set to 0, setting this bit to 1 will enable the inrush and bias current
	MAX1402_Read_reg(MAX1402_ENBL, &rx_buf[1]); // so after programming all configuration and verification register will set this

	//MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[2]);
	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_FLTEN, 0x0ff); // MV bit is set to 0,
	MAX1402_Read_reg(MAX1402_FLTEN, &rx_buf[2]);

	//MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[6]);
	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_CFG, 0x176); // CFG= Filtered data, at average of 2, external voltage reference enabled
	//MAX1402_Write_reg(MAX1402_CFG, 0x306);
	MAX1402_Read_reg(MAX1402_CFG, &rx_buf[3]);

	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_CFGV, 0x176); // CFGV= Configuration verification register

	//MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[4]);  // read flags

	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_FLTEN, 0x1ff); // MV bit is set to 1, after configuring and verifcation register
	MAX1402_Read_reg(MAX1402_FLTEN, &rx_buf[4]);


	MAX1402_Write_reg(MAX1402_WEN,0x000); //WEN is written to 000 for disabling writing to SPI registers
	MAX1402_Read_reg(MAX1402_WEN, &rx_buf[5]);

	MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[4]);  // read flags
	MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[5]);  // read flags

	MAX1402_Write_reg(MAX1402_ACT, 0x040); //80
	MAX1402_Read_reg(MAX1402_ACT, &rx_buf[12]);

	MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[10]);  // read flags
		MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[11]);  // read flags


	// if yes then procced
	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_WEN,0x294);   //WEN is written to 294 for enabling writing to SPI registers
	MAX1402_Read_reg(MAX1402_WEN, &rx_buf[6]);

	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_ENBL, 0x010); // ENA bit is set to 1, setting this bit to 1 will enable the inrush and bias current
	MAX1402_Read_reg(MAX1402_ENBL, &rx_buf[7]); // so after programming all configuration and verification register will set this

	HAL_Delay(100);
	MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[6]);  // read flags
	MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[7]);  // read flags

	//MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[8]);  // read flags

	HAL_Delay(100);
	MAX1402_Write_reg(MAX1402_WEN,0x000); //WEN is written to 294 for cpnfiguring
	MAX1402_Read_reg(MAX1402_WEN, &rx_buf[8]);


	MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[8]);  // read flags
	MAX1402_Read_reg(MAX1402_FLAGS, &flag_read[9]);  // read flags

//	if(0x100 == rx_buf)
//	{
//		status = MAX_1402_FLAG_OK;
//	}


	/*
	 * 1) value of 0x294 has to be written in register 0x0c, before writing to any of the configuration and verifation register.
	 * 2) ENA bit is set as 0 in ENBL register before programming CFG register.
	 * 3) Enable external shunt resistror n CFG register ENTI and EXRF bits are set as 1, also vonfigures the CFG register.
	 * 4) ENA bit is set as 1 in ENBL register after programming CFG register.
	 */

}

uint16_t ADC_Data = 0;
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  MAX1402_init();
  MAX1402_Read_reg(MAX1402_CFG, &rx_buf[14]);
  MAX1402_Read_reg(MAX1402_CFG, &rx_buf[14]);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	HAL_Delay(100);
	MAX1402_Read_reg(MAX1402_FADC, &ADC_Data);
	HAL_Delay(200);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

