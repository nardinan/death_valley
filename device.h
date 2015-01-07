/*
 * death_valley
 * Copyright (C) 2014 Andrea Nardinocchi (andrea@nardinan.it)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef death_valley_device_h
#define death_valley_device_h
#include <miranda/ground.h>
#include "commands.h"
#define d_death_valley_device "/dev/tty.usbserial"
#define d_death_valley_device_size 128
#define d_death_valley_device_timeout 1000
#define d_death_valley_device_sensors 4
#define d_death_valley_device_configure_prefix 47
#define d_death_valley_device_wait 500000
#define d_death_valley_device_configuration_null -1
extern int tc_descriptor;
typedef enum e_device_temperatures {
	e_device_temperature_main_nominal = 0,
	e_device_temperature_main_actual,
	e_device_temperature_pt100_A_nominal,
	e_device_temperature_pt100_A_actual,
	e_device_temperature_pt100_B_nominal,
	e_device_temperature_pt100_B_actual,
	e_device_temperature_pt100_C_nominal,
	e_device_temperature_pt100_C_actual,
	e_device_temperature_pt100_D_nominal,
	e_device_temperature_pt100_D_actual,
	e_device_temperature_fan_nominal,
	e_device_temperature_fan_actual,
	e_device_temperature_null
} e_device_temperatures;
typedef enum e_device_flags {
	e_device_flag_empty_A = 0,
	e_device_flag_start,
	e_device_flag_error,
	e_device_flag_temperature,
	e_device_flag_dehumidifier,
	e_device_flag_co2,
	e_device_flag_free_out1,
	e_device_flag_free_out2,
	e_device_flag_free_in1,
	e_device_flag_free_in2,
	e_device_flag_free_in3,
	e_device_flag_adjustment_temperature_HI,
	e_device_flag_adjustment_temperature_UN,
	e_device_flag_adjustment_temperature_SP,
	e_device_flag_empty_B,
	e_device_flag_empty_C,
	e_device_flag_null
} e_device_flags;
typedef enum e_device_conditions {
	e_device_condition_bigger_than = 1,
	e_device_condition_lower_than,
	e_device_condition_equal_to
} e_device_conditions;
typedef struct s_device_status_submitted {
	int temperature[e_device_temperature_null], flag[e_device_flag_null];
} s_device_status_submitted;
typedef struct s_device_status {
	float temperature[e_device_temperature_null];
	int flag[e_device_flag_null];
	struct s_device_status_submitted submitted;
} s_device_status;
typedef struct s_device_configuration {
	int initialized;
	float low_range, top_range;
	char *value, *description;
	enum e_device_temperatures temperature, actual_temperature;
} s_device_configuration;
extern int tc_descriptor, tc_interrupt, tc_show;
extern struct s_device_status tc_status;
extern struct s_device_configuration tc_configuration[];
extern int f_device_open(int descriptor);
extern void p_device_status_retrieve(void);
extern void p_device_status_temperature(const char *kind, enum e_device_temperatures actual, enum e_device_temperatures nominal, int output);
extern void p_device_status_flag(const char *kind, enum e_device_flags flag, int output);
extern int f_device_status(char **tokens, size_t elements, int output);
extern int f_device_configure(char **tokens, size_t elements, int output);
extern int p_device_set_key(const char *raw_key, float value, int output);
extern int p_device_set_flag(char **tokens, size_t elements, int output, enum e_device_flags flag);
extern int f_device_set(char **tokens, size_t elements, int output);
extern int f_device_set_chamber(char **tokens, size_t elements, int output);
extern int f_device_set_dehumidifier(char **tokens, size_t elements, int output);
extern int f_device_set_co2(char **tokens, size_t elements, int output);
extern int f_device_test(char **tokens, size_t elements, int output);
extern void f_device_close(void);
#endif
