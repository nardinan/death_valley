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
#ifndef death_valley_death_valley_h
#define death_valley_death_valley_h
#include <signal.h>
#include "commands.h"
#include "device.h"
#define d_death_valley_module(st,ds,mod) if(((st)==d_true)&&(p_death_valley_init_verbose((ds),(mod))))
#define d_death_valley_exit_command "quit"
#define d_death_valley_version "0.6"
#define d_death_valley_loop_sleep 100
extern const char *v_greetings[];
extern void p_death_valley_sigpipe_ignore(int signal);
extern void p_death_valley_sigint_interrupt(int signal);
extern void p_death_valley_init_greetings(int descriptor, size_t header_lenght);
extern int p_death_valley_init_verbose(int descriptor, const char *subsystem);
extern int f_death_valley_init(int descriptor);
#endif
