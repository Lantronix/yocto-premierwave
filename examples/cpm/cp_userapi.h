/* cp_userapi.h - CP configuration library header */

/* Copyright (c) 2019, Lantronix, Inc. All rights reserved. */

/*
modification history
--------------------
01a,05aug08,nss  written.
*/

#ifndef _CP_USERAPI_H
#define _CP_USERAPI_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#define CP_MAX 13

#define INPUT  0 
#define OUTPUT 1

#define UNSET  0
#define SET    1

extern int cp_config_type_set (int number, int value);
extern int cp_config_state_set (int number, int value);
extern int cp_config_invert_set (int number, int value);
extern int cp_config_level_input (int number, int *pValue);
extern int cp_config_data_input (int number, int *pValue);
extern int cp_config_data_output (int number, int value);

bool isSupported(int pin);
int cp_init_io(int pin);
int cp_reset_io(int pin);
int cp_set_direction_out(int pin);
int cp_set_direction_in(int pin);
int cp_set_output_high(int pin);
int cp_set_output_low(int pin);
int cp_set_active_low(int pin);
int cp_set_active_high(int pin);
int cp_get_pin_value(int pin, int *value);

#endif	/* _CP_CONFIG_H */
