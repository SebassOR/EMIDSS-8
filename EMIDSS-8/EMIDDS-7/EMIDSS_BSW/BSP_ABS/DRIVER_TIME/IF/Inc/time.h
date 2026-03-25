/*
 * time.h
 *
 *  Created on: 10 jun 2024
 *      Author: Marco Medrano
 */

#ifndef BSP_ABS_DRIVER_TIME_IF_INC_TIME_H_
#define BSP_ABS_DRIVER_TIME_IF_INC_TIME_H_

#include "S32K144W.h"

typedef struct
{
	uint8_t hour;
	uint8_t minutes;
}Time_t;


void Time_init_time(uint8_t hour, uint8_t minutes);
void Time_increase_minute(void);
uint8_t Time_get_hour(void);
uint8_t Time_get_minutes(void);
uint8_t Time_set_hour(uint8_t);
uint8_t Time_set_minutes(uint8_t);


#endif /* BSP_ABS_DRIVER_TIME_IF_INC_TIME_H_ */
