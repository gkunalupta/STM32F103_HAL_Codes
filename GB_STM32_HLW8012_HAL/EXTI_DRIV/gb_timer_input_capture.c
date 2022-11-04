/*
 * gb_timer_input_capture.c
 *
 *  Created on: Nov 5, 2022
 *      Author: acer
 */

#include "main.h"
#include "stm32f1xx_it.h"


extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

uint32_t TIM3_IC1Value = 0;
uint32_t TIM3_IC2Value = 0;
float TIM3_Frequency = 0;
float TIM3_Duty = 0;
float TIM3_Period = 0;
float TIM3_Width = 0;
float TIM3_Period_Current = 0;


uint32_t TIM4_IC1Value = 0;
uint32_t TIM4_IC2Value = 0;
float TIM4_Frequency = 0;
float TIM4_Duty = 0;
float TIM4_Period = 0;
float TIM4_Width = 0;
float TIM4_Period_Current = 0;


void TIM3_IRQHandler(void)
{
	 if (__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_CC2) != RESET)   // Capture Compare Event 1
	  {
	    if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_CC2) != RESET)   // Cross check that whether source of Interrupt is configured
		// if(TIM3->DIER & TIM_DIER_CC1IE)
	    {
	        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC2);
	        htim3.Channel = HAL_TIM_ACTIVE_CHANNEL_2;

	        /* Input capture event */
	        if ((htim3.Instance->CCMR1 & TIM_CCMR1_CC2S) != 0x00U) // cross check that if Input Capture Interrupt is their, if input capture event is their
	        {
	        	if (htim3.Channel == HAL_TIM_ACTIVE_CHANNEL_2)  // If the interrupt is triggered by channel 2( for timer 3: pin PB5)
	        						{
	        							// Read the IC value
	        							TIM3_IC2Value = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);

	        							if (TIM3_IC2Value != 0)
	        							{
	        								TIM3_IC1Value = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
	        								// calculate the Duty Cycle
	        								TIM3_Duty = (TIM3_IC1Value *100)/TIM3_IC2Value;
	        								TIM3_Frequency = 100000/TIM3_IC2Value;
	        								TIM3_Period = 1000000/TIM3_Frequency;//(Perid is in microseconds)
	        								TIM3_Width = TIM3_Period/2;  //(in microseconds)

	        							}
	        						}
	        }
	    }
	  }

  /* USER CODE END TIM3_IRQn 1 */
}

float TIM3_getPeriod()
{
	return TIM3_Period;
}

float TIM3_getwidth()
{
	return TIM3_Width;
}

float TIM3_getFrequency()
{
	return TIM3_Frequency;

}

void TIM3_setPeriod(uint32_t value)
{
	TIM3_Period = value;

}
/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
	 if (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_CC1) != RESET)   // Capture Compare Event 1
			  {
			    if (__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_IT_CC1) != RESET)   // Cross check that whether source of Interrupt is configured
				// if(TIM3->DIER & TIM_DIER_CC1IE)
			    {
			        __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_CC1);
			        htim4.Channel = HAL_TIM_ACTIVE_CHANNEL_1;

			        /* Input capture event */
			        if ((htim4.Instance->CCMR1 & TIM_CCMR1_CC1S) != 0x00U) // cross check that if Input Capture Interrupt is their, if input capture event is their
			        {
			        	if (htim4.Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // If the interrupt is triggered by channel 1( for timer 4: pin PB6)
							{
								// Read the IC value
								TIM4_IC1Value = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);

								if (TIM4_IC1Value != 0)
								{
									TIM4_IC2Value = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2);
									// calculate the Duty Cycle
									TIM4_Duty = (TIM4_IC2Value *100)/TIM4_IC1Value;
									TIM4_Frequency = 100000/TIM4_IC1Value;
									TIM4_Period = 1000000/TIM4_Frequency;//(Perid is in microseconds)
									TIM4_Width = TIM4_Period/2;  //(in microseconds)

								}
							}
			        }
			    }
			  }
  /* USER CODE END TIM4_IRQn 1 */
}

float TIM4_getPeriod()
{
	return TIM4_Period;
}

float TIM4_getwidth()
{
	return TIM4_Width;
}

float TIM4_getFrequency()
{
	return TIM4_Frequency;

}

void TIM4_setPeriod(uint32_t value)
{
	TIM4_Period = value;

}
