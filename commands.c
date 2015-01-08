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
#include "commands.h"
#include "device.h"
struct s_console_command v_commands[] = {
	{"status", "(usage: status) update details about thermic chamber",
		(struct s_console_parameter[]){
			{.initialized = d_false}
		},
		&f_commands_status, e_console_level_guest, d_true
	},
	{"configure", "(usage: configure) update configuration on the thermic chamber",
		(struct s_console_parameter[]){
			{.initialized = d_false}
		},
		&f_commands_configure, e_console_level_guest, d_true
	},
	{"set", "(usage: set -k <key> -v <value>) set a single entry to a certain value",
		(struct s_console_parameter[]){
			{"-k", "(enum between main|pt100_A|pt100_B|pt100_C|pt100_D|fan) the key that you want to edit", d_false, d_false, d_true},
			{"-v", "(float) the new value for the specified key", d_false, d_false, d_true},
			{.initialized = d_false}
		},
		&f_commands_set, e_console_level_guest, d_true
	},
	{"test", "(usage: test -k <key> -bt/-lt/-et <value> | test -k <key> -lt <value> -w) test an entry for a certain condition",
		(struct s_console_parameter[]){
			{"-k", "(enum main|pt100_A|pt100_B|pt100_C|pt100_D|fan) the key that you want to test", d_false, d_false, d_true},
			{"-bt", "(float, group -bt, -lt, -et) check if the key is bigger than a specified value", d_false, d_true, d_true},
			{"-lt", "(float, group -bt, -lt, -et) check if the key is lower than a specified value", d_false, d_true, d_true},
			{"-et", "(float, group -bt, -lt, -et) check if the key is equal to a specified value", d_false, d_true, d_true},
			{"-w", "(flag) wait until the specified condition is satisfied (ctrl-c to interrupt)", d_true, d_true, d_true},
			{.initialized = d_false}
		},
		&f_commands_test, e_console_level_guest, d_true
	},
	{"sleep", "(usage: sleep -s <value>) wait <value> seconds before continue",
		(struct s_console_parameter[]){
			{"-s", "(integer) number of seconds that you want to wait", d_false, d_false, d_true},
			{.initialized = d_false}
		},
		&f_commands_wait, e_console_level_guest, d_true
	},
	d_death_valley_commands_SET(chamber,"(usage: start -on | start -off) this command turns on/off the thermal chamber"),
	d_death_valley_commands_SET(dehumidifier, "(usage: dehumidifier -on | dehumidifier -off) this command turns on/off the dehumidifier of the chamber"),
	d_death_valley_commands_SET(co2, "(usage: co2 -on | co2 -off) this command turns on/off the CO2 cooler"),
	{"load", "(usage: load -f /path/file.dv) this command runs a batch of actions",
		(struct s_console_parameter[]){
			{"-f", "(string) path to the file", d_false, d_false, d_true},
			{.initialized = d_false}
		},
		&f_commands_load, e_console_level_guest, d_true
	},
	{.initialized = d_false}
};
char v_commands_queue[d_death_valley_commands_queue][d_string_buffer_size];
int v_commands_pointer = d_death_valley_commands_empty_queue;
int f_commands_status(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_status(tokens, elements, output);
}

int f_commands_configure(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_configure(tokens, elements, output);
}

int f_commands_set(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_set(tokens, elements, output);
}

int f_commands_test(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_test(tokens, elements, output);
}

int f_commands_wait(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	char *raw_value = tokens[f_console_parameter("-s", tokens, elements, d_false)];
	int value = atoi(raw_value), result = d_true;
	time_t begin, current;
	tc_interrupt = d_false;
	begin = current = time(NULL);
	while ((!tc_interrupt) && ((current-begin) < value)) {
		if (((current-begin)%d_death_valley_commands_wait_steps) == 0)
			p_device_status_retrieve();
		usleep(d_death_valley_commands_wait);
		current = time(NULL);
	}
	return result;
}

int f_commands_set_chamber(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_set_chamber(tokens, elements, output);
}

int f_commands_set_dehumidifier(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_set_dehumidifier(tokens, elements, output);
}

int f_commands_set_co2(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_set_co2(tokens, elements, output);
}

int f_commands_load(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	char message[d_string_buffer_size], buffer[d_string_buffer_size], *pointer, *raw_file = tokens[f_console_parameter("-f", tokens, elements, d_false)];
	FILE *stream;
	int result = d_false, index = 0;
	memset(v_commands_queue, 0, (d_death_valley_commands_queue*d_string_buffer_size));
	v_commands_pointer = d_death_valley_commands_empty_queue;
	if ((stream = fopen(raw_file, "r"))) {
		while (!feof(stream))
			if (fgets(buffer, d_string_buffer_size, stream)) {
				if ((pointer = strchr(buffer, '#')))
					*pointer = '\0';
				f_string_trim(buffer);
				if (f_string_strlen(buffer) > 0)
					strncpy(v_commands_queue[index++], buffer, d_string_buffer_size);
			}
		fclose(stream);
		if (index > 0)
			v_commands_pointer = 0;
		result = d_true;
	} else if (output != d_console_descriptor_null) {
		snprintf(message, d_string_buffer_size, "file \"%s\" not found\n", raw_file);
		write(output, message, f_string_strlen(message));
	}
	return result;
}
