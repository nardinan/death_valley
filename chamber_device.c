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
#include "chamber_device.h"
struct s_chamber_device v_chamber_device[d_chamber_device_entries] = {
	{ d_rs232_null, "/dev/ttyUSB0"}
};
struct s_chamber_device_parameters v_chamber_parameters[] = {
	{d_true, -100.0, 200.0, "main", e_chamber_device_temperature_main_nominal, e_chamber_device_temperature_main_actual},
	{d_true, -100.0, 200.0, "pt100_A", e_chamber_device_temperature_pt100_A_nominal, e_chamber_device_temperature_pt100_A_actual},
	{d_true, -100.0, 200.0, "pt100_B", e_chamber_device_temperature_pt100_B_nominal, e_chamber_device_temperature_pt100_B_actual},
	{d_true, -100.0, 200.0, "pt100_C", e_chamber_device_temperature_pt100_C_nominal, e_chamber_device_temperature_pt100_C_actual},
	{d_true, -200.0, 200.0, "pt100_D", e_chamber_device_temperature_pt100_D_nominal, e_chamber_device_temperature_pt100_D_actual},
	{d_true, 0, 100, "fan", e_chamber_device_temperature_fan_nominal, e_chamber_device_temperature_fan_actual},
	{d_false}
};
void f_chamber_device_description(unsigned char code, char *destination, size_t size) {
	char status[d_string_buffer_size], temperature[d_string_buffer_size];
	snprintf(destination, size, "#%d [%s VT4010] ", code, v_chamber_device[code].link);
	if (f_chamber_device_initialize(code)) {
		if (v_chamber_device[code].flag[d_chamber_device_current][e_chamber_device_flag_start]) {
			snprintf(status, d_string_buffer_size, "[connected: %sON%s] ", v_console_styles[e_console_style_green],
					v_console_styles[e_console_style_reset]);
			if (v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_main_nominal] !=
					v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_main_actual])
				snprintf(temperature, d_string_buffer_size, "%s%6.01f%s/%6.01f", v_console_styles[e_console_style_yellow],
						v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_main_actual],
						v_console_styles[e_console_style_reset],
						v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_main_nominal]);
			else
				snprintf(temperature, d_string_buffer_size, "%s%6.01f%sC/%6.01f", v_console_styles[e_console_style_green],
						v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_main_actual],
						v_console_styles[e_console_style_reset],
						v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_main_nominal]);
			strncat(status, temperature, (d_string_buffer_size-f_string_strlen(status)));
		} else
			snprintf(status, d_string_buffer_size, "[connected: %sOFF%s]", v_console_styles[e_console_style_red],
					v_console_styles[e_console_style_reset]);

	} else
		snprintf(status, d_string_buffer_size, "[%soffline%s]", v_console_styles[e_console_style_red], v_console_styles[e_console_style_reset]);
	strncat(destination, status, (size-f_string_strlen(destination)));
}

void p_chamber_device_status_temperature(unsigned char code, enum e_chamber_device_temperatures actual_temperature,
		enum e_chamber_device_temperatures nominal_temperature, char *description, char *destination, size_t size) {
	char values[d_string_buffer_size], argument[d_string_buffer_size];
	if (v_chamber_device[code].temperature[d_chamber_device_current][actual_temperature] !=
			v_chamber_device[code].temperature[d_chamber_device_current][nominal_temperature])
		snprintf(values, d_string_buffer_size, "%s%6.01f%s/%s%6.01f%s", v_console_styles[e_console_style_yellow],
				v_chamber_device[code].temperature[d_chamber_device_current][actual_temperature], v_console_styles[e_console_style_reset],
				v_console_styles[e_console_style_bold], v_chamber_device[code].temperature[d_chamber_device_current][nominal_temperature],
				v_console_styles[e_console_style_reset]);
	else
		snprintf(values, d_string_buffer_size, "%6.01f/%s%6.01f%s", v_chamber_device[code].temperature[d_chamber_device_current][actual_temperature],
				v_console_styles[e_console_style_bold], v_chamber_device[code].temperature[d_chamber_device_current][nominal_temperature],
				v_console_styles[e_console_style_reset]);
	if (v_chamber_device[code].temperature[d_chamber_device_defined][nominal_temperature] !=
			v_chamber_device[code].temperature[d_chamber_device_current][nominal_temperature])
		snprintf(argument, d_string_buffer_size, "%s%s%6.01f%s", v_console_styles[e_console_style_blink], v_console_styles[e_console_style_red],
				v_chamber_device[code].temperature[d_chamber_device_defined][nominal_temperature], v_console_styles[e_console_style_reset]);
	else
		snprintf(argument, d_string_buffer_size, "%s%6.01f%s", v_console_styles[e_console_style_green],
				v_chamber_device[code].temperature[d_chamber_device_defined][nominal_temperature], v_console_styles[e_console_style_reset]);
	snprintf(destination, size, "\t[%s%-8s%s %s | %s]\n", v_console_styles[e_console_style_bold], description, v_console_styles[e_console_style_reset],
			values, argument);
}

void p_chamber_device_status_flag(unsigned char code, enum e_chamber_device_flags flag, char *description, char *destination, size_t size) {
	if (v_chamber_device[code].flag[flag])
		snprintf(destination, size, "\t[%s%-16s%s %sON %s]\n", v_console_styles[e_console_style_bold], description,
				v_console_styles[e_console_style_reset], v_console_styles[e_console_style_green], v_console_styles[e_console_style_reset]);
	else
		snprintf(destination, size, "\t[%s%-16s%s %sOFF%s]\n", v_console_styles[e_console_style_bold], description,
				v_console_styles[e_console_style_reset], v_console_styles[e_console_style_red], v_console_styles[e_console_style_reset]);
}

int f_chamber_device_status(unsigned char code, char **tokens, size_t elements, int output) {
	char temperatures[d_console_output_size], flags[d_console_output_size];
	int result = d_true;
	p_chamber_device_refresh_status(code);
	if (output != d_console_descriptor_null) {
		snprintf(temperatures, d_console_output_size, "%stemperatures%s:\n", v_console_styles[e_console_style_yellow],
				v_console_styles[e_console_style_reset]);
		p_chamber_device_status_temperature(code, e_chamber_device_temperature_main_actual, e_chamber_device_temperature_main_nominal, "main",
				(temperatures+f_string_strlen(temperatures)), (d_console_output_size-f_string_strlen(temperatures)));
		p_chamber_device_status_temperature(code, e_chamber_device_temperature_pt100_A_actual, e_chamber_device_temperature_pt100_A_nominal, "PT100_A",
				(temperatures+f_string_strlen(temperatures)), (d_console_output_size-f_string_strlen(temperatures)));
		p_chamber_device_status_temperature(code, e_chamber_device_temperature_pt100_B_actual, e_chamber_device_temperature_pt100_B_nominal, "PT100_B",
				(temperatures+f_string_strlen(temperatures)), (d_console_output_size-f_string_strlen(temperatures)));
		p_chamber_device_status_temperature(code, e_chamber_device_temperature_pt100_C_actual, e_chamber_device_temperature_pt100_C_nominal, "PT100_C",
				(temperatures+f_string_strlen(temperatures)), (d_console_output_size-f_string_strlen(temperatures)));
		p_chamber_device_status_temperature(code, e_chamber_device_temperature_pt100_D_actual, e_chamber_device_temperature_pt100_D_nominal, "PT100_D",
				(temperatures+f_string_strlen(temperatures)), (d_console_output_size-f_string_strlen(temperatures)));
		p_chamber_device_status_temperature(code, e_chamber_device_temperature_fan_actual, e_chamber_device_temperature_fan_nominal, "fan",
				(temperatures+f_string_strlen(temperatures)), (d_console_output_size-f_string_strlen(temperatures)));
		write(output, temperatures, f_string_strlen(temperatures));
		snprintf(flags, d_console_output_size, "\n%sflags%s:\n", v_console_styles[e_console_style_yellow], v_console_styles[e_console_style_reset]);
		p_chamber_device_status_flag(code, e_chamber_device_flag_start, "chamber", (flags+f_string_strlen(flags)),
				(d_console_output_size-f_string_strlen(flags)));
		p_chamber_device_status_flag(code, e_chamber_device_flag_dehumidifier, "dehumidifier", (flags+f_string_strlen(flags)),
				(d_console_output_size-f_string_strlen(flags)));
		p_chamber_device_status_flag(code, e_chamber_device_flag_co2, "CO2", (flags+f_string_strlen(flags)),
				(d_console_output_size-f_string_strlen(flags)));
		write(output, flags, f_string_strlen(flags));
		fsync(output);
	}
	return result;
}

int f_chamber_device_apply(unsigned char code, char **tokens, size_t elements, int output) {
	static unsigned char *chamber_configure = "$00E", *device_rc = "\r\n";
	unsigned char buffer_command[d_chamber_device_message_size], buffer[d_chamber_device_message_size], *pointer;
	int result = d_false, index;
	if (f_chamber_device_initialize(code)) {
		memset(buffer_command, 0, d_chamber_device_message_size);
		snprintf(buffer_command, d_chamber_device_message_size, "%s %06.01f %06.01f %06.01f %06.01f %06.01f %06.01f 0000000000000000%s",
				chamber_configure, v_chamber_device[code].temperature[d_chamber_device_defined][e_chamber_device_temperature_main_nominal],
				v_chamber_device[code].temperature[d_chamber_device_defined][e_chamber_device_temperature_pt100_A_nominal],
				v_chamber_device[code].temperature[d_chamber_device_defined][e_chamber_device_temperature_pt100_B_nominal],
				v_chamber_device[code].temperature[d_chamber_device_defined][e_chamber_device_temperature_pt100_C_nominal],
				v_chamber_device[code].temperature[d_chamber_device_defined][e_chamber_device_temperature_pt100_D_nominal],
				v_chamber_device[code].temperature[d_chamber_device_defined][e_chamber_device_temperature_fan_nominal], device_rc);
		pointer = (buffer_command+d_chamber_device_configure_prefix);
		for (index = 0; index < e_chamber_device_flag_null; ++pointer, ++index)
			*pointer = (v_chamber_device[code].flag[d_chamber_device_defined][index])?'1':'0';
		if (f_rs232_write(v_chamber_device[code].descriptor, buffer_command, f_string_strlen(buffer_command)+d_chamber_device_configure_postfix) > 0)
			if (f_rs232_read_packet(v_chamber_device[code].descriptor, buffer, d_chamber_device_message_size, d_chamber_device_timeout,
						NULL, NULL, 0) > 0)
				result = d_true;
		if (!result)
			f_chamber_device_destroy(code);
	}
	return result;
}

int p_chamber_device_set_key(const char *raw_key, float value, int output) {
	char message[d_console_output_size];
	int index, result = d_chamber_device_configuration_null;
	for (index = 0; v_chamber_parameters[index].initialized; ++index)
		if (f_string_strcmp(v_chamber_parameters[index].value, raw_key) == 0) {
			if ((value >= v_chamber_parameters[index].low_range) && (value <= v_chamber_parameters[index].top_range))
				result = index;
			else if (output != d_console_descriptor_null) {
				snprintf(message, d_console_output_size, "the value is out of range (%.02f vs a range of %.02f - %.02f)\n", value,
						v_chamber_parameters[index].low_range, v_chamber_parameters[index].top_range);
				write(output, message, f_string_strlen(message));
			}
			break;
		}
	if ((!v_chamber_parameters[index].initialized) && (output != d_console_descriptor_null)) {
		snprintf(message, d_console_output_size, "sorry, I can't understand what '%s' is\n", raw_key);
		write(output, message, f_string_strlen(message));
	}
	fsync(output);
	return result;
}

int f_chamber_device_set(unsigned char code, char **tokens, size_t elements, int output) {
	char message[d_console_output_size], *raw_key = tokens[f_console_parameter("-k", tokens, elements, d_false)],
	     *raw_value = tokens[f_console_parameter("-v", tokens, elements, d_false)];
	float value;
	int index, result = d_false;
	value = atof(raw_value);
	if ((index = p_chamber_device_set_key(raw_key, value, output)) != d_chamber_device_configuration_null) {
		v_chamber_device[code].temperature[d_chamber_device_defined][v_chamber_parameters[index].temperature] = value;
		if (output != d_console_descriptor_null) {
			snprintf(message, d_console_output_size, "value %s is now %.01f\n", raw_key, value);
			write(output, message, f_string_strlen(message));
			fsync(output);
		}
		result = d_true;
	}
	return result;
}

int p_chamber_device_test_condition(unsigned char code) {
	float actual_value;
	int result = d_false;
	p_chamber_device_refresh_status(code);
	actual_value = v_chamber_device[code].temperature[d_chamber_device_current][v_chamber_parameters[v_chamber_device[code].condition.parameter_pointer].temperature];
	if (((v_chamber_device[code].condition.test == e_chamber_device_test_bigger_than) && (actual_value > v_chamber_device[code].condition.value)) ||
			((v_chamber_device[code].condition.test == e_chamber_device_test_lower_than) && (actual_value < v_chamber_device[code].condition.value)) ||
			((v_chamber_device[code].condition.test == e_chamber_device_test_equal_to) && (actual_value == v_chamber_device[code].condition.value)))
		result = d_true;
	return result;
}

int f_chamber_device_test(unsigned char code, char **tokens, size_t elements, int output) {
	char message[d_console_output_size], *raw_key = tokens[f_console_parameter("-k", tokens, elements, d_false)];
	enum e_chamber_device_tests current_test;
	int pointer, result = d_false;
	if ((((pointer = f_console_parameter("-bt", tokens, elements, d_false)) >= 0) && (current_test = e_chamber_device_test_bigger_than)) ||
			(((pointer = f_console_parameter("-lt", tokens, elements, d_false)) >= 0) && (current_test = e_chamber_device_test_lower_than)) ||
			(((pointer = f_console_parameter("-et", tokens, elements, d_false)) >= 0) && (current_test = e_chamber_device_test_equal_to))) {
		v_chamber_device[code].condition.parameter_pointer = pointer;
		v_chamber_device[code].condition.test = current_test;
		v_chamber_device[code].condition.value = atof(tokens[pointer]);
		if ((v_chamber_device[code].condition.parameter_pointer = p_chamber_device_set_key(raw_key, v_chamber_device[code].condition.value, output)) !=
				d_chamber_device_configuration_null) {
			if (f_console_parameter("-w", tokens, elements, d_true) >= 0)
				v_chamber_device[code].condition.running = d_true;
			else if (output != d_console_descriptor_null) {
				if (p_chamber_device_test_condition(code))
					snprintf(message, d_console_output_size, "%strue%s\n", v_console_styles[e_console_style_green],
							v_console_styles[e_console_style_reset]);
				else
					snprintf(message, d_console_output_size, "%sfalse%s\n", v_console_styles[e_console_style_red],
							v_console_styles[e_console_style_reset]);
				write(output, message, f_string_strlen(message));
				fsync(output);
			}
			result = d_true;
		}
	} else if (output != d_console_descriptor_null) {
		snprintf(message, d_console_output_size, "no condition (-bt, -lt or -et) has been selected\n");
		write(output, message, f_string_strlen(message));
		fsync(output);
	}
	return result;
}

int p_chamber_device_set_flag(unsigned char code, enum e_chamber_device_flags flag, char **tokens, size_t elements, int output) {
	char message[d_console_output_size], *flag_descriptions[] = {
		"empty",
		"device",
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
		v_chamber_device[code].flag[d_chamber_device_defined][flag] = d_false;
		snprintf(message, d_console_output_size, "%s is %sOFF%s\n", flag_descriptions[flag], v_console_styles[e_console_style_red],
				v_console_styles[e_console_style_reset]);
	} else if ((f_console_parameter("-on", tokens, elements, d_true)) >= 0) {
		v_chamber_device[code].flag[d_chamber_device_defined][flag] = d_true;
		snprintf(message, d_console_output_size, "%s is %sON%s\n", flag_descriptions[flag], v_console_styles[e_console_style_green],
				v_console_styles[e_console_style_reset]);
	} else {
		result = d_false;
		snprintf(message, d_console_output_size, "neither -off nor -on has been specified for entry %s\n", flag_descriptions[flag]);
	}
	if (output != d_console_descriptor_null) {
		write(output, message, f_string_strlen(message));
		fsync(output);
	}
	return result;
}

int f_chamber_device_set_chamber(unsigned char code, char **tokens, size_t elements, int output) {
	return p_chamber_device_set_flag(code, e_chamber_device_flag_start, tokens, elements, output);
}

int f_chamber_device_set_dehumidifier(unsigned char code, char **tokens, size_t elements, int output) {
	return p_chamber_device_set_flag(code, e_chamber_device_flag_dehumidifier, tokens, elements, output);
}

int f_chamber_device_set_co2(unsigned char code, char **tokens, size_t elements, int output) {
	return p_chamber_device_set_flag(code, e_chamber_device_flag_co2, tokens, elements, output);
}

int f_chamber_device_sleep(unsigned char code, char **tokens, size_t elements, int output) {
	char message[d_console_output_size], *raw_value = tokens[f_console_parameter("-s", tokens, elements, d_false)];
	int value, result = d_false;
	if ((value = atoi(raw_value)) > 0) {
		v_chamber_device[code].active_from = time(NULL)+value;
		result = d_true;
	} else if (output != d_console_descriptor_null) {
		snprintf(message, d_console_output_size, "%d is not a valid sleep value\n", value);
		write(output, message, f_string_strlen(message));
		fsync(output);
	}
	return result;
}

int f_chamber_device_enabled(unsigned char code) {
	return d_true; /* every chamber is enabled */
}

int f_chamber_device_initialize(unsigned char code) {
	int result = d_true;
	if (v_chamber_device[code].descriptor == d_rs232_null) {
		if ((result = f_rs232_open(v_chamber_device[code].link, e_rs232_baud_9600, e_rs232_bits_8, e_rs232_stops_1_bit, e_rs232_parity_no,
						e_rs232_flow_control_software, &(v_chamber_device[code].descriptor),
						&(v_chamber_device[code].old_configuration))))
			memset(&(v_chamber_device[code].temperature), 0, (sizeof(float)*e_chamber_device_temperature_null*2));
		else
			v_chamber_device[code].descriptor = d_rs232_null;
	}
	return result;
}

int f_chamber_device_destroy(unsigned char code) {
	int result = d_true;
	if (v_chamber_device[code].descriptor != d_rs232_null) {
		f_rs232_close_termios(v_chamber_device[code].descriptor, v_chamber_device[code].old_configuration);
		v_chamber_device[code].descriptor = d_rs232_null;
	}
	return result;
}

int p_chamber_device_refresh_status(unsigned char code) {
	static unsigned char *chamber_status = "$00I\r\n";
	unsigned char buffer[d_chamber_device_message_size], *pointer, *next;
	int result = d_false, step = 0;
	size_t readed, length;
	if (f_chamber_device_initialize(code)) {
		if ((f_rs232_write(v_chamber_device[code].descriptor, chamber_status, f_string_strlen(chamber_status))) > 0) {
			memset(buffer, 0, d_chamber_device_message_size);
			if (((readed = f_rs232_read_packet(v_chamber_device[code].descriptor, buffer, d_chamber_device_message_size, d_chamber_device_timeout,
								NULL, NULL, 0)) > 0) && (readed == d_chamber_device_answer_size)) {
				pointer = buffer;
				while ((step < e_chamber_device_temperature_null) && (next = strchr(pointer, ' '))) {
					*next = '\0';
					v_chamber_device[code].temperature[d_chamber_device_current][step++] = atof(pointer);
					pointer = (next+1);
				}
				if ((length = f_string_strlen(pointer)) > e_chamber_device_flag_null)
					for (step = 0; (length > 0) && (step != e_chamber_device_flag_null); ++pointer, --length)
						if (*pointer != ' ')
							v_chamber_device[code].flag[d_chamber_device_current][step++] = (*pointer=='1')?d_true:d_false;
				result = d_true;
			}
		}
		if (!result)
			f_chamber_device_destroy(code);
	}
	return result;
}

void p_chamber_device_refresh_console(unsigned char code, struct s_console *console, time_t current_timestamp) {
	char prefix[d_console_output_size] = {'\0'}, buffer[d_console_output_size], test_symbols[] = {'-', '>', '<', '='};
	float actual_value;
	if (v_chamber_device[code].active_from > current_timestamp) {
		snprintf(prefix, d_console_output_size, "{%ssleeping for %d seconds%s}", v_console_styles[e_console_style_blue],
				(int)(v_chamber_device[code].active_from-current_timestamp), v_console_styles[e_console_style_reset]);
	} else if (v_chamber_device[code].condition.running) {
		actual_value = v_chamber_device[code].temperature[d_chamber_device_current][v_chamber_parameters[v_chamber_device[code].condition.parameter_pointer].temperature];
		snprintf(prefix, d_console_output_size, "{%swaiting until %6.01f %c %6.01f%s}", v_console_styles[e_console_style_blue],
				v_chamber_device[code].condition.value, test_symbols[v_chamber_device[code].condition.test], actual_value,
				v_console_styles[e_console_style_reset]);
		if ((current_timestamp-v_chamber_device[code].last_condition) > d_chamber_device_condition_timeout) {
			if (p_chamber_device_test_condition(code))
				v_chamber_device[code].condition.running = d_false;
			v_chamber_device[code].last_condition = current_timestamp;
		}
	}
	if (console) {
		f_chamber_device_description(code, buffer, d_console_output_size);
		snprintf(console->prefix, d_console_output_size, "\r%s%s > ", prefix, buffer);
	}
}

int f_chamber_device_refresh(unsigned char code, struct s_console *console) {
	int index, result = d_true;
	time_t current_timestamp = time(NULL);
	if (console) {
		console->input_enabled = d_true;
		for (index = 0; index < d_chamber_device_entries; ++index)
			if ((v_chamber_device[index].active_from > current_timestamp) || (v_chamber_device[index].condition.running)) {
				console->input_enabled = d_false;
				break;
			}
	}
	if ((current_timestamp-v_chamber_device[code].last_refresh) > d_chamber_device_refresh_timeout)
		if (v_chamber_device[code].descriptor != d_rs232_null) {
			p_chamber_device_refresh_status(code);
			v_chamber_device[code].last_refresh = current_timestamp;
		}
	p_chamber_device_refresh_console(code, console, current_timestamp);
	return result;
}

