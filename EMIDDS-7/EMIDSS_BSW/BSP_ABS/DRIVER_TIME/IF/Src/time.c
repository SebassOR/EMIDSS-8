/*
 * time.c
 *
 *  Created on: 10 jun 2024
 *      Author: Marco Medrano
 */

#include "time.h"


// Time variable
Time_t current_time;




void Time_init_time(uint8_t hour, uint8_t minutes)
{
	current_time.hour = hour;
	current_time.minutes = minutes;
}


void Time_increase_minute(void)
{
	current_time.minutes++;

	if(current_time.minutes == 60)
	{
		current_time.hour++;
		current_time.minutes = 0;
	}

	if(current_time.hour >= 24)
	{
		current_time.hour = 0;
	}
}


uint8_t Time_get_hour(void)
{
	return current_time.hour;
}

uint8_t Time_get_minutes(void)
{
	return current_time.minutes;
}

uint8_t Time_set_hour(uint8_t new_hour)
{
	current_time.hour = new_hour;;
}

uint8_t Time_set_minutes(uint8_t new_min)
{
	current_time.minutes = new_min;
}
