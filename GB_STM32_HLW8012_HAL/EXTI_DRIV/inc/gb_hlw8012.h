/*
 * gb_hlw8012.h
 *
 *  Created on: Oct 24, 2022
 *      Author: acer
 */

#ifndef GB_HLW8012_H_
#define GB_HLW8012_H_


double hlw8012_getcurrent_multiplier();
double hlw8012_getvoltage_multiplier();
double hlw8012_getpower_multiplier();

void hlw8012_sel_high_current();
void hlw8012_sel_low_volatge();
void gb_unblockingDelay(uint32_t mseconds);
float hlw8012_getcurrent();
float pulseIN_current();
float hlw8012_getvoltage();
float pulseIN_voltage();
float hlw8012_getactivepower();
float pulseIN_power();
//void hlw8012_config();
//void hlw8012_pins_config();
void hlw8012_expected_ActiveCurrent( double value);
void hlw8012_expected_Activevoltage(double value);
void hlw8012_expected_Activepower(unsigned int value);
void hlw8012_calculate_default_multiplier();
void hlw8012_setResistors( double current, double voltage_upstream, double voltage_downstream);

void hlw8012_calibrate();


//double hlw8012_getcurrent_multiplier();
//double hlw8012_getvoltage_multiplier();
//double hlw8012_getpower_multiplier();
//
//void gb_unblockingDelay(uint32_t mseconds);
////void hlw8012_calibrate();
//float hlw8012_getcurrent();
//float pulseIN_current();
//float hlw8012_getvoltage();
//float pulseIN_voltage();
//float hlw8012_getactivepower();
//void hlw8012_setResistors( double current, double voltage_upstream, double voltage_downstream);
//void hlw8012_expected_ActiveCurrent( double value);
//
//
//void hlw8012_calibrate();

#endif /* GB_HLW8012_H_ */
