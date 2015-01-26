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
#include "hcwin_device.h"
struct s_hcwin_device v_hcwin_device[d_hcwin_device_entries] = {
	{ d_rs232_null, "/dev/ttyUSB1"}
};

int f_hcwin_device_status(unsigned char code, char **tokens, size_t elements, int output) {
	char informations[d_console_output_size];
	int result = d_true;
	p_hcwin_device_refresh_status(code);
	if (output != d_console_descriptor_null) {
		snprintf(informations, d_console_output_size, "\n%sHC2-WIN probe%s:\n\t[%shumidity   %s %6.01f %%RH]\n\t[%stemperature%s %6.01f   C]\n",
				v_console_styles[e_console_style_yellow], v_console_styles[e_console_style_reset], v_console_styles[e_console_style_bold],
				v_console_styles[e_console_style_reset], v_hcwin_device[code].humidity, v_console_styles[e_console_style_bold],
				v_console_styles[e_console_style_reset], v_hcwin_device[code].temperature);
		write(output, informations, f_string_strlen(informations));
		fsync(output);
	}
	return result;
}

int f_hcwin_device_enabled(unsigned char code) {
	return d_true; /* every hcwin is enabled */
}

int f_hcwin_device_initialize(unsigned char code) {
	int result = d_true;
	if (v_hcwin_device[code].descriptor == d_rs232_null) {
		if (!(result = f_rs232_open(v_hcwin_device[code].link, e_rs232_baud_19200, e_rs232_bits_8, e_rs232_stops_1_bit, e_rs232_parity_no,
						e_rs232_flow_control_no, &(v_hcwin_device[code].descriptor), &(v_hcwin_device[code].old_configuration))))
			v_hcwin_device[code].descriptor = d_rs232_null;
	}
	return result;
}

int f_hcwin_device_destroy(unsigned char code) {
	int result = d_true;
	if (v_hcwin_device[code].descriptor != d_rs232_null) {
		f_rs232_close_termios(v_hcwin_device[code].descriptor, v_hcwin_device[code].old_configuration);
		v_hcwin_device[code].descriptor = d_rs232_null;
	}
	return result;
}

int p_hcwin_device_refresh_status(unsigned char code) {
	static unsigned char *hcwin_status = "{F00RDD}\r\n";
	unsigned char buffer[d_hcwin_device_message_size], *pointer, *next;
	int result = d_false, step = 0;
	size_t readed;
	if ((f_hcwin_device_initialize(code))) {
		if ((f_rs232_write(v_hcwin_device[code].descriptor, hcwin_status, f_string_strlen(hcwin_status))) > 0) {
			memset(buffer, 0, d_hcwin_device_message_size);
			if (((readed = f_rs232_read_packet(v_hcwin_device[code].descriptor, buffer, d_hcwin_device_message_size, d_hcwin_device_timeout,
								NULL, NULL, 0)) > 0) && (readed == d_hcwin_device_answer_size)) {
				pointer = buffer;
				while ((next = strchr(pointer, ';'))) {
					*next = '\0';
					switch(step) {
						case d_hcwin_device_humidity_position:
							v_hcwin_device[code].humidity = atof(pointer);
							break;
						case d_hcwin_device_temperature_position:
							v_hcwin_device[code].temperature = atof(pointer);
						default:
							break;
					}
					pointer = (next+1);
					step++;
				}
				result = d_true;
			}
		}
		if (!result)
			f_hcwin_device_destroy(code);
	}
	return result;
}

int f_hcwin_device_refresh(unsigned char code, struct s_console *console) {
	int result = d_true;
	time_t current_timestamp = time(NULL);
	if ((current_timestamp-v_hcwin_device[code].last_refresh) > d_hcwin_device_refresh_timeout)
		if (v_hcwin_device[code].descriptor != d_rs232_null) {
			p_hcwin_device_refresh_status(code);
			v_hcwin_device[code].last_refresh = current_timestamp;
		}
	return result;
}

