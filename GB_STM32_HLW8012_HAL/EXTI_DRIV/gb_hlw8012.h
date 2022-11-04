/*
 * gb_hlw8012.h
 *
 *  Created on: Oct 24, 2022
 *      Author: acer
 */

#ifndef GB_HLW8012_H_
#define GB_HLW8012_H_

void hlw8012_config();
void hlw8012_pins_config();


double hlw8012_getcurrent_multiplier();
double hlw8012_getvoltage_multiplier();
double hlw8012_getpower_multiplier();


void hlw8012_sel_low_volatge();
void hlw8012_sel_high_current();
void gb_unblockingDelay(uint32_t mseconds);
void hlw8012_calibrate();
float hlw8012_getcurrent();
float pulseIN_current();
float hlw8012_getvoltage();
float pulseIN_voltage();
float hlw8012_getactivepower();
void hlw8012_setResistors( double current, double voltage_upstream, double voltage_downstream);
void hlw8012_expected_ActiveCurrent( double value);


#endif /* GB_HLW8012_H_ */
