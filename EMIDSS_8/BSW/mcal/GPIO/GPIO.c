/*
 * GPIO.c
 *
 *  Created on: Jun 2026
 *  Author: Gerardo Trejo
 *  Version: 1.0v
 *
 */

/* System Headers*/
#include "Siul2_Dio_Ip.h"
//#include "Platform_Types.h"
#include "Sensor_Data.h"

/* Own headers */
#include "GPIO.h"

/* 
 * Note: You should check which pin is connected to your LED in the .mex file.

 */
#define LED_PTA29_PIN           13u
#define LED_PTA29_PORT          PTA_H_HALF
void GPIO_vInit(void)
{
    /* Port initialization is already done in main.c via Siul2_Port_Ip_Init */
}

void GPIO_u32SetOutputState(uint8 u8State)
{
    if(u8State == (uint8)STD_HIGH)
    {
        Siul2_Dio_Ip_WritePin(LED_PTA29_PORT, LED_PTA29_PIN, 1U);
    }
    else
    {
        Siul2_Dio_Ip_WritePin(LED_PTA29_PORT, LED_PTA29_PIN, 0U);
    }
}

void GPIO_ToggleOutputState(void)
{
    /* 
     * Siul2_Dio_Ip_TogglePins takes a bitmask, not a raw pin index! 
     * LED_PTA29_PIN is 13, so we must bit-shift 1 by 13 to create the mask.
     */
    Siul2_Dio_Ip_TogglePins(LED_PTA29_PORT, (1U << LED_PTA29_PIN));
}

/* EOF */
