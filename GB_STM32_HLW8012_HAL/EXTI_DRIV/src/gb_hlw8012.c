#include "stm32f1xx.h"
#include "gb_hlw8012.h"
#include "main.h"
#include "gb_timer_input_capture.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;


#define V_REF               2.43
#define R_CURRENT           0.001
#define R_VOLTAGE           2821
#define F_OSC               3579000
#define READING_INTERVAL    3000
#define PULSE_TIMEOUT       2000000

double _current_resistor = R_CURRENT;
double _voltage_resistor = R_VOLTAGE;


double _current_multiplier; // Unit: us/A
double _voltage_multiplier; // Unit: us/V
double _power_multiplier;   // Unit: us/W

unsigned long _pulse_timeout = PULSE_TIMEOUT;    //Unit: us
  volatile unsigned long _voltage_pulse_width = 0; //Unit: us
  volatile unsigned long _current_pulse_width = 0; //Unit: us
  volatile unsigned long _power_pulse_width = 0;   //Unit: us
  volatile unsigned long _pulse_count = 0;

  // variables that store the current voltage power readings from hlw8012 after calculation
  float _current = 0;
  float _voltage = 0;
  float _power = 0;

// Check values every 2 seconds
#define UPDATE_TIME                     2000

  double hlw8012_getcurrent_multiplier() { return _current_multiplier; };
    double hlw8012_getvoltage_multiplier() { return _voltage_multiplier; };

double hlw8012_getpower_multiplier() { return _power_multiplier; };

void hlw8012_sel_high_current()
{
//	HAL_GPIO_WritePin(GPIOB, GPIO_Pin, PinState)
	GPIOB->ODR |= (GPIO_ODR_ODR7);
	//GPIOC->ODR |= (GPIO_ODR_ODR13);
}
void hlw8012_sel_low_volatge()
{
	GPIOB->ODR &= ~(GPIO_ODR_ODR7);
	//GPIOC->ODR &= ~(GPIO_ODR_ODR13);
}


void gb_unblockingDelay(uint32_t mseconds)
{
	//unsigned long gb_timeIn = GB_millis();	// Timestamp coming into function
	unsigned long gb_timeIn = HAL_GetTick();	// Timestamp coming into function
	while (HAL_GetTick() - gb_timeIn < mseconds) HAL_Delay(1); // While we haven't timed out
}

float hlw8012_getcurrent()
{
	//if(_power == 0){
	//     _current_pulse_width = 0;
	//}
	//else
	{
	hlw8012_sel_high_current();
	_current_pulse_width = pulseIN_current();
	_current = (_current_pulse_width > 0) ? (_current_multiplier / _current_pulse_width )*2: 0;
	//return _current;
	}
	return _current;
}
float Current_Period = 0;
/*
 * Period is taken from TIM4_IRQ
 *
 */
float pulseIN_current()
{
unsigned long gb_pulse_current = HAL_GetTick();	// Timestamp coming into function
//tim4_ch1_capture_config();
	while (HAL_GetTick() - gb_pulse_current < 2000 )
	{
	tim3_ch2_capture_config();
		tim4_ch1_capture_config();
	//	 HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	Current_Period = TIM3_getPeriod();
	}
	//tim4_ch1_capture_unconfig();
	tim3_ch2_capture_unconfig();

	//reinitialise Period to 0
	TIM4_setPeriod(0);
	//}
	return Current_Period;

}
float hlw8012_getvoltage()
{
	hlw8012_sel_low_volatge();
	_voltage_pulse_width = pulseIN_voltage();
	_voltage = (_voltage_pulse_width > 0) ? _voltage_multiplier / _voltage_pulse_width/2: 0;
	return _voltage;
}
float voltage_Period = 0;
float pulseIN_voltage()
{
	unsigned long gb_pulse_voltage = HAL_GetTick();	// Timestamp coming into function
		while (HAL_GetTick() - gb_pulse_voltage < 2000 )
		{
		//	tim4_ch1_capture_config();
	tim3_ch2_capture_config();
		// HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	voltage_Period = TIM3_getPeriod();
		}
	//	tim4_ch1_capture_unconfig();
	tim3_ch2_capture_unconfig();

	//reinitilise Period to 0
	TIM4_setPeriod(0);
	return voltage_Period;
}
float hlw8012_getactivepower()
{
	_power_pulse_width = pulseIN_power();
	_power = (_power_pulse_width > 0) ? _power_multiplier / _power_pulse_width / 2 : 0;
	return _power;
}

float Power_Period = 0;
float pulseIN_power()
{
	unsigned long gb_pulse_voltage = HAL_GetTick();	// Timestamp coming into function
			while (HAL_GetTick() - gb_pulse_voltage < 2000 )
			{
	//tim4_ch1_capture_config();

				HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);   // main channel
	Power_Period = TIM4_getPeriod();
			}
	tim4_ch1_capture_unconfig();

	//reinitilise Period to 0
	TIM3_setPeriod(0);
	return Power_Period;
}
void hlw8012_expected_ActiveCurrent( double value)
{
	if (_current == 0) _current = hlw8012_getcurrent();
	if (_current > 0) _current_multiplier *= (value / _current);
}
void hlw8012_expected_Activevoltage(double value)
{
	 if (_voltage == 0) hlw8012_getvoltage();
	 if (_voltage > 0) _voltage_multiplier *= (((double) value / _voltage));
}
void hlw8012_expected_Activepower(unsigned int value)
{
	  if (_power == 0) hlw8012_getactivepower();
	  if (_power > 0) _power_multiplier *= ((double) value / _power);
}
void hlw8012_calculate_default_multiplier()
{
	 _current_multiplier = ( 1000000.0 * 512 * V_REF / _current_resistor / 24.0 / F_OSC );
	 _voltage_multiplier = ( 1000000.0 * 512 * V_REF * _voltage_resistor / 2.0 / F_OSC );
	 _power_multiplier = ( 1000000.0 * 128 * V_REF * V_REF * _voltage_resistor / _current_resistor / 48.0 / F_OSC );
}

void hlw8012_setResistors( double current, double voltage_upstream, double voltage_downstream)
{
	if (voltage_downstream > 0) {
	        _current_resistor = current;
	        _voltage_resistor = (voltage_upstream + voltage_downstream) / voltage_downstream;
	        hlw8012_calculate_default_multiplier();
	    }
}
float cur_current_multi = 0;
float cur_voltage_multi = 0;
float cur_power_multi = 0;


void hlw8012_calibrate()
{

//	hlw8012_getactivepower();
//
//	hlw8012_sel_high_current();
//	gb_unblockingDelay(2000);
//	hlw8012_getcurrent();

//	hlw8012_sel_low_volatge();
//	gb_unblockingDelay(2000);
//	hlw8012_getvoltage();

	hlw8012_expected_Activepower(165.0);
 hlw8012_expected_Activevoltage(226.0);
	hlw8012_expected_ActiveCurrent(0.730);

	GB_printString1(" New Current Multiplier");
	//GB_decimel1(hlw8012_getcurrent_multiplier());
	cur_current_multi = hlw8012_getcurrent_multiplier();

	GB_printString1(" New Voltage Multiplier");
	//GB_decimel1(hlw8012_getvoltage_multiplier());
	cur_voltage_multi = hlw8012_getvoltage_multiplier();

	GB_printString1(" New Power Multiplier");
	//GB_decimel1(hlw8012_getpower_multiplier());
	cur_power_multi = hlw8012_getpower_multiplier();

}
