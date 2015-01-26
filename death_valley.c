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
#include "death_valley.h"
const char *v_greetings[] = {
	"it's got electrolytes!",
	"now with the double of vitamins!",
	"and also try the bugs-free version!",
	"now with the double of CPU consumption!",
	NULL
};
struct s_console *console;
struct s_console_input input = { .ready = d_true };
struct s_device v_initialized_devices[] = {
	{0x00, "VT4010 Thermical chamber",
		{&f_chamber_device_status, &f_chamber_device_apply, &f_chamber_device_set, &f_chamber_device_test, &f_chamber_device_set_chamber,
			&f_chamber_device_set_dehumidifier, &f_chamber_device_set_co2, NULL, &f_chamber_device_sleep},
		{&f_chamber_device_enabled, &f_chamber_device_initialize, &f_chamber_device_destroy},
		&f_chamber_device_refresh
	},
	{0x00, "HC2-WIN Humidity probe",
		{&f_hcwin_device_status, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
		{&f_hcwin_device_enabled, &f_hcwin_device_initialize, &f_hcwin_device_destroy},
		&f_hcwin_device_refresh
	},
	{0x00, "VT4010 & HC2-WIN log system",
		{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, NULL, NULL},
		&f_log_device_refresh
	},
	{0xcc, "Telnet server",
		{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, &f_telnet_device_initialize, &f_telnet_device_destroy},
		&f_telnet_device_refresh
	},{0xff}
};
void p_death_valley_sigpipe_ignore(int signal) {
	/* just ignore */
}

void p_death_valley_sigint_interrupt(int signal) {
	int index;
	for (index = 0; index < d_chamber_device_entries; ++index) {
		v_chamber_device[index].active_from = (time(NULL)-1);
		v_chamber_device[index].condition.running = d_false;
	}
}

void p_death_valley_init_greetings(int descriptor, size_t header_length) {
	const char *postfix = "\n\n";
	char prefix[d_console_output_size] = {0}, buffer[d_console_output_size];
	int index, value;
	size_t length;
	if (descriptor != d_console_descriptor_null) {
		for (index = 0; v_greetings[index]; ++index);
		srand(time(NULL));
		value = (rand()%index);
		if ((length = snprintf(buffer, d_console_output_size, "%s%s ... %s ...%s%s", v_console_styles[e_console_style_white],
				v_console_styles[e_console_style_italic], v_greetings[value], v_console_styles[e_console_style_reset], postfix)) <
				header_length) {
			header_length = (header_length-(length-f_string_strlen(postfix)))/2;
			memset(prefix, ' ', header_length);
			write(descriptor, prefix, f_string_strlen(prefix));
		}
		write(descriptor, buffer, f_string_strlen(buffer));
		fsync(descriptor);
	}
}

int p_death_valley_init_verbose(int descriptor, const char *subsystem) {
	char buffer[d_string_buffer_size];
	int result = d_true;
	if (descriptor != d_console_descriptor_null) {
		snprintf(buffer, d_string_buffer_size, "[^] initializing %s\"%s\"%s module\n", v_console_styles[e_console_style_green],
				subsystem, v_console_styles[e_console_style_reset]);
		write(descriptor, buffer, f_string_strlen(buffer));
		fsync(descriptor);
	}
	return result;
}

int f_death_valley_init(int descriptor) {
	char buffer[d_console_output_size], prefix[d_console_output_size];
	int index, status = d_true;
	size_t length;
	if (descriptor != d_console_descriptor_null) {
		snprintf(prefix, d_console_output_size, " ____  ____    __   ____  _   _    _  _  __    __    __    ____  _  _     \n"
	 						"(  _ \\( ___)  /__\\ (_  _)( )_( )  ( \\/ )/__\\  (  )  (  )  ( ___)( \\/ ) \n"
	  						" )(_) ))__)  /(__)\\  )(   ) _ (    \\  //(__)\\  )(__  )(__  )__)  \\  /   \n"
							"(____/(____)(__)(__)(__) (_) (_)    \\/(__)(__)(____)(____)(____) (__)      \n");
		write(descriptor, prefix, f_string_strlen(prefix));
		length = snprintf(buffer, d_console_output_size, "[Death Valley %sVT4010%s series thermal devices controller (version %s)]\n",
				v_console_styles[e_console_style_bold], v_console_styles[e_console_style_reset],
				d_death_valley_version);
		write(descriptor, buffer, f_string_strlen(buffer));
		p_death_valley_init_greetings(descriptor, length);
	}
	d_death_valley_module(status, descriptor, "console") {
		if ((status = f_console_init(&console, v_commands, STDIN_FILENO))) {
			strcpy(console->prefix, "\r[input]>");
			console->level = e_console_level_guest;
		}
	}
	d_death_valley_module(status, descriptor, "devices") {
		v_devices = v_initialized_devices;
		if (descriptor != d_console_descriptor_null) {
			for (index = 0; v_devices[index].code != 0xff; index++) {
				snprintf(buffer, d_console_output_size, "\t[required unity] name: %s%-32s%s\tcode: 0x%02x\n",
						v_console_styles[e_console_style_bold], v_devices[index].description, v_console_styles[e_console_style_reset],
						v_devices[index].code);
				write(descriptor, buffer, f_string_strlen(buffer));
			}
			fsync(descriptor);
		}
		f_device_system_recall(e_device_system_calls_initialize, d_console_descriptor_null);
	}
	return status;
}

int main (int argc, char *argv[]) {
	f_memory_init();
	if (argc >= 2) {
		strncpy(v_chamber_device[0].link, argv[1], d_string_buffer_size);
		if (argc >= 3)
			strncpy(v_hcwin_device[0].link, argv[2], d_string_buffer_size);
		if (f_death_valley_init(STDOUT_FILENO)) {
			signal(SIGPIPE, p_death_valley_sigpipe_ignore);
			signal(SIGINT, p_death_valley_sigint_interrupt);
			while (d_true) {
				f_console_read(console, &input, STDOUT_FILENO, 0, 10);
				if (input.ready) {
					if (f_string_strcmp(input.input, d_death_valley_exit_command) == 0)
						break;
					f_console_execute(console, &input, STDOUT_FILENO);
				}
				usleep(d_death_valley_loop_sleep);
				f_device_system_refresh(console);
			}
			f_device_system_recall(e_device_system_calls_destroy, STDOUT_FILENO);
			f_console_destroy(&console);
		}
	} else
		fprintf(stderr, "usage: %s <path to VT4010 chamber> {path to HC2-WIN humidity probe}\n", argv[0]);
	f_memory_destroy();
	return 0;
}

