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
#ifndef death_valley_log_device_h
#define death_valley_log_device_h
#include "console.h"
#include "chamber_device.h"
#include "hcwin_device.h"
#define d_log_device_refresh_timeout 3 	/* seconds */
#define d_log_device_location "./death_valley.log"
struct s_log_device {
	time_t last_refresh;
} s_log_device;
extern struct s_log_device v_log_device;
extern int f_log_device_refresh(unsigned char code, struct s_console *console);
#endif
