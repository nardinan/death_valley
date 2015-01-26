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
#include "log_device.h"
struct s_log_device v_log_device;
void p_log_device_refresh_status(unsigned char code, time_t current_timestamp) {
	FILE *stream;
	if ((stream = fopen(d_log_device_location, "a"))) {
		fprintf(stream, "%ld; 0x%02x; %.01f/%.01f, %.01f/%.01f, %.01f/%.01f, %.01f/%.01f, %.01f/%.01f, %.01f/%.01f | %d%d%d | %.01f, %.01f\n",
				current_timestamp, code,
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_main_actual],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_main_nominal],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_pt100_A_actual],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_pt100_A_nominal],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_pt100_B_actual],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_pt100_B_nominal],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_pt100_C_actual],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_pt100_C_nominal],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_pt100_D_actual],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_pt100_D_nominal],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_fan_actual],
				v_chamber_device[code].temperature[d_chamber_device_current][e_chamber_device_temperature_fan_nominal],
				v_chamber_device[code].flag[d_chamber_device_current][e_chamber_device_flag_start],
				v_chamber_device[code].flag[d_chamber_device_current][e_chamber_device_flag_dehumidifier],
				v_chamber_device[code].flag[d_chamber_device_current][e_chamber_device_flag_co2],
				v_hcwin_device[code].humidity, v_hcwin_device[code].temperature);
		fclose(stream);
	}
}

int f_log_device_refresh(unsigned char code, struct s_console *console) {
	int result = d_true;
	time_t current_timestamp = time(NULL);
	if ((current_timestamp-v_log_device.last_refresh) > d_log_device_refresh_timeout) {
		p_log_device_refresh_status(code, current_timestamp);
		v_log_device.last_refresh = current_timestamp;
	}
	return result;
}

