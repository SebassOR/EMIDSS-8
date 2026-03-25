/*
 * Delay.c
 *
 *  Created on: Mar 15, 2024
 *  Author: JARS
 *  Version: 1.0v
 *  Ultima modif.: -
 *  Author: -
 *
 */

/* Headers */
#include "S32K144W.h"
#include "Platform_Types.h"
#include "Std_Types.h"
#include "interrupt_manager.h"
#include "GPIO_def.h"
/*	Own headers */
#include "Delay.h"


/*	Macros	*/

/*	Defines	*/

/*	Types	*/

/* Variables */

volatile uint32 u32vTicks = (uint32)0;
volatile uint32_t mils_to_minutes_counter;
volatile uint32_t mils_to_second_counter;

/*	Private functions */

/*	Public functions	*/

void Delay_vPauseMs(uint32 u32DelayMs){
	for(uint32 u32x = 0; u32x < u32DelayMs; u32x++){
		for(uint16 u16x = 0; u16x < 6200; u16x++){
			/*	Nothing to do	*/
		}

	}
}

void Delay_vMs(uint32 u32DelayMs){

	uint32 u32TickStart = (uint32)Delay_u32GetTicks();
	uint32 u32Ticks = 0;

	while(u32Ticks <= u32DelayMs){
		u32Ticks = (uint32)Delay_u32GetTicks();
		u32Ticks = (u32Ticks - u32TickStart);
	}
	return;
}

uint32 Delay_u32GetTicks(void){

	return u32vTicks;
}

void Delay_vTicksUpdate(uint32 u32Ticks){

	u32vTicks += u32Ticks;

	return;
}

/*
 * This function return 1 when its internal counter registers 1 minute
 *
 */
uint8_t Delay_count_1_minute(void)
{
	if(mils_to_minutes_counter >= 60000)
	{
		mils_to_minutes_counter = 0;
		return 1;
	}

	return 0;
}

uint8_t Delay_count_1_second(void)
{
	/*This function have a counter that is called every 1ms. When it reach 1000ms call the Task_1s function. Simulating 1s task as an OS*/
	if(mils_to_second_counter >= 1000)
	{
		mils_to_second_counter = 0;
		return 1;
	}

	return 0;
}

/*	Interrupts functions */

void SysTick_Handler(void)
{
	/*Called every 1ms */
	Delay_vTicksUpdate(1);

	mils_to_second_counter++;
	mils_to_minutes_counter++;
	
	return;
}



/* EOF */
