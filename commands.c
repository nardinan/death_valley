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
	{"status", "(usage: status) view details about thermic chamber",
		(struct s_console_parameter[]){
			{.initialized = d_false}
		},
		&f_commands_status, e_console_level_guest, d_true
	},
	{"apply", "(usage: apply) load current configuration on the thermic chamber",
		(struct s_console_parameter[]){
			{.initialized = d_false}
		},
		&f_commands_apply, e_console_level_guest, d_true
	},
	{"set", "(usage: set -k main -v 100.0 | set -k main -v -30) set a single entry to a certain value",
		(struct s_console_parameter[]){
			{"-k", "(enum between main|pt100_A|pt100_B|pt100_C|pt100_D|fan) the key that you want to edit", d_false, d_false, d_true},
			{"-v", "(float) the new value for the specified key", d_false, d_false, d_true},
			{.initialized = d_false}
		},
		&f_commands_set, e_console_level_guest, d_true
	},
	{"test", "(usage: test -k main -et 30 | test -k main -lt -25 -w) test an entry for a certain condition",
		(struct s_console_parameter[]){
			{"-k", "(enum main|pt100_A|pt100_B|pt100_C|pt100_D|fan) the key that you want to test", d_false, d_false, d_true},
			{"-bt", "(float, group -bt, -lt, -et) check if the key is bigger than a specified value", d_false, d_true, d_true},
			{"-lt", "(float, group -bt, -lt, -et) check if the key is lower than a specified value", d_false, d_true, d_true},
			{"-et", "(float, group -bt, -lt, -et) check if the key is equal to a specified value", d_false, d_true, d_true},
			{"-w", "(flag) sleep until the specified condition is satisfied (ctrl-c to interrupt)", d_true, d_true, d_true},
			{.initialized = d_false}
		},
		&f_commands_test, e_console_level_guest, d_true
	},
	d_commands_SET(chamber,		"(usage: chamber -on | chamber -off) this command turns on/off the thermal device"),
	d_commands_SET(dehumidifier, 	"(usage: dehumidifier -on | dehumidifier -off) this command turns on/off the dehumidifier of the device"),
	d_commands_SET(co2, 		"(usage: co2 -on | co2 -off) this command turns on/off the CO2 cooler"),
	{"load", "(usage: load -f batches/test_15jan2015.dv) this command runs a batch of actions",
		(struct s_console_parameter[]){
			{"-f", "(string) path to the file", d_false, d_false, d_true},
			{.initialized = d_false}
		},
		&f_commands_load, e_console_level_guest, d_true
	},
	{"sleep", "(usage: sleep -s 15) sleep <value> seconds before continue",
		(struct s_console_parameter[]){
			{"-s", "(integer) number of seconds that you want to sleep", d_false, d_false, d_true},
			{.initialized = d_false}
		},
		&f_commands_sleep, e_console_level_guest, d_true
	},
	{.initialized = d_false}
};
char v_commands_queue[d_commands_queue][d_string_buffer_size];
int v_commands_pointer = d_commands_empty_queue;
int f_commands_status(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_status, d_false, tokens, elements, output);
}

int f_commands_apply(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_apply, d_false, tokens, elements, output);
}

int f_commands_set(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_set, d_false, tokens, elements, output);
}

int f_commands_test(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_test, d_false, tokens, elements, output);
}

int f_commands_set_chamber(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_chamber, d_false, tokens, elements, output);
}

int f_commands_set_dehumidifier(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_dehumidifier, d_false, tokens, elements, output);
}

int f_commands_set_co2(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_co2, d_false, tokens, elements, output);
}

int f_commands_load(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_load, d_false, tokens, elements, output);
}

int f_commands_sleep(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output) {
	return f_device_recall(e_device_calls_sleep, d_false, tokens, elements, output);
}

