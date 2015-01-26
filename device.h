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
#ifndef death_valley_device_h
#define death_valley_device_h
#include "console.h"
#include "chamber_device.h"
#include "hcwin_device.h"
#include "log_device.h"
#include "telnet_device.h"
#define d_chamber_device_elements 1
typedef enum e_device_calls {
	e_device_calls_status = 0,
	e_device_calls_apply,
	e_device_calls_set,
	e_device_calls_test,
	e_device_calls_chamber,
	e_device_calls_dehumidifier,
	e_device_calls_co2,
	e_device_calls_load,
	e_device_calls_sleep,
	e_device_calls_null
} e_device_calls;
typedef enum e_device_system_calls {
	e_device_system_calls_is_enabled = 0,
	e_device_system_calls_initialize,
	e_device_system_calls_destroy,
	e_device_system_calls_null
} e_device_system_calls;
typedef int (*t_device_call_generic)(unsigned char);
typedef int (*t_device_call)(unsigned char, char **, size_t, int output);
typedef int (*t_device_call_refresh)(unsigned char, struct s_console *);
typedef struct s_device {
	unsigned char code;
	char *description;
	t_device_call calls[e_device_calls_null];
	t_device_call_generic system_calls[e_device_system_calls_null];
	t_device_call_refresh refresh_call;
} s_device;
extern struct s_device *v_devices;
extern const char *v_device_call_description[], *v_device_system_call_description[];
extern int f_device_recall(enum e_device_calls call, int skip_mask, char **tokens, size_t elements, int output);
extern int f_device_system_recall(enum e_device_system_calls call, int output);
extern int f_device_system_refresh(struct s_console *console);
#endif
