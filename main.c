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
#define d_death_valley_exit_command "quit"
#define d_death_valley_version "0.5a"
#define d_death_valley_loop_sleep 100
#define d_death_valley_loop_status 2	/* seconds */
#define d_death_valley_console_wait 10
#define d_death_valley_new_line "\n"
struct s_console *console;
struct s_console_input input = { .ready = d_true };
void p_death_valley_sigpipe_ignore(int signal) {
	/* just ignore */
}

void p_death_valley_sigint_interrupt(int signal) {
	tc_interrupt = d_true;
}

int f_death_valley_init(int descriptor) {
	char buffer[d_string_buffer_size];
	int status = d_true;
	if (descriptor != d_console_descriptor_null) {
		snprintf(buffer, d_string_buffer_size, "[Death Valley - %sVT4010%s series thermal chambers controller (version %s)]\n",
				v_console_styles[e_console_style_bold], v_console_styles[e_console_style_reset], d_death_valley_version);
		write(descriptor, buffer, f_string_strlen(buffer));
	}
	if ((status = f_console_init(&console, v_commands, STDIN_FILENO)))
		console->level = e_console_level_guest;
	return status;
}

int main (int argc, char *argv[]) {
	time_t before, current;
	f_memory_init();
	if (f_death_valley_init(STDOUT_FILENO)) {
		signal(SIGPIPE, p_death_valley_sigpipe_ignore);
		signal(SIGINT, p_death_valley_sigint_interrupt);
		f_device_configure(NULL, 0, d_console_descriptor_null);
		before = current = time(NULL);
		while (d_true) {
			if ((v_commands_pointer != d_death_valley_commands_empty_queue) && (f_string_strlen(v_commands_queue[v_commands_pointer]) > 0)) {
				memset(input.input, 0, d_string_buffer_size);
				strcpy(input.input, v_commands_queue[v_commands_pointer]);
				strcat(input.input, d_death_valley_new_line);
				input.data_length = input.data_pointer = f_string_strlen(input.input);
				f_console_refresh(console, &input, STDOUT_FILENO);
				input.ready = d_true;
				v_commands_pointer++;
			} else {
				v_commands_pointer = d_death_valley_commands_empty_queue;
				f_console_read(console, &input, STDOUT_FILENO, 0, d_death_valley_console_wait);
			}
			if (input.ready) {
				if (f_string_strcmp(input.input, d_death_valley_exit_command) == 0)
					break;
				f_console_execute(console, &input, STDOUT_FILENO);
			}
			f_device_open(d_console_descriptor_null);
			if (tc_descriptor == d_rs232_null)
				snprintf(console->prefix, d_string_buffer_size, "\r[%sdisconnected%s]> ", v_console_styles[e_console_style_red],
						v_console_styles[e_console_style_reset]);
			else
				snprintf(console->prefix, d_string_buffer_size, "\r[%sconnected%s]> ", v_console_styles[e_console_style_green],
						v_console_styles[e_console_style_reset]);
			usleep(d_death_valley_loop_sleep);
			current = time(NULL);
			if ((current-before) > d_death_valley_loop_status) {
				p_device_status_retrieve();
				before = current;
			}
		}
		f_console_destroy(&console);
	}
	f_memory_destroy();
	return 0;
}

