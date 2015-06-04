/*
 * death_valley
 * Copyright (C) 2015 Andrea Nardinocchi (andrea@nardinan.it)
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
#ifndef death_valley_chamber_device_h
#define death_valley_chamber_device_h
#include <miranda/ground.h>
#define d_chamber_device_entries 1
#define d_chamber_device_refresh_timeout 3 	/* seconds */
#define d_chamber_device_condition_timeout 3	/* seconds */
#define d_chamber_device_batch_timeout 1	/* seconds */
#define d_chamber_device_timeout 75000		/* useconds */
#define d_chamber_device_current 0
#define d_chamber_device_defined 1
#define d_chamber_device_message_size 128
#define d_chamber_device_answer_size 104
#define d_chamber_device_configure_prefix 47
#define d_chamber_device_configure_postfix 3
#define d_chamber_device_configuration_null -1
#define d_chamber_device_batches 1024
typedef enum e_chamber_device_temperatures {
	e_chamber_device_temperature_main_nominal = 0,
	e_chamber_device_temperature_main_actual,
	e_chamber_device_temperature_pt100_A_nominal,
	e_chamber_device_temperature_pt100_A_actual,
	e_chamber_device_temperature_pt100_B_nominal,
	e_chamber_device_temperature_pt100_B_actual,
	e_chamber_device_temperature_pt100_C_nominal,
	e_chamber_device_temperature_pt100_C_actual,
	e_chamber_device_temperature_pt100_D_nominal,
	e_chamber_device_temperature_pt100_D_actual,
	e_chamber_device_temperature_fan_nominal,
	e_chamber_device_temperature_fan_actual,
	e_chamber_device_temperature_null
} e_chamber_device_temperatures;
typedef enum e_chamber_device_flags {
	e_chamber_device_flag_empty_A = 0,
	e_chamber_device_flag_start,
	e_chamber_device_flag_error,
	e_chamber_device_flag_temperature,
	e_chamber_device_flag_dehumidifier,
	e_chamber_device_flag_co2,
	e_chamber_device_flag_free_out1,
	e_chamber_device_flag_free_out2,
	e_chamber_device_flag_free_in1,
	e_chamber_device_flag_free_in2,
	e_chamber_device_flag_free_in3,
	e_chamber_device_flag_adjustment_temperature_HI,
	e_chamber_device_flag_adjustment_temperature_UN,
	e_chamber_device_flag_adjustment_temperature_SP,
	e_chamber_device_flag_empty_B,
	e_chamber_device_flag_empty_C,
	e_chamber_device_flag_null
} e_chamber_device_flags;
typedef enum e_chamber_device_tests {
	e_chamber_device_test_bigger_than = 1,
	e_chamber_device_test_lower_than,
	e_chamber_device_test_equal_to
} e_chamber_device_tests;
typedef struct s_chamber_device_condition {
	int running, parameter_pointer;
	float value;
	enum e_chamber_device_tests test;
} s_chamber_device_condition;
typedef struct s_chamber_device {
	/* do not touch */
	int descriptor;
	char link[d_string_buffer_size], batches[d_chamber_device_batches][d_string_buffer_size];
	/* ok, now you can add your values */
	float temperature[2][e_chamber_device_temperature_null];
	int flag[2][e_chamber_device_flag_null], batch_pointer;
	struct s_chamber_device_condition condition;
	struct termios old_configuration;
	time_t last_refresh, last_condition, last_batch, active_from;
} s_chamber_device;
typedef struct s_chamber_device_parameters {
	int initialized;
	float low_range, top_range;
	char *value;
	enum e_chamber_device_temperatures temperature, actual_temperature;
} s_chamber_device_parameters;
extern struct s_chamber_device v_chamber_device[d_chamber_device_entries];
extern struct s_chamber_device_parameters v_chamber_parameters[];
extern void f_chamber_device_description(unsigned char code, char *destination, size_t size);
extern void p_chamber_device_status_temperature(unsigned char code, enum e_chamber_device_temperatures actual_temperature,
		enum e_chamber_device_temperatures nominal_temperature, char *description, char *destination, size_t size);
extern void p_chamber_device_status_flag(unsigned char code, enum e_chamber_device_flags flag, char *description, char *destination, size_t size);
extern int f_chamber_device_status(unsigned char code, char **tokens, size_t elements, int output);
extern int f_chamber_device_apply(unsigned char code, char **tokens, size_t elements, int output);
extern int p_chamber_device_set_key(const char *raw_key, float value, int output);
extern int f_chamber_device_set(unsigned char code, char **tokens, size_t elements, int output);
extern int p_chamber_device_test_condition(unsigned char code);
extern int f_chamber_device_test(unsigned char code, char **tokens, size_t elements, int output);
extern int p_chamber_device_set_flag(unsigned char code, enum e_chamber_device_flags flag, char **tokens, size_t elements, int output);
extern int f_chamber_device_set_chamber(unsigned char code, char **tokens, size_t elements, int output);
extern int f_chamber_device_set_dehumidifier(unsigned char code, char **tokens, size_t elements, int output);
extern int f_chamber_device_set_co2(unsigned char code, char **tokens, size_t elements, int output);
extern int f_chamber_device_load(unsigned char code, char **tokens, size_t elements, int output);
extern int f_chamber_device_sleep(unsigned char code, char **tokens, size_t elements, int output);
extern int f_chamber_device_enabled(unsigned char code);
extern int f_chamber_device_initialize(unsigned char code);
extern int f_chamber_device_destroy(unsigned char code);
extern int p_chamber_device_refresh_status(unsigned char code);
extern void p_chamber_device_refresh_console(unsigned char code, struct s_console *console, time_t current_timestamp);
extern void p_chamber_device_refresh_batches(unsigned char code, struct s_console *console, time_t current_timestamp);
extern int f_chamber_device_refresh(unsigned char code, struct s_console *console);
#endif
