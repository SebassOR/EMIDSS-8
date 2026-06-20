/*
 * timeCounter.h
 *
 *  Created on: 16 jun. 2026
 *      Author: grtre
 */

#ifndef ECU_TIMECOUNTER_IF_INC_TIMECOUNTER_H_
#define ECU_TIMECOUNTER_IF_INC_TIMECOUNTER_H_

#include "Platform_Types.h"

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

#endif /* ECU_TIMECOUNTER_IF_INC_TIMECOUNTER_H_ */
