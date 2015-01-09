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
#include "device.h"
char tc_device_link[d_string_buffer_size];
int tc_descriptor = d_rs232_null, tc_interrupt;
struct s_device_status tc_status;
struct s_device_configuration tc_configuration[] = {
	{d_true, -100.0, 200.0, "main", "main   ", e_device_temperature_main_nominal, e_device_temperature_main_actual},
	{d_true, -100.0, 200.0, "pt100_A", "pt100 A", e_device_temperature_pt100_A_nominal, e_device_temperature_pt100_A_actual},
	{d_true, -100.0, 200.0, "pt100_B", "pt100 B", e_device_temperature_pt100_B_nominal, e_device_temperature_pt100_B_actual},
	{d_true, -100.0, 200.0, "pt100_C", "pt100 C", e_device_temperature_pt100_C_nominal, e_device_temperature_pt100_C_actual},
	{d_true, -200.0, 200.0, "pt100_D", "pt100 D", e_device_temperature_pt100_D_nominal, e_device_temperature_pt100_D_actual},
	{d_true, 0, 100, "fan", "fan    ", e_device_temperature_fan_nominal, e_device_temperature_fan_actual},
	{d_false}
};
int f_device_open(int descriptor) {
	char buffer[d_string_buffer_size];
	int result = d_true;
	if (tc_descriptor == d_rs232_null) {
		if ((result = f_rs232_open(tc_device_link, e_rs232_baud_9600, e_rs232_bits_8, e_rs232_stops_1_bit, e_rs232_parity_no,
						e_rs232_flow_control_software, &tc_descriptor, NULL))) {
			memset(&(tc_status.submitted), d_true, sizeof(struct s_device_status_submitted));
			snprintf(buffer, d_string_buffer_size, "chamber VT4010 is ready and %sonline%s!\n", v_console_styles[e_console_style_green],
					v_console_styles[e_console_style_reset]);
		} else {
			snprintf(buffer, d_string_buffer_size, "chamber VT4010 is %soffline%s!\n", v_console_styles[e_console_style_red],
					v_console_styles[e_console_style_reset]);
			tc_descriptor = d_rs232_null;
		}
		if (descriptor != d_console_descriptor_null)
			write(descriptor, buffer, f_string_strlen(buffer));
	}
	return result;
}

void p_device_status_retrieve_log(void) {
	time_t current_timestamp = time(NULL);
	FILE *stream;
	if ((stream = fopen(d_death_valley_device_log, "a"))) {
		fprintf(stream, "%ld: %.02f/%.02f, %.02f/%.02f, %.02f/%.02f, %.02f/%.02f, %.02f/%.02f, %.02f/%.02f | %d%d%d\n", current_timestamp,
				tc_status.temperature[e_device_temperature_main_actual], tc_status.temperature[e_device_temperature_main_nominal],
				tc_status.temperature[e_device_temperature_pt100_A_actual], tc_status.temperature[e_device_temperature_pt100_A_nominal],
				tc_status.temperature[e_device_temperature_pt100_B_actual], tc_status.temperature[e_device_temperature_pt100_B_nominal],
				tc_status.temperature[e_device_temperature_pt100_C_actual], tc_status.temperature[e_device_temperature_pt100_C_nominal],
				tc_status.temperature[e_device_temperature_pt100_D_actual], tc_status.temperature[e_device_temperature_pt100_D_nominal],
				tc_status.temperature[e_device_temperature_fan_actual], tc_status.temperature[e_device_temperature_fan_nominal],
				tc_status.flag[e_device_flag_start], tc_status.flag[e_device_flag_dehumidifier], tc_status.flag[e_device_flag_co2]);
		fclose(stream);
	}
}

void p_device_status_retrieve(void) {
	static unsigned char *device_status = "$00I\r\n";
	unsigned char buffer[d_death_valley_device_size], *pointer, *next;
	int readed, done = d_false, tries = 0, index = 0, step = 0;
	if (tc_descriptor != d_rs232_null)
		do {
			if ((f_rs232_write(tc_descriptor, device_status, f_string_strlen(device_status))) > 0) {
				memset(buffer, 0, d_death_valley_device_size);
				if (((readed = f_rs232_read_packet(tc_descriptor, buffer, d_death_valley_device_size,  d_death_valley_device_timeout, NULL,
								NULL, 0)) > 0) && (readed == d_death_valley_device_answer_size)) {
					pointer = buffer;
					while ((step < e_device_temperature_null) && (next = strchr(pointer, ' '))) {
						*next = '\0';
						if (tc_status.submitted.temperature[step])
							tc_status.temperature[step] = atof(pointer);
						step++;
						pointer = (next+1);
					}
					if (f_string_strlen(pointer) > e_device_flag_null)
						for (index = 0, step = 0; index < e_device_flag_null; ++index) {
							if (*pointer != ' ') {
								if (tc_status.submitted.flag[step])
									tc_status.flag[step] = (*pointer=='1')?d_true:d_false;
								step++;
							}
							pointer++;
						}
					p_device_status_retrieve_log();
					done = d_true;
				}
			}
		} while ((!done) && (tries++ < d_death_valley_device_tries));
}

void p_device_status_temperature(const char *kind, enum e_device_temperatures actual, enum e_device_temperatures nominal, int output) {
	char message[d_string_buffer_size];
	if (output != d_console_descriptor_null) {
		snprintf(message, d_string_buffer_size, "[%s]\t\t%s%.02f%s/%s%.02f%s\t[%c]\n", kind, v_console_styles[e_console_style_bold],
				tc_status.temperature[actual], v_console_styles[e_console_style_reset], v_console_styles[e_console_style_bold],
				tc_status.temperature[nominal], v_console_styles[e_console_style_reset], (!tc_status.submitted.temperature[nominal])?'*':'-');
		write(output, message, f_string_strlen(message));
	}
}

void p_device_status_flag(const char *kind, enum e_device_flags flag, int output) {
	char message[d_string_buffer_size];
	if (output != d_console_descriptor_null) {
		snprintf(message, d_string_buffer_size, "[%s]\t%s%s%s\t\t[%c]\n", kind,
				(tc_status.flag[flag])?v_console_styles[e_console_style_green]:v_console_styles[e_console_style_red],
				(tc_status.flag[flag])?"ON":"OFF", v_console_styles[e_console_style_reset], (!tc_status.submitted.flag[flag])?'*':'-');
		write(output, message, f_string_strlen(message));
	}
}

int f_device_status(char **tokens, size_t element, int output) {
	int index, result = d_true;
	for (index = 0; tc_configuration[index].initialized; ++index)
		p_device_status_temperature(tc_configuration[index].description, tc_configuration[index].actual_temperature, tc_configuration[index].temperature,
				output);
	p_device_status_flag("thermal chamber", e_device_flag_start, output);
	p_device_status_flag("dehumidifier   ", e_device_flag_dehumidifier, output);
	p_device_status_flag("CO2 cool       ", e_device_flag_co2, output);
	return result;
}

int f_device_configure(char **tokens, size_t elements, int output) {
	static unsigned char *device_configure = "$00E", *device_rc = "\r\n";
	unsigned char buffer_command[d_death_valley_device_size], buffer[d_death_valley_device_size], message[d_string_buffer_size], *pointer;
	int index, done = d_false, tries = 0, result = d_true;
	if (tc_descriptor != d_rs232_null) {
		memset(buffer_command, 0, d_death_valley_device_size);
		snprintf(buffer_command, d_death_valley_device_size, "%s %06.01f %06.01f %06.01f %06.01f %06.01f %06.01f 0000000000000000%s",
				device_configure, tc_status.temperature[e_device_temperature_main_nominal],
				tc_status.temperature[e_device_temperature_pt100_A_nominal], tc_status.temperature[e_device_temperature_pt100_B_nominal],
				tc_status.temperature[e_device_temperature_pt100_C_nominal], tc_status.temperature[e_device_temperature_pt100_D_nominal],
				tc_status.temperature[e_device_temperature_fan_nominal], device_rc);
		pointer = buffer_command+d_death_valley_device_configure_prefix;
		for (index = 0; index < e_device_flag_null; ++pointer, ++index)
			*pointer = (tc_status.flag[index])?'1':'0';
		do {
			if (f_rs232_write(tc_descriptor, buffer_command, f_string_strlen(buffer_command)+d_death_valley_device_answer_extra_tail) > 0)
				if (f_rs232_read_packet(tc_descriptor, buffer, d_death_valley_device_size, d_death_valley_device_timeout, NULL, NULL, 0) > 0) {
					memset(&(tc_status.submitted), d_true, sizeof(struct s_device_status_submitted));
					if (output != d_console_descriptor_null) {
						snprintf(message, d_string_buffer_size, "a new configuration has been written:\n%s%s%s",
								v_console_styles[e_console_style_bold], buffer_command,
								v_console_styles[e_console_style_reset]);
						write(output, message, f_string_strlen(message));
					}
					done = d_true;
				}
		} while ((!done) && (tries++ < d_death_valley_device_tries));
	}
	return result;
}

int p_device_set_key(const char *raw_key, float value, int output) {
	char message[d_string_buffer_size];
	int index, result = d_death_valley_device_configuration_null;
	for (index = 0; tc_configuration[index].initialized; ++index)
		if (f_string_strcmp(tc_configuration[index].value, raw_key) == 0) {
			if ((value >= tc_configuration[index].low_range) && (value < tc_configuration[index].top_range))
				result = index;
			else if (output != d_console_descriptor_null) {
				snprintf(message, d_string_buffer_size, "the value is out of range (%.02f vs a range of %.02f - %.02f)\n", value,
						tc_configuration[index].low_range, tc_configuration[index].top_range);
				write(output, message, f_string_strlen(message));
			}
			break;
		}
	if ((!tc_configuration[index].initialized) && (output != d_console_descriptor_null)) {
		snprintf(message, d_string_buffer_size, "sorry, I can't understand what '%s' is\n", raw_key);
		write(output, message, f_string_strlen(message));
	}
	return result;
}

int p_device_set_flag(char **tokens, size_t elements, int output, enum e_device_flags flag) {
	char message[d_string_buffer_size], *flag_descriptions[] = {
		"empty",
		"chamber",
		"error",
		"temperature",
		"dehumidifier",
		"co2 cool",
		"empty",
		"empty",
		"empty",
		"empty",
		"empty",
		"temperature HI",
		"temperature UN",
		"temperature SP",
		"empty",
		"empty"
	};
	int result = d_true;
	if ((f_console_parameter("-off", tokens, elements, d_true)) >= 0) {
		tc_status.flag[flag] = d_false;
		tc_status.submitted.flag[flag] = d_false;
		snprintf(message, d_string_buffer_size, "%s is %sOFF%s\n", flag_descriptions[flag], v_console_styles[e_console_style_red],
				v_console_styles[e_console_style_reset]);
	} else if ((f_console_parameter("-on", tokens, elements, d_true)) >= 0) {
		tc_status.flag[flag] = d_true;
		tc_status.submitted.flag[flag] = d_false;
		snprintf(message, d_string_buffer_size, "%s is %sON%s\n", flag_descriptions[flag], v_console_styles[e_console_style_green],
				v_console_styles[e_console_style_reset]);
	} else {
		result = d_false;
		snprintf(message, d_string_buffer_size, "neither -off nor -on has been specified for entry %s\n", flag_descriptions[flag]);
	}
	if (output != d_console_descriptor_null)
		write(output, message, f_string_strlen(message));
	return result;
}

int f_device_set(char **tokens, size_t elements, int output) {
	char message[d_string_buffer_size], *raw_key = tokens[f_console_parameter("-k", tokens, elements, d_false)],
	     *raw_value = tokens[f_console_parameter("-v", tokens, elements, d_false)];
	float value = atof(raw_value);
	int index, result = d_false;
	if ((index = p_device_set_key(raw_key, value, output)) != d_death_valley_device_configuration_null) {
		tc_status.temperature[tc_configuration[index].temperature] = value;
		tc_status.submitted.temperature[tc_configuration[index].temperature] = d_false;
		if (output != d_console_descriptor_null) {
			snprintf(message, d_string_buffer_size, "value %s is now %.02f\n", raw_key, value);
			write(output, message, f_string_strlen(message));
		}
		result = d_true;
	}
	return result;
}

int f_device_set_chamber(char **tokens, size_t elements, int output) {
	return p_device_set_flag(tokens, elements, output, e_device_flag_start);
}

int f_device_set_dehumidifier(char **tokens, size_t elements, int output) {
	return p_device_set_flag(tokens, elements, output, e_device_flag_dehumidifier);
}

int f_device_set_co2(char **tokens, size_t elements, int output) {
	return p_device_set_flag(tokens, elements, output, e_device_flag_co2);
}

int f_device_test(char **tokens, size_t elements, int output) {
	char message[d_string_buffer_size], *raw_key = tokens[f_console_parameter("-k", tokens, elements, d_false)];
	enum e_device_conditions current_condition = e_device_condition_equal_to;
	float value, actual_value;
	int index, pointer, satisfied = d_false, wait_condition = (f_console_parameter("-w", tokens, elements, d_true) >= 0), result = d_false;
	if ((((pointer = f_console_parameter("-bt", tokens, elements, d_false)) >= 0) && (current_condition = e_device_condition_bigger_than)) ||
			(((pointer = f_console_parameter("-lt", tokens, elements, d_false)) >= 0) && (current_condition = e_device_condition_lower_than)) ||
			(((pointer = f_console_parameter("-et", tokens, elements, d_false)) >= 0) && (current_condition = e_device_condition_equal_to))) {
		value = atof(tokens[pointer]);
		if ((index = p_device_set_key(raw_key, value, output)) != d_death_valley_device_configuration_null) {
			tc_interrupt = d_false;
			while ((!tc_interrupt) && (!satisfied)) {
				p_device_status_retrieve();
				actual_value = tc_status.temperature[tc_configuration[index].actual_temperature];
				if (((current_condition == e_device_condition_bigger_than) && (actual_value > value)) ||
						((current_condition == e_device_condition_lower_than) && (actual_value < value)) ||
						((current_condition == e_device_condition_equal_to) && (actual_value == value)))
					satisfied = d_true;
				if ((!satisfied) && (wait_condition)) {
					usleep(d_death_valley_device_wait);
					continue;
				}
				break;
			}
			if (output != d_console_descriptor_null) {
				snprintf(message, d_string_buffer_size, "%s\n", (satisfied)?"true":"false");
				write(output, message, f_string_strlen(message));
			}
			result = d_true;
		}
	} else if (output != d_console_descriptor_null) {
		snprintf(message, d_string_buffer_size, "no condition (-bt, -lt or -et) has been selected\n");
		write(output, message, f_string_strlen(message));
	}
	return result;
}

void f_device_close(void) {
	if (tc_descriptor != d_rs232_null) {
		f_rs232_close(tc_descriptor);
		tc_descriptor = d_rs232_null;
	}
}
