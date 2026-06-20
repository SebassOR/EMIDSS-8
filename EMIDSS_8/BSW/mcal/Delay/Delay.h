/*
 * Delay.h
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: -
 *  Author: -
 *
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "Platform_Types.h"
#include "Std_Types.h"
#include "OsIf.h"

extern void Delay_vPauseMs(uint32 u32DelayMs);
extern void Delay_vMs(uint32 u32DelayMs);
extern uint32 Delay_u32GetTicks(void);

uint8_t Delay_count_1_minute(void);
uint8_t Delay_count_1_second(void);

#endif /* DELAY_H_ */
