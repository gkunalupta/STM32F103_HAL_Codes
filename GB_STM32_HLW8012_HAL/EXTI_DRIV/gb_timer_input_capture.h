/*
 * gb_timer_input_capture.h
 *
 *  Created on: Nov 5, 2022
 *      Author: acer
 */

#ifndef GB_TIMER_INPUT_CAPTURE_H_
#define GB_TIMER_INPUT_CAPTURE_H_



void tim4_ch1_capture_config();
void tim4_ch1_capture_unconfig();


float TIM4_getPeriod();
float TIM4_getwidth();
float TIM4_getFrequency();

void tim3_ch2_capture_config();
void tim3_ch2_capture_unconfig();


float TIM3_getPeriod();
float TIM3_getwidth();
float TIM3_getFrequency();

#endif /* GB_TIMER_INPUT_CAPTURE_H_ */
