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
#ifndef death_valley_commands_h
#define death_valley_commands_h
#include "console.h"
#define d_death_valley_commands_queue 1024
#define d_death_valley_commands_empty_queue -1
#define d_death_valley_commands_wait 100000
#define d_death_valley_commands_wait_steps 10
#define d_death_valley_commands_SET(k,d)\
{\
	#k,d,(struct s_console_parameter[]){\
		{"-on","(flag) enable the feature",d_false,d_true,d_true},\
		{"-off","(flag) disable the feature",d_false,d_true,d_true},\
		{.initialized=d_false}\
	},&f_commands_set_##k,e_console_level_guest,d_true\
}
extern struct s_console_command v_commands[];
extern char v_commands_queue[d_death_valley_commands_queue][d_string_buffer_size];
extern int v_commands_pointer;
extern int f_commands_status(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
extern int f_commands_configure(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
extern int f_commands_set(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
extern int f_commands_test(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
extern int f_commands_wait(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
extern int f_commands_set_chamber(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
extern int f_commands_set_dehumidifier(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
extern int f_commands_set_co2(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
extern int f_commands_load(struct s_console *console, struct s_console_command *command, char **tokens, size_t elements, int output);
#endif
