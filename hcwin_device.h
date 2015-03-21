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
#ifndef death_valley_hcwin_device_h
#define death_valley_hcwin_device_h
#include <miranda/ground.h>
#define d_hcwin_device_entries 1
#define d_hcwin_device_humidity_position 1
#define d_hcwin_device_temperature_position 5
#define d_hcwin_device_refresh_timeout 3 	/* seconds */
#define d_hcwin_device_timeout 75000		/* useconds */
#define d_hcwin_device_message_size 128
#define d_hcwin_device_answer_size 105
typedef struct s_hcwin_device {
	/* do not touch */
	int descriptor;
	char link[d_string_buffer_size];
	/* ok, now you can add your values */
	float humidity, temperature;
	struct termios old_configuration;
	time_t last_refresh;
} s_hcwin_device;
extern struct s_hcwin_device v_hcwin_device[d_hcwin_device_entries];
extern int f_hcwin_device_status(unsigned char code, char **tokens, size_t elements, int output);
extern int f_hcwin_device_enabled(unsigned char code);
extern int f_hcwin_device_initialize(unsigned char code);
extern int f_hcwin_device_destroy(unsigned char code);
extern int p_hcwin_device_refresh_status(unsigned char code);
extern int f_hcwin_device_refresh(unsigned char code, struct s_console *console);
#endif
